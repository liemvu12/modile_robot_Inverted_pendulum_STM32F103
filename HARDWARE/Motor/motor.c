#include "motor.h"
  /**************************************************************************
	Hàm Motor_PWM_Init
	Khởi tạo PWM cho motor
  
	Tham số:
	  - arr: Giá trị tự định sẵn của thanh ghi Auto-Reload (ARR) của timer
	  - psc: Giá trị tự định sẵn của thanh ghi PreScaler (PSC) của timer
  
	Giá trị trả về: không
  
  **************************************************************************/
  void Motor_PWM_Init(u16 arr, u16 psc)
  { 	   
	  GPIO_InitTypeDef GPIO_InitStructure; // Khai báo cấu trúc để khởi tạo GPIO
	  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; // Khai báo cấu trúc để khởi tạo timer
	  TIM_OCInitTypeDef  TIM_OCInitStructure; // Khai báo cấu trúc để khởi tạo chế độ đầu ra của timer
  
	  // Bật clock cho TIM3
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	  // Bật clock cho GPIOB và GPIOA
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);
  
	  // Cấu hình GPIOB_PIN0 và GPIOB_PIN1 là đầu ra chế độ AF_PP
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
	  // Cấu hình GPIOA_PIN6 và GPIOA_PIN7 là đầu ra chế độ AF_PP
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	  // Cấu hình timer TIM3
	  TIM_TimeBaseStructure.TIM_Period = arr; // Set giá trị tự định sẵn của thanh ghi Auto-Reload (ARR)
	  TIM_TimeBaseStructure.TIM_Prescaler = psc; // Set giá trị tự định sẵn của thanh ghi PreScaler (PSC)
	  TIM_TimeBaseStructure.TIM_ClockDivision = 0; // Set chia tỷ lệ clock là 0
	  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // Set chế độ đếm là lên
	  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  
	  // Cấu hình chế độ PWM cho các kênh của TIM3
	  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // Set chế độ PWM là Mode 1
	  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // Set cực nghịch cho chế độ đầu ra
	  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // Bật chế độ đầu ra
	  // Cấu hình kênh 1
	  TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); // Bật chế độ preload cho kênh 1: Chân GPIOB_PIN0 (TIM3_CH3)
	  // Cấu hình kênh 2
	  TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); // Bật chế độ preload cho kênh 2: Chân GPIOB_PIN1 (TIM3_CH4)
	  // Cấu hình kênh 3
	  TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); // Bật chế độ preload cho kênh 3: Chân GPIOA_PIN6 (TIM3_CH1)
	  // Cấu hình kênh 4
	  TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable); // Bật chế độ preload cho kênh 4: Chân GPIOA_PIN7 (TIM3_CH2)
  
	  // Bật chế độ preload cho thanh ghi Auto-Reload (ARR)
	  TIM_ARRPreloadConfig(TIM3, ENABLE);
  
	  // Bật timer TIM3
	  TIM_Cmd(TIM3, ENABLE);
  }

void control(int moto, int pwm1, int pwm2)
{
    if(moto == 1)
    {
        // Set PWM duty cycle for motor 1
        TIM_SetCompare1(TIM3, pwm1); // Set duty cycle for TIM3 channel 1 (PA6)
        TIM_SetCompare2(TIM3, pwm2); // Set duty cycle for TIM3 channel 2 (PA7)
        // The duty cycle values are scaled by dividing by 7200
    }
    
    if(moto == 2)
    {
        // Set PWM duty cycle for motor 2
        TIM_SetCompare3(TIM3, pwm1); // Set duty cycle for TIM3 channel 3 (PB0)
        TIM_SetCompare4(TIM3, pwm2); // Set duty cycle for TIM3 channel 4 (PB1)
        // The duty cycle values are scaled by dividing by 7200
    }
}

/**************************************************************************
 * Function: Servo_PWM_Init
 * Description: Initialize PWM for servo motor control
 * Input:
 *     - arr: Auto-reload value
 *     - psc: Prescaler value
 * Output: None
**************************************************************************/
void Servo_PWM_Init(u16 arr, u16 psc)
{	 
    RCC->APB2ENR |= RCC_APB2Periph_TIM1;  // Enable TIM1 clock
    RCC->APB2ENR |= RCC_APB2Periph_GPIOA; // Enable GPIOA clock
	
    GPIOA->CRH &= 0xFFFF0FFF;             // Clear configuration for PA11
    GPIOA->CRH |= 0x0000B000;             // Configure PA11 as alternate function push-pull
	
    TIM1->ARR = arr;                      // Set auto-reload value
    TIM1->PSC = psc;                      // Set prescaler value
	
    TIM1->CCMR2 |= 6 << 12;               // Set CH4 PWM1 mode	
    TIM1->CCMR2 |= 1 << 11;               // Enable CH4 preload buffer	   
    TIM1->CCER |= 1 << 12;                // Enable CH4 output	   
    TIM1->BDTR |= 1 << 15;                // Enable TIM1 break and dead-time
    TIM1->CR1 = 0x80;                     // Enable TIM1 auto-reload preload
    TIM1->CR1 |= 0x01;                    // Start TIM1		
    TIM1->CCR4 = 750;                     // Set initial duty cycle for CH4
}

