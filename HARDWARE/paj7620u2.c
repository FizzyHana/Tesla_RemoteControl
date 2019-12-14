#include "paj7620u2.h"
#include "paj7620u2_cfg.h"
#include "get_usart_code.h"
#include "delay.h"
#include "usart.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK MiniV3 STM32������
//PAJ7620U2 ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2017/7/1
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////



//ѡ��PAJ7620U2 BANK����
void paj7620u2_selectBank(bank_e bank)
{
	switch(bank)
	{
		case BANK0: GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK0);break;//BANK0�Ĵ�������
		case BANK1: GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK1);break;//BANK1�Ĵ�������
	}
}

//PAJ7620U2����
u8 paj7620u2_wakeup(void)
{ 
	u8 data=0x0a;
	GS_WakeUp();//����PAJ7620U2
	delay_ms(5);//����ʱ��>400us
	GS_WakeUp();//����PAJ7620U2
	delay_ms(5);//����ʱ��>400us
	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	data = GS_Read_Byte(0x00);//��ȡ״̬
	if(data!=0x20) return 0; //����ʧ��
	
	return 1;
}

//PAJ7620U2��ʼ��
//����ֵ��0:ʧ�� 1:�ɹ�
u8 paj7620u2_init(void)
{
	u8 i;
	u8 status;
	
	GS_i2c_init();//IIC��ʼ��
    status = paj7620u2_wakeup();//����PAJ7620U2
	if(!status) return 0;
	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	for(i=0;i<INIT_SIZE;i++)
	{
		GS_Write_Byte(init_Array[i][0],init_Array[i][1]);//��ʼ��PAJ7620U2
	}
    paj7620u2_selectBank(BANK0);//�л���BANK0�Ĵ�������
	
	return 1;
}

/**
  * @brief  ����ʶ���Ϳ���ָ��
	* @param  ges_value������ʶ���ֵ		
  * @retval ��
  */
void Ges_sent(ges_e ges_value)
{
	u8 code = get_gesture_usart_code(ges_value);		//��ö�Ӧ������Ӧָ��
	usart_send_command(code);												//������Ӧָ��	
	delay_us(9500);																	//��ʱ9.5ms
}

/**
  * @brief  ��ʼ������ʶ����
	* @param  ��
  * @retval ��
  */
void Gesture_init(void)
{
	u8 i;
	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	for(i=0;i<GESTURE_SIZE;i++)
	{
		GS_Write_Byte(gesture_arry[i][0],gesture_arry[i][1]);//����ʶ��ģʽ��ʼ��
	}
	paj7620u2_selectBank(BANK0);//�л���BANK0�Ĵ�������
	i=0;
}

/**
  * @brief  �������
	* @param  ��
  * @retval ��
  * @note		��������ʶ��ģ�鰲װ��������������ʱ
						����ת��90�ȣ����͵�ָ��Ϊģ����ķ���˳ʱ����ת��
						90�ȵĽ��
  */
void Gesture_detect(void)
{
	static bool Ges_inited = false;						//����ʶ���ʼ��״̬
	u16 gesture_data;
	u8 status;
	u8 data[2]={0x00};
	
	/****** �״ν��뺯����ʼ������ʶ���� ******/
	if(!Ges_inited)
	{
		Gesture_init();
		Ges_inited = true;
	}
	
	/****** ��ȡ����ʶ��״̬ ******/
	status = GS_Read_nByte(PAJ_GET_INT_FLAG1,2,&data[0]);			
	if(!status)
	{   
		gesture_data =(u16)data[1]<<8 | data[0];
		if(gesture_data) 
		{
			switch(gesture_data)
			{
				case GES_UP:								//ģ���⵽�ϣ�ʵ������
					Ges_sent(LEFT);
					break;
				case GES_DOWN:							//ģ���⵽�£�ʵ�����ң�
					Ges_sent(RIGHT);
					break;
				case GES_LEFT:							//ģ���⵽�£�ʵ�����ң�
					Ges_sent(DOWN);
					break;
				case GES_RIGHT:							//ģ���⵽�ң�ʵ�����ϣ�
					Ges_sent(UP);
					break;
				case GES_FORWARD:						//����ģ��ͣ����������
					Ges_sent(FORWARD);
					break;
				case GES_BACKWARD:					//Զ��ģ��ر����е�
					Ges_sent(BACKWARD);
					break;
				case GES_WAVE:							//�����������
					Ges_sent(WAVE);
					break;
				default:
					break;
			}						
		}
	}
}
