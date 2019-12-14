#include <stdbool.h>

#include "stm32f10x.h"
#include "stm32f10x_it.h"

#include "usart.h"
#include "key.h"
#include "led.h"
#include "get_usart_code.h"
#include "delay.h"
#include "paj7620u2.h"
#include "sys.h"

#define sys_clock 		72							//时钟频率为72Mhz
#define usart_bound 	115200					//波特率为115200

int main(void)
{
	/****** 定义变量 ******/
	
	/****** 初始化模块 ******/
	led_init();
	key_init();
	delay_init(sys_clock);
	USART1_init(usart_bound);
	while(!paj7620u2_init())						//PAJ7620U2传感器初始化
	{
//	    printf("PAJ7620U2 Error!!!n");	
	}

	for(;;)
	{
		/****** 指示蓝牙连接状态 ******/
		detect_bluetooth_status();
		
		/****** 实现手势识别功能 ******/
		Gesture_detect();	
	}
}
