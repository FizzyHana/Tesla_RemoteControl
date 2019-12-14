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

#define sys_clock 		72							//ʱ��Ƶ��Ϊ72Mhz
#define usart_bound 	115200					//������Ϊ115200

int main(void)
{
	/****** ������� ******/
	
	/****** ��ʼ��ģ�� ******/
	led_init();
	key_init();
	delay_init(sys_clock);
	USART1_init(usart_bound);
	while(!paj7620u2_init())						//PAJ7620U2��������ʼ��
	{
//	    printf("PAJ7620U2 Error!!!n");	
	}

	for(;;)
	{
		/****** ָʾ��������״̬ ******/
		detect_bluetooth_status();
		
		/****** ʵ������ʶ���� ******/
		Gesture_detect();	
	}
}
