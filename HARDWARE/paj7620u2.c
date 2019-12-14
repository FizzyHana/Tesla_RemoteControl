#include "paj7620u2.h"
#include "paj7620u2_cfg.h"
#include "get_usart_code.h"
#include "delay.h"
#include "usart.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK MiniV3 STM32开发板
//PAJ7620U2 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2017/7/1
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////



//选择PAJ7620U2 BANK区域
void paj7620u2_selectBank(bank_e bank)
{
	switch(bank)
	{
		case BANK0: GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK0);break;//BANK0寄存器区域
		case BANK1: GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK1);break;//BANK1寄存器区域
	}
}

//PAJ7620U2唤醒
u8 paj7620u2_wakeup(void)
{ 
	u8 data=0x0a;
	GS_WakeUp();//唤醒PAJ7620U2
	delay_ms(5);//唤醒时间>400us
	GS_WakeUp();//唤醒PAJ7620U2
	delay_ms(5);//唤醒时间>400us
	paj7620u2_selectBank(BANK0);//进入BANK0寄存器区域
	data = GS_Read_Byte(0x00);//读取状态
	if(data!=0x20) return 0; //唤醒失败
	
	return 1;
}

//PAJ7620U2初始化
//返回值：0:失败 1:成功
u8 paj7620u2_init(void)
{
	u8 i;
	u8 status;
	
	GS_i2c_init();//IIC初始化
    status = paj7620u2_wakeup();//唤醒PAJ7620U2
	if(!status) return 0;
	paj7620u2_selectBank(BANK0);//进入BANK0寄存器区域
	for(i=0;i<INIT_SIZE;i++)
	{
		GS_Write_Byte(init_Array[i][0],init_Array[i][1]);//初始化PAJ7620U2
	}
    paj7620u2_selectBank(BANK0);//切换回BANK0寄存器区域
	
	return 1;
}

/**
  * @brief  手势识别发送控制指令
	* @param  ges_value：手势识别的值		
  * @retval 无
  */
void Ges_sent(ges_e ges_value)
{
	u8 code = get_gesture_usart_code(ges_value);		//获得对应按键对应指令
	usart_send_command(code);												//发送相应指令	
	delay_us(9500);																	//延时9.5ms
}

/**
  * @brief  初始化手势识别功能
	* @param  无
  * @retval 无
  */
void Gesture_init(void)
{
	u8 i;
	paj7620u2_selectBank(BANK0);//进入BANK0寄存器区域
	for(i=0;i<GESTURE_SIZE;i++)
	{
		GS_Write_Byte(gesture_arry[i][0],gesture_arry[i][1]);//手势识别模式初始化
	}
	paj7620u2_selectBank(BANK0);//切换回BANK0寄存器区域
	i=0;
}

/**
  * @brief  检测手势
	* @param  无
  * @retval 无
  * @note		由于手势识别模块安装方向与正常的逆时
						针旋转了90度，发送的指令为模块检测的方向顺时针旋转了
						90度的结果
  */
void Gesture_detect(void)
{
	static bool Ges_inited = false;						//手势识别初始化状态
	u16 gesture_data;
	u8 status;
	u8 data[2]={0x00};
	
	/****** 首次进入函数初始化手势识别功能 ******/
	if(!Ges_inited)
	{
		Gesture_init();
		Ges_inited = true;
	}
	
	/****** 读取手势识别状态 ******/
	status = GS_Read_nByte(PAJ_GET_INT_FLAG1,2,&data[0]);			
	if(!status)
	{   
		gesture_data =(u16)data[1]<<8 | data[0];
		if(gesture_data) 
		{
			switch(gesture_data)
			{
				case GES_UP:								//模块检测到上（实际向左）
					Ges_sent(LEFT);
					break;
				case GES_DOWN:							//模块检测到下（实际向右）
					Ges_sent(RIGHT);
					break;
				case GES_LEFT:							//模块检测到下（实际向右）
					Ges_sent(DOWN);
					break;
				case GES_RIGHT:							//模块检测到右（实际向上）
					Ges_sent(UP);
					break;
				case GES_FORWARD:						//靠近模块停车、蜂鸣器
					Ges_sent(FORWARD);
					break;
				case GES_BACKWARD:					//远离模块关闭所有灯
					Ges_sent(BACKWARD);
					break;
				case GES_WAVE:							//挥手响蜂鸣器
					Ges_sent(WAVE);
					break;
				default:
					break;
			}						
		}
	}
}
