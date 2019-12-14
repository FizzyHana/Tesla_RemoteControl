#include "systick.h"
#include "sys.h"

volatile unsigned long long int SystickTime = 0;   
volatile unsigned long long int g_TimingTickOld = 0;

// 使用系统时钟中断产生时间  
	
//void SysTick_init(u8 SYSCLK){
//	u8 fac_us=0;							//us延时倍乘数		
//	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 	// 表示使用的时钟节拍为HCLK/8=168M/8=21M
//	fac_us=SYSCLK/8;													//表示延时1us时钟需要的节拍数（SYSCLK应该为168）	 
//	SysTick->LOAD=1*fac_us; 									//定时时间加载寄存器(1um的时间)每次不能大于798915us
//	SysTick->VAL=0x00;        								//清空当前计数器的值
//	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk ; //开启中断
//	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ; 	//开始倒数 	
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




// 使用通用中断产生时间  

void SysTick_init(u8 SYSCLK){
	u8 fac_us=0;							//us延时倍乘数		
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 	// 表示使用的时钟节拍为HCLK/8=168M/8=21M
	fac_us=SYSCLK/8;													//表示延时1us时钟需要的节拍数（SYSCLK应该为168）	 
	SysTick->LOAD=1*fac_us; 									//定时时间加载寄存器(1um的时间)每次不能大于798915us
	SysTick->VAL=0x00;        								//清空当前计数器的值
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk ; //开启中断
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ; 	//开始倒数 	
}
	
//void SysTick_Handler(void)
//{	
//	SystickTime ++ ;
//}

unsigned long long int GetSystickTime(void)
{	
	return SystickTime;
}



//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器2!
void Indicator_Time2_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///使能TIM2时钟
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//初始化TIM2
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //允许定时器2更新中断
	TIM_Cmd(TIM2,ENABLE); //使能定时器2
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

//定时器2中断服务函数
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //溢出中断
	{
		SystickTime ++ ;
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
}
