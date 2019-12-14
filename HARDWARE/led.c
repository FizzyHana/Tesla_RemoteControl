#include "led.h"

/*************************
	函数名：led_init
	输入：无
	输出：无
	作用：初始化LED灯
*************************/
void led_init(void)
{    
	/****** 1、定义GPIO_InitTypeDef类型结构体 ******/
	GPIO_InitTypeDef GPIO_InitStructure;

	/****** 2、使能led相关gpio口时钟 ******/
	RCC_APB2PeriphClockCmd(LED_GPIO_CLK, ENABLE);
		
	/****** 3、初始化led灯相应gpio口 ******/
	GPIO_InitStructure.GPIO_Pin = EXT_LED_GPIO_PINS;				//控制led所有引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//设置为推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;				//引脚速率为2Mhz
	GPIO_Init(EXT_LED_GPIO_PORT, &GPIO_InitStructure);				//调用库函数初始化GPIOB     

	GPIO_InitStructure.GPIO_Pin = INT_LED_GPIO_PINS;				//控制led所有引脚				//引脚速率为2Mhz
	GPIO_Init(INT_LED_GPIO_PORT, &GPIO_InitStructure);				//调用库函数初始化GPIOB     
	
	/****** 4、默认置高电平关闭LED灯 ******/
	GPIO_SetBits(GPIOB, EXT_LED_GPIO_PINS);		//外置灯
	GPIO_SetBits(GPIOC, INT_LED_GPIO_PINS);		//内置灯
}

/*************************
	函数名：led_reverse
	输入：控制的LED Flag
	输出：无
	作用：反转相应LED的亮灭状态，使其亮时熄灭，灭时点亮
*************************/
void led_reverse(led_e LED)
{
	switch(LED)
	{
		case BTH:
		{
			if(GPIO_ReadOutputDataBit(EXT_LED_GPIO_PORT, BTH_LED_PORT))		
				GPIO_ResetBits(EXT_LED_GPIO_PORT, BTH_LED_PORT);				//点亮led
			else
				GPIO_SetBits(EXT_LED_GPIO_PORT, BTH_LED_PORT);				
			break;
		} 
		case ACT:
		{
			if(GPIO_ReadOutputDataBit(EXT_LED_GPIO_PORT, ACT_LED_PORT))
				GPIO_ResetBits(EXT_LED_GPIO_PORT, ACT_LED_PORT);				
			else
				GPIO_SetBits(EXT_LED_GPIO_PORT, ACT_LED_PORT);
			break;
		}
		case STATUS:
		{
			if(GPIO_ReadOutputDataBit(INT_LED_GPIO_PORT, STATUS_LED_PORT))
				GPIO_ResetBits(INT_LED_GPIO_PORT, STATUS_LED_PORT);				
			else
				GPIO_SetBits(INT_LED_GPIO_PORT, STATUS_LED_PORT);
			break;
		}
	}
}

/*************************
	函数名：ACT_LED
	输入operation
	说明：led灯亮灭操作
	类型：bool

	输出：无
	作用：控制动作LED灯的亮灭
*************************/
void ACT_LED(bool operation)
{
	if(operation)
		GPIO_ResetBits(GPIOB, ACT_LED_PORT);
	else
		GPIO_SetBits(GPIOB, ACT_LED_PORT);
}

/*************************
	函数名：BTH_LED
	输入operation
	说明：led灯亮灭操作
	类型：bool

	输出：无
	作用：控制蓝牙连接LED灯的亮灭
*************************/
void BTH_LED(bool operation)
{
	if(operation)
		GPIO_ResetBits(GPIOB, BTH_LED_PORT);
	else
		GPIO_SetBits(GPIOB, BTH_LED_PORT);
}

/*************************
	函数名：STATUS_LED
	输入operation
	说明：led灯亮灭操作
	类型：bool

	输出：无
	作用：控制按键菜单状态LED灯的亮灭
*************************/
void STATUS_LED(bool operation)
{
	if(operation)
		GPIO_ResetBits(GPIOC, STATUS_LED_PORT);
	else
		GPIO_SetBits(GPIOC, STATUS_LED_PORT);
}
