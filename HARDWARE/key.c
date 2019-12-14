#include "key.h"
#include "led.h"
#include "delay.h" 
#include "usart.h"
#include "get_usart_code.h"

/**
  * 按键键值表
  * 1：电源键
  * 2：中键（停止键）
  * 3：左键
  * 4：右键
  * 5：上键
  * 6：下键
**/

struct key							//按键数组
{	
	bool longpress_flag;			//长按状态
	u16 longpress;					//按钮按下时间
};

char menu = 1;						//按键菜单

/**
  * @brief 	初始化按键
  * @param  无			
  * @retval 无
  */
void key_init(void)
{
	/****** 1、定义数组 ******/
	GPIO_InitTypeDef GPIO_InitStructure;						//GPIO结构体
	EXTI_InitTypeDef EXTI_InitStructure;						//外部中断
	NVIC_InitTypeDef NVIC_InitStructure;						//中断优先级
	
	/****** 2、初始化按钮时钟 ******/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | 
						RCC_APB2Periph_AFIO, ENABLE);					//使能GPIOA时钟
	
	/****** 3、配置相应GPIO口 ******/
	GPIO_InitStructure.GPIO_Pin = KEY_GPIO_PINS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				//上拉输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		//10MHZ
	GPIO_Init(GPIOA, &GPIO_InitStructure);							//初始化GPIOA0-6口
	
	/****** 4、配置按键外部中断与优先级 ******/
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);						//使用中断优先组2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	/****** 电源 ******/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, POWER_KEY_EXTI_SOURCE);	//连接POWER按键GPIO与中断线
	EXTI_InitStructure.EXTI_Line = POWER_KEY_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;				//中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;				
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;			//抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;							//子优先级为0
	NVIC_Init(&NVIC_InitStructure);
	
	/****** 中键（停止键） ******/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, CENTRE_KEY_EXTI_SOURCE);	//连接CENTRE按键GPIO与中断线
	
	EXTI_InitStructure.EXTI_Line = CENTRE_KEY_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;			//抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;							//子优先级为0
	NVIC_Init(&NVIC_InitStructure);
	
	/****** 右键 ******/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, RIGHT_KEY_EXTI_SOURCE);
	
	EXTI_InitStructure.EXTI_Line = RIGHT_KEY_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;			//抢占优先级为2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;							//子优先级为0
	NVIC_Init(&NVIC_InitStructure);
	
	/****** 左键 ******/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, LEFT_KEY_EXTI_SOURCE);
	
	EXTI_InitStructure.EXTI_Line = LEFT_KEY_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure); 
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;			//抢占优先级为3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;							//子优先级为0
	NVIC_Init(&NVIC_InitStructure);	
	
	/****** 下键 ******/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, DOWN_KEY_EXTI_SOURCE);
	
	EXTI_InitStructure.EXTI_Line = DOWN_KEY_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;			//抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;							//子优先级为1
	NVIC_Init(&NVIC_InitStructure);
	
	/****** 上键 ******/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, UP_KEY_EXTI_SOURCE);
	
	EXTI_InitStructure.EXTI_Line = UP_KEY_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;			//抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;							//子优先级为1
	NVIC_Init(&NVIC_InitStructure);
} 

/**
  * @brief  按键发送控制指令
  * @param  KeyValue : 按下的按钮，参数可以是POWER、CENTRE、
					RIGHT、LEFT、FORWARD、BACK
			menu : 当前按键菜单
			long_pressed_status：是否长按
			backup_ops：是否为备用指令（控制大厅、房间led灯时有效）			
  * @retval 无
  */
void key_sent(key_e KeyValue, char menu, bool long_pressed_status, bool backup)
{
	u8 code = get_key_usart_code(KeyValue, menu, long_pressed_status, backup);		//获得对应按键对应指令
	usart_send_command(code);					//发送相应指令	
	delay_us(9500);										//延时9.5ms以防止处理器处理不过来
}

/************ 中断服务程序 **********/

/**
  * @brief  电源键中断服务程序，用于切换按键菜单
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(POWER_KEY_EXTI_LINE) != RESET)
	{
		delay_ms(7);
		/****** 电源键按下 ******/
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
  * @brief 	中键（停止键）中断服务程序
  * @param  None
  * @retval None
  */
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(CENTRE_KEY_EXTI_LINE) != RESET)
	{
		delay_ms(7);
		/****** 中键按下 ******/
		if(CENTRE_KEY == 0)
		{
			key_sent(KEY_CENTRE, menu, false, false);	
			if(menu == 2)
				EXTI_ClearITPendingBit(CENTRE_KEY_EXTI_LINE);

		}
		else
		{
			if(menu == 1)
				usart_send_command(0x00);						//发送停止指令以停止小车
			EXTI_ClearITPendingBit(CENTRE_KEY_EXTI_LINE);
		}
	}

}

