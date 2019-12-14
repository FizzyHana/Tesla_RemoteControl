#include "key.h"
#include "led.h"
#include "delay.h" 
#include "usart.h"
#include "get_usart_code.h"

/**
  * ������ֵ��
  * 1����Դ��
  * 2���м���ֹͣ����
  * 3�����
  * 4���Ҽ�
  * 5���ϼ�
  * 6���¼�
**/

struct key							//��������
{	
	bool longpress_flag;			//����״̬
	u16 longpress;					//��ť����ʱ��
};

char menu = 1;						//�����˵�

/**
  * @brief 	��ʼ������
  * @param  ��			
  * @retval ��
  */
void key_init(void)
{
	/****** 1���������� ******/
	GPIO_InitTypeDef GPIO_InitStructure;						//GPIO�ṹ��
	EXTI_InitTypeDef EXTI_InitStructure;						//�ⲿ�ж�
	NVIC_InitTypeDef NVIC_InitStructure;						//�ж����ȼ�
	
	/****** 2����ʼ����ťʱ�� ******/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | 
						RCC_APB2Periph_AFIO, ENABLE);					//ʹ��GPIOAʱ��
	
	/****** 3��������ӦGPIO�� ******/
	GPIO_InitStructure.GPIO_Pin = KEY_GPIO_PINS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				//��������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		//10MHZ
	GPIO_Init(GPIOA, &GPIO_InitStructure);							//��ʼ��GPIOA0-6��
	
	/****** 4�����ð����ⲿ�ж������ȼ� ******/
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);						//ʹ���ж�������2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	/****** ��Դ ******/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, POWER_KEY_EXTI_SOURCE);	//����POWER����GPIO���ж���
	EXTI_InitStructure.EXTI_Line = POWER_KEY_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;				//�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//�½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;				
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;			//��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;							//�����ȼ�Ϊ0
	NVIC_Init(&NVIC_InitStructure);
	
	/****** �м���ֹͣ���� ******/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, CENTRE_KEY_EXTI_SOURCE);	//����CENTRE����GPIO���ж���
	
	EXTI_InitStructure.EXTI_Line = CENTRE_KEY_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;			//��ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;							//�����ȼ�Ϊ0
	NVIC_Init(&NVIC_InitStructure);
	
	/****** �Ҽ� ******/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, RIGHT_KEY_EXTI_SOURCE);
	
	EXTI_InitStructure.EXTI_Line = RIGHT_KEY_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;			//��ռ���ȼ�Ϊ2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;							//�����ȼ�Ϊ0
	NVIC_Init(&NVIC_InitStructure);
	
	/****** ��� ******/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, LEFT_KEY_EXTI_SOURCE);
	
	EXTI_InitStructure.EXTI_Line = LEFT_KEY_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure); 
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;			//��ռ���ȼ�Ϊ3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;							//�����ȼ�Ϊ0
	NVIC_Init(&NVIC_InitStructure);	
	
	/****** �¼� ******/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, DOWN_KEY_EXTI_SOURCE);
	
	EXTI_InitStructure.EXTI_Line = DOWN_KEY_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;			//��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;							//�����ȼ�Ϊ1
	NVIC_Init(&NVIC_InitStructure);
	
	/****** �ϼ� ******/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, UP_KEY_EXTI_SOURCE);
	
	EXTI_InitStructure.EXTI_Line = UP_KEY_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;			//��ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;							//�����ȼ�Ϊ1
	NVIC_Init(&NVIC_InitStructure);
} 

/**
  * @brief  �������Ϳ���ָ��
  * @param  KeyValue : ���µİ�ť������������POWER��CENTRE��
					RIGHT��LEFT��FORWARD��BACK
			menu : ��ǰ�����˵�
			long_pressed_status���Ƿ񳤰�
			backup_ops���Ƿ�Ϊ����ָ����ƴ���������led��ʱ��Ч��			
  * @retval ��
  */
void key_sent(key_e KeyValue, char menu, bool long_pressed_status, bool backup)
{
	u8 code = get_key_usart_code(KeyValue, menu, long_pressed_status, backup);		//��ö�Ӧ������Ӧָ��
	usart_send_command(code);					//������Ӧָ��	
	delay_us(9500);										//��ʱ9.5ms�Է�ֹ��������������
}

/************ �жϷ������ **********/

/**
  * @brief  ��Դ���жϷ�����������л������˵�
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(POWER_KEY_EXTI_LINE) != RESET)
	{
		delay_ms(7);
		/****** ��Դ������ ******/
		if(POWER_KEY == 0)
		{
			if(menu == 1)
			{
				menu = 2;
				STATUS_LED(true);
			}
			else
			{
				menu = 1;
				STATUS_LED(false);				
			}
		}
	}
	EXTI_ClearITPendingBit(POWER_KEY_EXTI_LINE);
}

