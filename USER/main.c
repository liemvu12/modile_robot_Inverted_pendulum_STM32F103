#include "stm32f10x.h"
#include "sys.h"
u8 Flag_Way=0,Flag_Show=0,Flag_Stop=1;                 
int Encoder_Left,Encoder_Right;             
float Velocity,Velocity_Set,Turn,Angle_Set;
u8 Flag_Qian,Flag_Hou,Flag_Left,Flag_Right,Flag_Velocity=2;
int Motor_A,Motor_B,Target_A,Target_B;       
int Voltage;                                
int temp_add;              
u8 Urxbuf[4],rxbuf[4],Usart_Flag,PID_Send;
int Angle_Balance;
float Balance_KP=72,Balance_KD=300,Velocity_Kp=200,Velocity_Ki=4;  
u8 delay_50,delay_flag;
int main(void)
{
	Stm32_Clock_Init(9);	// Khởi tạo đồng hồ hệ thống    
	JTAG_Set(JTAG_SWD_DISABLE);	// Ngắt chế độ Debug JTAG_SWD
	JTAG_Set(SWD_ENABLE);	// Khởi tạo chế độ Debug SWD           
	delay_init();	// Khởi tạo bộ Delay                 
	delay_ms(1000);	// Delay 1s
	delay_ms(1000);	// Delay 1s
	LED_Init(); 	// Khởi tạo LED chân GPIOC, Pin2, Push-pull, tốc độ 50MHz
	KEY_Init(); 	// Khởi tạo nút nhấn KEY chân GPIOA, Pin0, Pull-up
	OLED_Init(); 	// Khởi tạo màn hình OLED
	Encoder_Init_TIM4(); // Khởi tạo Timer 4 chế độ encoder cho bánh trái
	Encoder_Init_TIM8(); // Khởi tạo Timer 8 chế độ encoder cho bánh phải
 	uart3_init(72,9600);  // Khởi tạo ngoại vi Uart3, tần số 72MHz, baudRate =9600
	delay_ms(300); // Delay 300ms
	uart1_init(72, 128000);	// Khởi tạo ngoại vi Uart1, tần số 72Mhz, baudRate = 128000
	Motor_PWM_Init(7199, 0); // khởi tạo chế độ PWM điều khiển 2 động cơ
	Adc_Init_(); 	// Khởi tạo ADC đo điện áp          
	Angle_Adc_Init();	// Khởi tạo ADC đo góc	             
	Timer_Init(99,7199); 	// Khởi tạo timer 6, ngắt timer 6 mức ưu tiên 1          
  while(1)
	{
		if(Flag_Show==0)         
		{
				APP_Show();		// Hiện thị thông số
				oled_show();	// Hiển thị thông số
		}
		else                      
		{
				DataScope();          
		}
		delay_flag=1;	
		delay_50=0;
		while(delay_flag);	     
		
	}
}



