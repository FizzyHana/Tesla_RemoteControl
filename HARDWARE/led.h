#ifndef __LED_H
#define __LED_H

#include <stdbool.h>
#include "sys.h"

/****** �����������LED��GPIO����Ϣ ******/
#define EXT_LED_GPIO_PINS	GPIO_Pin_0 | GPIO_Pin_1				//���LED��GPIO����
#define INT_LED_GPIO_PINS 	GPIO_Pin_13
#define EXT_LED_GPIO_PORT	GPIOB								//���LED��Ӧ��GPIO��		
#define INT_LED_GPIO_PORT	GPIOC								//����LED��Ӧ��GPIO��		
#define LED_GPIO_CLK	RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC		//GPIO�ڶ�Ӧʱ��

/****** ����LED�ƶ�ӦGPIO�� ******/
#define BTH_LED_PORT 		GPIO_Pin_0							//��������״̬LED
#define ACT_LED_PORT 		GPIO_Pin_1							//�����շ�LED��
#define STATUS_LED_PORT 	GPIO_Pin_13							//����LED�������˵�״ָ̬ʾ��

/****** ���忪�ر�־ ******/
#define ON 	true
#define OFF false

typedef enum{
	BTH = 0,
	ACT,
	STATUS,
}led_e;

/****** �����б� ******/
void led_init(void);									//��ʼ��LED��	 
void led_reverse(led_e LED);								//��תLED��

void ACT_LED(bool operation);							//���ƶ���led������
void BTH_LED(bool operation);							//������������״̬led������
void STATUS_LED(bool operation);						//����״ָ̬ʾled�Ƶ�����
#endif
