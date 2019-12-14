#include "systick.h"
#include "sys.h"

volatile unsigned long long int SystickTime = 0;   
volatile unsigned long long int g_TimingTickOld = 0;

// ʹ��ϵͳʱ���жϲ���ʱ��  
	
//void SysTick_init(u8 SYSCLK){
//	u8 fac_us=0;							//us��ʱ������		
//	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 	// ��ʾʹ�õ�ʱ�ӽ���ΪHCLK/8=168M/8=21M
//	fac_us=SYSCLK/8;													//��ʾ��ʱ1usʱ����Ҫ�Ľ�������SYSCLKӦ��Ϊ168��	 
//	SysTick->LOAD=1*fac_us; 									//��ʱʱ����ؼĴ���(1um��ʱ��)ÿ�β��ܴ���798915us
//	SysTick->VAL=0x00;        								//��յ�ǰ��������ֵ
//	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk ; //�����ж�
//	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ; 	//��ʼ���� 	
//}
//	
//void SysTick_Handler(void)
//{	
//	SystickTime ++ ;
//}

//unsigned long long int GetSystickTime(void)
//{	
//	return SystickTime;
//}




// ʹ��ͨ���жϲ���ʱ��  

void SysTick_init(u8 SYSCLK){
	u8 fac_us=0;							//us��ʱ������		
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 	// ��ʾʹ�õ�ʱ�ӽ���ΪHCLK/8=168M/8=21M
	fac_us=SYSCLK/8;													//��ʾ��ʱ1usʱ����Ҫ�Ľ�������SYSCLKӦ��Ϊ168��	 
	SysTick->LOAD=1*fac_us; 									//��ʱʱ����ؼĴ���(1um��ʱ��)ÿ�β��ܴ���798915us
	SysTick->VAL=0x00;        								//��յ�ǰ��������ֵ
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk ; //�����ж�
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ; 	//��ʼ���� 	
}
	
//void SysTick_Handler(void)
//{	
//	SystickTime ++ ;
//}

unsigned long long int GetSystickTime(void)
{	
	return SystickTime;
}



//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��2!
void Indicator_Time2_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///ʹ��TIM2ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//��ʼ��TIM2
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //����ʱ��2�����ж�
	TIM_Cmd(TIM2,ENABLE); //ʹ�ܶ�ʱ��2
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

//��ʱ��2�жϷ�����
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //����ж�
	{
		SystickTime ++ ;
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //����жϱ�־λ
}