/**
  * @brief 	�м���ֹͣ�����жϷ������
  * @param  None
  * @retval None
  */
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(CENTRE_KEY_EXTI_LINE) != RESET)
	{
		delay_ms(7);
		/****** �м����� ******/
		if(CENTRE_KEY == 0)
		{
			key_sent(KEY_CENTRE, menu, false, false);	
			if(menu == 2)
				EXTI_ClearITPendingBit(CENTRE_KEY_EXTI_LINE);

		}
		else
		{
			if(menu == 1)
				usart_send_command(0x00);						//����ָֹͣ����ֹͣС��
			EXTI_ClearITPendingBit(CENTRE_KEY_EXTI_LINE);
		}
	}

}

/**
  * @brief  �Ҽ��жϷ������
  * @param  None
  * @retval None
  */
void EXTI2_IRQHandler(void)
{
	struct key right_key;
	if(EXTI_GetITStatus(RIGHT_KEY_EXTI_LINE) != RESET)
	{
		delay_ms(7);
		/****** �Ҽ����� ******/
		if(RIGHT_KEY == 0)
		{
			if(menu == 1)								//�ڿ���С���˶��˵�
			{
				key_sent(KEY_RIGHT, menu, false, false);
			}
			/****** ����Ƿ񳤰� ******/
			if(right_key.longpress >= 70)
				right_key.longpress_flag = true;
			else
				++right_key.longpress;
		}
		/****** �Ҽ����� ******/
		else
		{
			if(menu == 1)								//�ڿ���С���˶��˵�
				usart_send_command(0x00);
			else										//�ڿ��Ƶ��봰���Ĳ˵�
			{
				key_sent(KEY_RIGHT, menu, right_key.longpress_flag, false);
				/****** �̰�ʱ���Ϳ��Ʒ��䱸��ledָ�� ******/
				if(right_key.longpress_flag == false)
					delay_ms(1);
					key_sent(KEY_RIGHT, menu, right_key.longpress_flag, true);
				/****** ���������־ ******/				
				right_key.longpress_flag = false;
				right_key.longpress = 0;
			}
			EXTI_ClearITPendingBit(RIGHT_KEY_EXTI_LINE);

		}
	}

}

/**
  * @brief  ����жϷ������
  * @param  None
  * @retval None
  */
void EXTI3_IRQHandler(void)
{
	struct key left_key;
	if(EXTI_GetITStatus(LEFT_KEY_EXTI_LINE) != RESET)
	{
		delay_ms(7);
		/****** ������� ******/
		if(LEFT_KEY == 0)							//���°���
		{
			if(menu == 1)
				key_sent(KEY_LEFT, menu, false, false);
			/****** ����Ƿ񳤰� ******/
			if(left_key.longpress >= 70)
				left_key.longpress_flag = true;
			else
				++left_key.longpress;
		}
		/****** ������� ******/
		else
		{
			if(menu == 1)							//�ڿ���С���˶��˵�
				usart_send_command(0x00);
			else									//�ڿ��Ƶ��봰���Ĳ˵�
			{
				key_sent(KEY_LEFT, menu, left_key.longpress_flag, false);
				/****** �̰�ʱ���Ϳ��ƴ�������ledָ�� ******/
				if(left_key.longpress_flag == false)
					delay_ms(1);
					key_sent(KEY_LEFT, menu, left_key.longpress_flag, true);
				/****** ���������־ ******/
				left_key.longpress_flag = false;
				left_key.longpress = 0;
			}
			EXTI_ClearITPendingBit(LEFT_KEY_EXTI_LINE);
		}
	}
}

/**
  * @brief  �¼��жϷ������
  * @param  None
  * @retval None
  */
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(DOWN_KEY_EXTI_LINE) != RESET)
	{
		delay_ms(7);
		/****** �¼����� ******/
		if(DOWN_KEY == 0)										//���°�ť
		{
			key_sent(KEY_BACK, menu, false, false);					//���������ָ��
			if(menu == 2)
				EXTI_ClearITPendingBit(DOWN_KEY_EXTI_LINE);		//�ڲ˵�2ʱ�˳������ж�
		}
		/****** �¼����� ******/
		else													//�ɿ�����
		{
			if(menu == 1)
				usart_send_command(0x00);						//����ָֹͣ����ֹͣС��
			EXTI_ClearITPendingBit(DOWN_KEY_EXTI_LINE);
		}
	}
}

/**
  * @brief  �ϼ��жϷ������
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(UP_KEY_EXTI_LINE) != RESET)
	{
		delay_ms(7);
		/****** �ϼ����� ******/
		if(UP_KEY == 0)
		{
			key_sent(KEY_FORWARD, menu, false, false);
			if(menu == 2)
				EXTI_ClearITPendingBit(UP_KEY_EXTI_LINE);		//�ڲ˵�2ʱ�˳������ж�
		}
		/****** �ϼ����� ******/
		else
		{
			if(menu == 1)
				usart_send_command(0x00);
			EXTI_ClearITPendingBit(UP_KEY_EXTI_LINE);
		}
	}
}
