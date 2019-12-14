#include "usart.h"	
#include "delay.h"
#include "led.h"
#include <string.h>

//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"	//ʹ�� ucos
#endif

 
/****** �������´���,֧��printf����,������Ҫѡ��use MicroLIB ******/
#if 1
#pragma import(__use_no_semihosting)     

//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout; 

/*************************
	��������_sys_exit
	���룺x
	���ͣ�int

	�������
	���ã�����_sys_exit()����ʹ�ð�����ģʽ
*************************/
_sys_exit(int x) 
{ 
	x = x; 
} 

/*************************
	��������fputc
	���룺ch �ַ���f �ļ���ָ�����
	���ͣ�int��*FILE

	�����ch �ַ�
	���ͣ�int
	���ã��ض���fputc����
*************************/
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);	//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif

//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
/****** ����״̬ ******
	bit15��		������ɱ�־
	bit14��		���յ�0x0d
	bit13~0��	���յ�����Ч�ֽ���Ŀ
**********************/

u16 USART_RX_STA=0;       	//����״̬���	

/****** ������� ******/
char recv_buf;				//�������ݱ���
extern char recvend;		//������ɱ�־


/**
  * @brief  	��ʼ��USART1��NVIC�ж�
  * @param  	��	
  * @retval 	��
  * @version 	V1.0
  */
void USART1_NVIC_Init(void)
{
	/****** 1���������� ******/
	NVIC_InitTypeDef NVIC_InitStruct;
	
	/****** 2����ʼ��NVIC�ж� ******/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);			//�жϿ�������
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;			//�ж�ԴΪusart1�ж�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;	//�������ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;			//�����ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			//ʹ���ж�
	
	/****** 3�������ʼ������ ******/
	NVIC_Init(&NVIC_InitStruct);							//��ʼ������NVIC
}

/**
  * @brief  	��ʼ��BTH_LED��ؽӿ�
  * @param  	��	
  * @retval 	��
  * @version 	V1.0
  */
void HC05_state_pin_init()
{
	/****** 1������GPIO_InitTypeDef���ͽṹ�� ******/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/****** 2��ʹ��HC05 state��ؽӿ�ʱ�� ******/
	RCC_APB2PeriphClockCmd(HC05_STATE_CLK, ENABLE);
		
	/****** 3����ʼ��HC05 state��ؽӿ� ******/
	GPIO_InitStructure.GPIO_Pin = HC05_STATE_PIN;		//����led��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		//����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	//��������Ϊ2Mhz
	GPIO_Init(HC05_STATE_GPIO, &GPIO_InitStructure);
}

/**
  * @brief  	��ʼ��USART1
  * @param  	bound ������		
  * @retval 	��
  * @version 	V1.0
  */
void USART1_init(u32 bound)
{
	/****** 1���������� ******/
	USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/****** 2����ʼ��USART ******/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);				//ʹ��usart1ʱ��
	
	USART_InitStruct.USART_BaudRate = bound;							//���ò�����
																		//Ӳ����������Ϊ��
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//usartģʽ
	USART_InitStruct.USART_StopBits = USART_StopBits_1;					//ֹͣλ
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;			//����λ
	USART_Init(USART1, &USART_InitStruct);
	
	/****** 3��������Ӧgpio�� ******/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//ʹ��GPIOAʱ��
	//Tx PA9
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;				//���츴��ģʽ
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//Rx PA10
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;			//��������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	/****** 4����ʼ��USART1 NVIC�ж� ******/
	USART1_NVIC_Init();
	
	/****** 5�������ʼ������ ******/
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
	
	/****** 6����ʼ��HC-05 state���� ******/
	HC05_state_pin_init();
}

/**
  * @brief  	���Ϳ���ָ�С��
  * @param  	motion ����������Ϊ��get_usart_code��õ�ָ��			
  * @retval 	��
  * @version 	V1.0
  */
void usart_send_command(u8 motion)
{
	static char sendSec = 1;
	ACT_LED(ON);
	for(sendSec = 1; sendSec <= 3; sendSec++)
	{
		switch(sendSec)												
		{
			case 1:
				USART_SendData(USART1, motion);				//����
				break;
			case 2:
				USART_SendData(USART1, 0x0D);
				break;
			case 3:
				USART_SendData(USART1, 0x0A);
				break;
		}
		led_reverse(ACT);
		delay_us(500);
	}
}

/**
  * @brief  	������յ�USART���� 
  * @param  	��			
  * @retval 	��
  * @version 	V1.0
  */
void usart_recv_data()
{
	
}

/**
  * @brief  	USART1�жϷ������
  * @param  	��			
  * @retval 	��
  * @version 	V1.0
  */
void USART1_IRQHandler()
{
	
	/****** ��⴮���Ƿ���յ������� ******/													//���յ��˴�������
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
		usart_recv_data();						//���ղ���������
	
	/****** ��⴮���Ƿ����ڴ������� ******/
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)	//���ڴ�������
	{
		ACT_LED(OFF);
		USART_ClearITPendingBit(USART1, USART_IT_IDLE);
	}
	
/****** ���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS. ******/	
#if SYSTEM_SUPPORT_OS 						
	OSIntExit();  											 
#endif	                                                                                                                                                                                                                                                                                                          
}

/**
  * @brief  ָʾ��������״̬
  * @param  ��			
  * @retval ��
  */
void detect_bluetooth_status(void)
{
	if(GPIO_ReadInputDataBit(HC05_STATE_GPIO, HC05_STATE_PIN))
		BTH_LED(ON);
	else
		BTH_LED(OFF);
}