/**
  * @brief  右键中断服务程序
  * @param  None
  * @retval None
  */
void EXTI2_IRQHandler(void)
{
	struct key right_key;
	if(EXTI_GetITStatus(RIGHT_KEY_EXTI_LINE) != RESET)
	{
		delay_ms(7);
		/****** 右键按下 ******/
		if(RIGHT_KEY == 0)
		{
			if(menu == 1)								//在控制小车运动菜单
			{
				key_sent(KEY_RIGHT, menu, false, false);
			}
			/****** 检测是否长按 ******/
			if(right_key.longpress >= 70)
				right_key.longpress_flag = true;
			else
				++right_key.longpress;
		}
		/****** 右键弹起 ******/
		else
		{
			if(menu == 1)								//在控制小车运动菜单
				usart_send_command(0x00);
			else										//在控制灯与窗帘的菜单
			{
				key_sent(KEY_RIGHT, menu, right_key.longpress_flag, false);
				/****** 短按时发送控制房间备用led指令 ******/
				if(right_key.longpress_flag == false)
					delay_ms(1);
					key_sent(KEY_RIGHT, menu, right_key.longpress_flag, true);
				/****** 清除长按标志 ******/				
				right_key.longpress_flag = false;
				right_key.longpress = 0;
			}
			EXTI_ClearITPendingBit(RIGHT_KEY_EXTI_LINE);

		}
	}

}

/**
  * @brief  左键中断服务程序
  * @param  None
  * @retval None
  */
void EXTI3_IRQHandler(void)
{
	struct key left_key;
	if(EXTI_GetITStatus(LEFT_KEY_EXTI_LINE) != RESET)
	{
		delay_ms(7);
		/****** 左键按下 ******/
		if(LEFT_KEY == 0)							//按下按键
		{
			if(menu == 1)
				key_sent(KEY_LEFT, menu, false, false);
			/****** 检测是否长按 ******/
			if(left_key.longpress >= 70)
				left_key.longpress_flag = true;
			else
				++left_key.longpress;
		}
		/****** 左键弹起 ******/
		else
		{
			if(menu == 1)							//在控制小车运动菜单
				usart_send_command(0x00);
			else									//在控制灯与窗帘的菜单
			{
				key_sent(KEY_LEFT, menu, left_key.longpress_flag, false);
				/****** 短按时发送控制大厅备用led指令 ******/
				if(left_key.longpress_flag == false)
					delay_ms(1);
					key_sent(KEY_LEFT, menu, left_key.longpress_flag, true);
				/****** 清除长按标志 ******/
				left_key.longpress_flag = false;
				left_key.longpress = 0;
			}
			EXTI_ClearITPendingBit(LEFT_KEY_EXTI_LINE);
		}
	}
}

/**
  * @brief  下键中断服务程序
  * @param  None
  * @retval None
  */
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(DOWN_KEY_EXTI_LINE) != RESET)
	{
		delay_ms(7);
		/****** 下键按下 ******/
		if(DOWN_KEY == 0)										//按下按钮
		{
			key_sent(KEY_BACK, menu, false, false);					//发送向后走指令
			if(menu == 2)
				EXTI_ClearITPendingBit(DOWN_KEY_EXTI_LINE);		//在菜单2时退出本次中断
		}
		/****** 下键弹起 ******/
		else													//松开按键
		{
			if(menu == 1)
				usart_send_command(0x00);						//发送停止指令以停止小车
			EXTI_ClearITPendingBit(DOWN_KEY_EXTI_LINE);
		}
	}
}

/**
  * @brief  上键中断服务程序
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(UP_KEY_EXTI_LINE) != RESET)
	{
		delay_ms(7);
		/****** 上键按下 ******/
		if(UP_KEY == 0)
		{
			key_sent(KEY_FORWARD, menu, false, false);
			if(menu == 2)
				EXTI_ClearITPendingBit(UP_KEY_EXTI_LINE);		//在菜单2时退出本次中断
		}
		/****** 上键弹起 ******/
		else
		{
			if(menu == 1)
				usart_send_command(0x00);
			EXTI_ClearITPendingBit(UP_KEY_EXTI_LINE);
		}
	}
}
