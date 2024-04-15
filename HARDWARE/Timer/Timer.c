#include "Timer.h"

/**************************************************************************
Chức năng: Khởi tạo timer
Đặc điểm: Thiết lập giá trị arr là giá trị đếm, psc là tỉ lệ chia tần số
**************************************************************************/
void Timer_Init(u16 arr,u16 psc)  
{  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM6_IRQn;        //Cho phép ngắt từ bên ngoài
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;           //Cho phép kênh ngắt bên ngoài
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1; //Ưu tiên bộ 1
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;        //Ưu tiên con 3
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_TimeBaseInitStruct.TIM_Period = arr;               //Giá trị đếm
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc;            //Tỉ lệ chia tần số
	TIM_TimeBaseInitStruct.TIM_ClockDivision =0;           //Chia tỷ số clock
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//Chế độ đếm lên
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);               //Cho phép ngắt timer
	
	TIM_Cmd(TIM6,ENABLE);     //Bật timer 6
}  
