#include "led.h"

/*************************
	��������led_init
	���룺��
	�������
	���ã���ʼ��LED��
*************************/
void led_init(void)
{    
	/****** 1������GPIO_InitTypeDef���ͽṹ�� ******/
	GPIO_InitTypeDef GPIO_InitStructure;

	/****** 2��ʹ��led���gpio��ʱ�� ******/
	RCC_APB2PeriphClockCmd(LED_GPIO_CLK, ENABLE);
		
	/****** 3����ʼ��led����Ӧgpio�� ******/
	GPIO_InitStructure.GPIO_Pin = EXT_LED_GPIO_PINS;				//����led��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//����Ϊ�������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;				//��������Ϊ2Mhz
	GPIO_Init(EXT_LED_GPIO_PORT, &GPIO_InitStructure);				//���ÿ⺯����ʼ��GPIOB     

	GPIO_InitStructure.GPIO_Pin = INT_LED_GPIO_PINS;				//����led��������				//��������Ϊ2Mhz
	GPIO_Init(INT_LED_GPIO_PORT, &GPIO_InitStructure);				//���ÿ⺯����ʼ��GPIOB     
	
	/****** 4��Ĭ���øߵ�ƽ�ر�LED�� ******/
	GPIO_SetBits(GPIOB, EXT_LED_GPIO_PINS);		//���õ�
	GPIO_SetBits(GPIOC, INT_LED_GPIO_PINS);		//���õ�
}

/*************************
	��������led_reverse
	���룺���Ƶ�LED Flag
	�������
	���ã���ת��ӦLED������״̬��ʹ����ʱϨ����ʱ����
*************************/
void led_reverse(led_e LED)
{
	switch(LED)
	{
		case BTH:
		{
			if(GPIO_ReadOutputDataBit(EXT_LED_GPIO_PORT, BTH_LED_PORT))		
				GPIO_ResetBits(EXT_LED_GPIO_PORT, BTH_LED_PORT);				//����led
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
	��������ACT_LED
	����ooperation
	˵����led���������
	���ͣ�bool

	�������
	���ã����ƶ���LED�Ƶ�����
*************************/
void ACT_LED(bool operation)
{
	if(operation)
		GPIO_ResetBits(GPIOB, ACT_LED_PORT);
	else
		GPIO_SetBits(GPIOB, ACT_LED_PORT);
}

/*************************
	��������BTH_LED
	����ooperation
	˵����led���������
	���ͣ�bool

	�������
	���ã�������������LED�Ƶ�����
*************************/
void BTH_LED(bool operation)
{
	if(operation)
		GPIO_ResetBits(GPIOB, BTH_LED_PORT);
	else
		GPIO_SetBits(GPIOB, BTH_LED_PORT);
}

/*************************
	��������STATUS_LED
	����ooperation
	˵����led���������
	���ͣ�bool

	�������
	���ã����ư����˵�״̬LED�Ƶ�����
*************************/
void STATUS_LED(bool operation)
{
	if(operation)
		GPIO_ResetBits(GPIOC, STATUS_LED_PORT);
	else
		GPIO_SetBits(GPIOC, STATUS_LED_PORT);
}
