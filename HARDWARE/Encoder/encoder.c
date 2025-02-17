#include "encoder.h"
#include "sys.h"
/**************************************************************************
函数功能：把TIM4初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
```c
// Khai b谩o v脿 kh峄焛 t岷 ch峄ヽ n膬ng encoder tr锚n timer TIM4
void Encoder_Init_TIM4(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; // Khai b谩o c岷 tr煤c 膽峄� kh峄焛 t岷 GPIO
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; // Khai b谩o c岷 tr煤c 膽峄� kh峄焛 t岷 timer
	TIM_ICInitTypeDef TIM_ICInitStructure; // Khai b谩o c岷 tr煤c 膽峄� kh峄焛 t岷 ch岷� 膽峄� capture c峄 timer
	NVIC_InitTypeDef NVIC_InitStructure; // Khai b谩o c岷 tr煤c 膽峄� kh峄焛 t岷 ng岷痶
	
	// B岷璽 clock cho TIM4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	// B岷璽 clock cho GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// C岷 h矛nh GPIOB_PIN6 v脿 GPIOB_PIN7 l脿 膽岷 v脿o kh么ng k茅o l锚n
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// C岷 h矛nh nh贸m 瓢u ti锚n ng岷痶
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	// C岷 h矛nh ng岷痶 cho TIM4
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// C岷 h矛nh timer TIM4
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	// C岷 h矛nh ch岷� 膽峄� encoder cho TIM4
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	
	// X贸a c峄� ng岷痶 v脿 b岷璽 ng岷痶 cho TIM4
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_Trigger, ENABLE);
	
	// B岷璽 timer TIM4
	TIM_Cmd(TIM4, ENABLE);
}
```

// Khai b谩o v脿 kh峄焛 t岷 ch峄ヽ n膬ng encoder tr锚n timer TIM8
void Encoder_Init_TIM8(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; // Khai b谩o c岷 tr煤c 膽峄� kh峄焛 t岷 GPIO
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; // Khai b谩o c岷 tr煤c 膽峄� kh峄焛 t岷 timer
	TIM_ICInitTypeDef TIM_ICInitStructure; // Khai b谩o c岷 tr煤c 膽峄� kh峄焛 t岷 ch岷� 膽峄� capture c峄 timer
	NVIC_InitTypeDef NVIC_InitStructure; // Khai b谩o c岷 tr煤c 膽峄� kh峄焛 t岷 ng岷痶
	
	// B岷璽 clock cho TIM8
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	// B岷璽 clock cho GPIOC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	// C岷 h矛nh GPIOC_PIN6 v脿 GPIOC_PIN7 l脿 膽岷 v脿o kh么ng k茅o l锚n
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	// C岷 h矛nh nh贸m 瓢u ti锚n ng岷痶
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	// C岷 h矛nh ng岷痶 cho TIM8
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_IRQn | TIM8_TRG_COM_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// C岷 h矛nh timer TIM8
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
	
	// C岷 h矛nh ch岷� 膽峄� encoder cho TIM8
	TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM8, &TIM_ICInitStructure);
	
	// X贸a c峄� ng岷痶 v脿 b岷璽 ng岷痶 cho TIM8
	TIM_ClearFlag(TIM8, TIM_FLAG_Update);
	TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);
	TIM_ITConfig(TIM8, TIM_IT_Trigger, ENABLE);
	
	// B岷璽 timer TIM8
	TIM_Cmd(TIM8, ENABLE);
}

int Read_Encoder(u8 TIMX)
{
    int Encoder_TIM;    
   switch(TIMX)
	 {
	   case 4:  Encoder_TIM= (short)TIM4 -> CNT;  TIM4 -> CNT=0;break;	
		 case 8:  Encoder_TIM= (short)TIM8 -> CNT;  TIM8 -> CNT=0;break;		
		 default:  Encoder_TIM=0;
	 }
		return Encoder_TIM;
}

void TIM4_IRQHandler(void)
{ 		    		  			    
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)//溢出中断
	{    				   				     	    	
	}				   
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);   //清除中断标志位   
}

void TIM8_IRQHandler(void)
{ 		    		  			    
	if(TIM_GetITStatus(TIM8, TIM_IT_Update) == SET)//溢出中断
	{    				   				     	    	
	}				   
	TIM_ClearITPendingBit(TIM8, TIM_IT_Update);   //清除中断标志位	
}
