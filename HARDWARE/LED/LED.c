#include "led.h" 
#include "stm32f10x_gpio.h" 

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; // Khai báo cấu trúc để khởi tạo GPIO

    // Bật clock cho GPIOC
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 

    // Thiết lập chân GPIOC_PIN2 là đầu ra, chế độ push-pull, tốc độ 50MHz
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure); // Áp dụng cấu hình vào chân GPIO
    // Đặt mức logic cao cho chân GPIOC_PIN2 (tắt LED ban đầu)
    GPIO_SetBits(GPIOC, GPIO_Pin_2); 
}

void Led_Flash(u16 time)
{
	  static int temp;
	  if(0==time) LED=0;
	  else		if(++temp==time)	LED=~LED,temp=0;
}
