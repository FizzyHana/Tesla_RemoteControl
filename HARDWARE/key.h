#ifndef __KEY_H
#define __KEY_H	 

#include <stdbool.h>
#include "sys.h" 

#define KEY_GPIO_PINS GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5
#define EXTI_KEY_GPIO GPIO_PinSource0 | GPIO_PinSource1 | GPIO_PinSource2 | GPIO_PinSource3 | GPIO_PinSource4 | GPIO_PinSource5

#define POWER_KEY 							GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define POWER_KEY_EXTI_LINE	 		EXTI_Line0
#define POWER_KEY_EXTI_SOURCE 	GPIO_PinSource0

#define CENTRE_KEY 							GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)
#define CENTRE_KEY_EXTI_LINE 		EXTI_Line1
#define CENTRE_KEY_EXTI_SOURCE	GPIO_PinSource1

#define RIGHT_KEY 							GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)
#define RIGHT_KEY_EXTI_LINE 		EXTI_Line2
#define RIGHT_KEY_EXTI_SOURCE 	GPIO_PinSource2

#define LEFT_KEY 								GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)
#define LEFT_KEY_EXTI_LINE 			EXTI_Line3
#define LEFT_KEY_EXTI_SOURCE 		GPIO_PinSource3

#define DOWN_KEY 								GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)
#define DOWN_KEY_EXTI_LINE 			EXTI_Line4
#define DOWN_KEY_EXTI_SOURCE 		GPIO_PinSource4

#define UP_KEY									GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)
#define UP_KEY_EXTI_LINE 				EXTI_Line5
#define UP_KEY_EXTI_SOURCE 			GPIO_PinSource5

extern bool long_press_flag;							//长按标志位
extern char long_press;									//长按统计变量

typedef enum {
	KEY_POWER = 1,
	KEY_CENTRE,
	KEY_RIGHT,
	KEY_LEFT,
	KEY_FORWARD,
	KEY_BACK,
}key_e;


/*下面方式是通过位带操作方式读取IO*/
/*
#define KEY0 		PEin(4)   	//PE4
#define KEY1 		PEin(3)		//PE3 
#define KEY2 		PEin(2)		//P32
#define WK_UP 	PAin(0)		//PA0
*/


void key_init(void);	//IO初始化

u8 get_key_value(u8 mode);

void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);
void EXTI9_5_IRQHandler(void);

#endif
