#ifndef __LED_H
#define __LED_H

#include <stdbool.h>
#include "sys.h"

/****** 定义包含所有LED的GPIO口信息 ******/
#define EXT_LED_GPIO_PINS	GPIO_Pin_0 | GPIO_Pin_1				//外接LED的GPIO引脚
#define INT_LED_GPIO_PINS 	GPIO_Pin_13
#define EXT_LED_GPIO_PORT	GPIOB								//外接LED对应的GPIO口		
#define INT_LED_GPIO_PORT	GPIOC								//内置LED对应的GPIO口		
#define LED_GPIO_CLK	RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC		//GPIO口对应时钟

/****** 定义LED灯对应GPIO口 ******/
#define BTH_LED_PORT 		GPIO_Pin_0							//蓝牙连接状态LED
#define ACT_LED_PORT 		GPIO_Pin_1							//串口收发LED灯
#define STATUS_LED_PORT 	GPIO_Pin_13							//内置LED（按键菜单状态指示）

/****** 定义开关标志 ******/
#define ON 	true
#define OFF false

typedef enum{
	BTH = 0,
	ACT,
	STATUS,
}led_e;

/****** 函数列表 ******/
void led_init(void);									//初始化LED灯	 
void led_reverse(led_e LED);								//反转LED灯

void ACT_LED(bool operation);							//控制动作led的亮灭
void BTH_LED(bool operation);							//控制蓝牙连接状态led的亮灭
void STATUS_LED(bool operation);						//控制状态指示led灯的亮灭
#endif
