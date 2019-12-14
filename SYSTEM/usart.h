#ifndef __USART_H
#define __USART_H
#include <stdio.h>
#include <stdbool.h>
#include "stm32f10x_conf.h"
#include "sys.h" 

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
//#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
extern char recvbuf;						// ���ڽ�����Ч�ַ�
extern char recvend;						// ������ɱ�־

/****** ����HC05 state���Ų��� ******/
#define HC05_STATE_GPIO			GPIOB
#define HC05_STATE_PIN			GPIO_Pin_15
#define HC05_STATE_CLK			RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO
#define HC05_STATE_LINE	EXTI_Line15

//����봮���жϽ��գ��벻Ҫע�����º궨��

/****** �����б� ******/
void USART1_init(u32 band);
void USART1_IRQHandler(void);
void detect_bluetooth_status(void);

void usart_send_command(u8 motion);
void usart_recv_data(void);
#endif
