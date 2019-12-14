#ifndef __USART_H
#define __USART_H
#include <stdio.h>
#include <stdbool.h>
#include "stm32f10x_conf.h"
#include "sys.h" 

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
//#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
extern char recvbuf;						// 串口接收有效字符
extern char recvend;						// 接收完成标志

/****** 定义HC05 state引脚参数 ******/
#define HC05_STATE_GPIO			GPIOB
#define HC05_STATE_PIN			GPIO_Pin_15
#define HC05_STATE_CLK			RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO
#define HC05_STATE_LINE	EXTI_Line15

//如果想串口中断接收，请不要注释以下宏定义

/****** 函数列表 ******/
void USART1_init(u32 band);
void USART1_IRQHandler(void);
void detect_bluetooth_status(void);

void usart_send_command(u8 motion);
void usart_recv_data(void);
#endif
