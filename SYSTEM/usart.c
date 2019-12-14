#include "usart.h"	
#include "delay.h"
#include "led.h"
#include <string.h>

//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"	//使用 ucos
#endif

 
/****** 加入以下代码,支持printf函数,而不需要选择use MicroLIB ******/
#if 1
#pragma import(__use_no_semihosting)     

//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout; 

/*************************
	函数名：_sys_exit
	输入：x
	类型：int

	输出：无
	作用：定义_sys_exit()避免使用半主机模式
*************************/
_sys_exit(int x) 
{ 
	x = x; 
} 

/*************************
	函数名：fputc
	输入：ch 字符、f 文件流指针变量
	类型：int、*FILE

	输出：ch 字符
	类型：int
	作用：重定义fputc函数
*************************/
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);	//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif

//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
/****** 接收状态 ******
	bit15：		接收完成标志
	bit14：		接收到0x0d
	bit13~0：	接收到的有效字节数目
**********************/

u16 USART_RX_STA=0;       	//接收状态标记	

/****** 接收相关 ******/
char recv_buf;				//接收数据变量
extern char recvend;		//接收完成标志


/**
  * @brief  	初始化USART1的NVIC中断
  * @param  	无	
  * @retval 	无
  * @version 	V1.0
  */
void USART1_NVIC_Init(void)
{
	/****** 1、定义数组 ******/
	NVIC_InitTypeDef NVIC_InitStruct;
	
	/****** 2、初始化NVIC中断 ******/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);			//中断控制器组
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;			//中断源为usart1中断
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;	//抢断优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;			//子优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			//使能中断
	
	/****** 3、保存初始化配置 ******/
	NVIC_Init(&NVIC_InitStruct);							//初始化配置NVIC
}

/**
  * @brief  	初始化BTH_LED相关接口
  * @param  	无	
  * @retval 	无
  * @version 	V1.0
  */
void HC05_state_pin_init()
{
	/****** 1、定义GPIO_InitTypeDef类型结构体 ******/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/****** 2、使能HC05 state相关接口时钟 ******/
	RCC_APB2PeriphClockCmd(HC05_STATE_CLK, ENABLE);
		
	/****** 3、初始化HC05 state相关接口 ******/
	GPIO_InitStructure.GPIO_Pin = HC05_STATE_PIN;		//控制led所有引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		//设置为下拉输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	//引脚速率为2Mhz
	GPIO_Init(HC05_STATE_GPIO, &GPIO_InitStructure);
}

/**
  * @brief  	初始化USART1
  * @param  	bound 波特率		
  * @retval 	无
  * @version 	V1.0
  */
void USART1_init(u32 bound)
{
	/****** 1、定义数组 ******/
	USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/****** 2、初始化USART ******/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);				//使能usart1时钟
	
	USART_InitStruct.USART_BaudRate = bound;							//设置波特率
																		//硬件流控制设为无
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//usart模式
	USART_InitStruct.USART_StopBits = USART_StopBits_1;					//停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;			//数据位
	USART_Init(USART1, &USART_InitStruct);
	
	/****** 3、配置相应gpio口 ******/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//使能GPIOA时钟
	//Tx PA9
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;				//推挽复用模式
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//Rx PA10
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;			//浮空输入
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	/****** 4、初始化USART1 NVIC中断 ******/
	USART1_NVIC_Init();
	
	/****** 5、保存初始化配置 ******/
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
	
	/****** 6、初始化HC-05 state引脚 ******/
	HC05_state_pin_init();
}

/**
  * @brief  	发送控制指令到小车
  * @param  	motion 动作，参数为从get_usart_code获得的指令			
  * @retval 	无
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
				USART_SendData(USART1, motion);				//发送
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
  * @brief  	处理接收的USART数据 
  * @param  	无			
  * @retval 	无
  * @version 	V1.0
  */
void usart_recv_data()
{
	
}

/**
  * @brief  	USART1中断服务程序
  * @param  	无			
  * @retval 	无
  * @version 	V1.0
  */
void USART1_IRQHandler()
{
	
	/****** 检测串口是否接收到了数据 ******/													//接收到了串口数据
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
		usart_recv_data();						//接收并处理数据
	
	/****** 检测串口是否正在传输数据 ******/
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)	//正在传输数据
	{
		ACT_LED(OFF);
		USART_ClearITPendingBit(USART1, USART_IT_IDLE);
	}
	
/****** 如果SYSTEM_SUPPORT_OS为真，则需要支持OS. ******/	
#if SYSTEM_SUPPORT_OS 						
	OSIntExit();  											 
#endif	                                                                                                                                                                                                                                                                                                          
}

/**
  * @brief  指示蓝牙连接状态
  * @param  无			
  * @retval 无
  */
void detect_bluetooth_status(void)
{
	if(GPIO_ReadInputDataBit(HC05_STATE_GPIO, HC05_STATE_PIN))
		BTH_LED(ON);
	else
		BTH_LED(OFF);
}
