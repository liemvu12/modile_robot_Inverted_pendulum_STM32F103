#include "encoder.h"
#include "sys.h"
/**************************************************************************
�������ܣ���TIM4��ʼ��Ϊ�������ӿ�ģʽ
��ڲ�������
����  ֵ����
**************************************************************************/
```c
// Khai báo và khởi tạo chức năng encoder trên timer TIM4
void Encoder_Init_TIM4(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; // Khai báo cấu trúc để khởi tạo GPIO
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; // Khai báo cấu trúc để khởi tạo timer
	TIM_ICInitTypeDef TIM_ICInitStructure; // Khai báo cấu trúc để khởi tạo chế độ capture của timer
	NVIC_InitTypeDef NVIC_InitStructure; // Khai báo cấu trúc để khởi tạo ngắt
	
	// Bật clock cho TIM4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	// Bật clock cho GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// Cấu hình GPIOB_PIN6 và GPIOB_PIN7 là đầu vào không kéo lên
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// Cấu hình nhóm ưu tiên ngắt
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	// Cấu hình ngắt cho TIM4
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// Cấu hình timer TIM4
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	// Cấu hình chế độ encoder cho TIM4
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	
	// Xóa cờ ngắt và bật ngắt cho TIM4
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_Trigger, ENABLE);
	
	// Bật timer TIM4
	TIM_Cmd(TIM4, ENABLE);
}
```

// Khai báo và khởi tạo chức năng encoder trên timer TIM8
void Encoder_Init_TIM8(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; // Khai báo cấu trúc để khởi tạo GPIO
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; // Khai báo cấu trúc để khởi tạo timer
	TIM_ICInitTypeDef TIM_ICInitStructure; // Khai báo cấu trúc để khởi tạo chế độ capture của timer
	NVIC_InitTypeDef NVIC_InitStructure; // Khai báo cấu trúc để khởi tạo ngắt
	
	// Bật clock cho TIM8
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	// Bật clock cho GPIOC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	// Cấu hình GPIOC_PIN6 và GPIOC_PIN7 là đầu vào không kéo lên
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	// Cấu hình nhóm ưu tiên ngắt
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	// Cấu hình ngắt cho TIM8
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_IRQn | TIM8_TRG_COM_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// Cấu hình timer TIM8
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
	
	// Cấu hình chế độ encoder cho TIM8
	TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM8, &TIM_ICInitStructure);
	
	// Xóa cờ ngắt và bật ngắt cho TIM8
	TIM_ClearFlag(TIM8, TIM_FLAG_Update);
	TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);
	TIM_ITConfig(TIM8, TIM_IT_Trigger, ENABLE);
	
	// Bật timer TIM8
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
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)//����ж�
	{    				   				     	    	
	}				   
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);   //����жϱ�־λ   
}

void TIM8_IRQHandler(void)
{ 		    		  			    
	if(TIM_GetITStatus(TIM8, TIM_IT_Update) == SET)//����ж�
	{    				   				     	    	
	}				   
	TIM_ClearITPendingBit(TIM8, TIM_IT_Update);   //����жϱ�־λ	
}
