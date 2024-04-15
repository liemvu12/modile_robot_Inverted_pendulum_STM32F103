#include "control.h"	
  
  /**************************************************************************
  Chức năng: Xử lý ngắt từ TIM6
  Đặc điểm: Mỗi 5ms, kiểm tra cảm biến MPU6050 qua chân INT.
			Đảm bảo việc lấy dữ liệu từ cảm biến được đồng bộ với thời gian.
  **************************************************************************/
  void TIM6_IRQHandler(void)  
  {    
	  if(TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)//Kiểm tra ngắt từ timer
	  {   
			TIM_ClearITPendingBit(TIM6, TIM_IT_Update); 		   //Xóa cờ ngắt
		  
			if(delay_flag==1)
			   {
				   if(++delay_50==10)  delay_50=0,delay_flag=0; 					//Đếm và tạo 50ms delay
			   }
		   
			if(++Flag_Target==4)  
			   {
		  Encoder_Left=Read_Encoder(4); 									  //Đọc giá trị từ encoder trái						   
			  Encoder_Right=-Read_Encoder(8);									   //Đọc giá trị từ encoder phải
				Position_Pwm=velocity(Encoder_Left,Encoder_Right);		// Tính toán giá trị PWM dựa vào vận tốc
				  Flag_Target=0;
			   }
				  
			Trun_Pwm=turn(); 
  
		Angle_Balance=Get_Adc_Average(5,15);					 //Đọc giá trị cảm biến góc	  
		Balance_Pwm =balance(Angle_Balance);										  //Điều khiển PD theo góc  
  
		Motor_A=Balance_Pwm+Position_Pwm+Trun_Pwm;		  //Tính PWM cho motor A
			  Motor_B=Balance_Pwm+Position_Pwm-Trun_Pwm;		//Tính PWM cho motor B
			Xianfu_Pwm();						  //Giới hạn PWM không vượt quá 100%
			  if(Turn_Off(Voltage)==0)		   
			  	Set_Pwm(Motor_A,Motor_B);   //Thiết lập PWM cho motor  
			  Key();									   //Kiểm tra trạng thái nút nhấn để thay đổi chế độ
			  Voltage_Temp=Get_battery_volt();										   //Đọc điện áp pin 	  
			  Voltage_Count++;													   //Tính trung bình
			  Voltage_All+=Voltage_Temp;										   //Tổng hợp
			  if(Voltage_Count==100) Voltage=Voltage_All/100,Voltage_All=0,Voltage_Count=0;//Tính trung bình	  
			  Led_Flash(100);							   //Bật tắt đèn LED
	  } 	   
  }
  
  /**************************************************************************
  Chức năng: Thiết lập PWM cho motor A và motor B
  Đặc điểm: Đầu vào là giá trị PWM cho motor A và motor B
  **************************************************************************/
  void Set_Pwm(int motor_a,int motor_b)
  {
		  if(motor_a<0) 		  PWMA2=7200,PWMA1=7200+motor_a;
			  else							  PWMA1=7200,PWMA2=7200-motor_a;
							  
		  
			if(motor_b<0)		  PWMB1=7200,PWMB2=7200+motor_b;
			  else				  PWMB2=7200,PWMB1=7200-motor_b;
  }
  
  /**************************************************************************
  Chức năng: Giới hạn giá trị PWM trong phạm vi cho phép
  Đặc điểm: Không cho phép PWM vượt quá giá trị cực đại và cực tiểu
  **************************************************************************/
  void Xianfu_Pwm(void)
  {   
		int Amplitude=7200;    //PWM tối đa 7200
	  if(Motor_A<-Amplitude) Motor_A=-Amplitude;  
		  if(Motor_A>Amplitude)  Motor_A=Amplitude;   
		if(Motor_B<-Amplitude) Motor_B=-Amplitude;	  
		  if(Motor_B>Amplitude)  Motor_B=Amplitude; 	  
  }
  
  /**************************************************************************
  Chức năng: Kiểm tra và xử lý nút nhấn
  Đặc điểm: Thay đổi trạng thái dừng/hoạt động khi nhấn nút
  **************************************************************************/
  void Key(void)
  {   
	  u8 tmp, tmp1;
	  tmp=click_N_Double(50);
	  tmp1 = click();
	  if(tmp1==1)Flag_Stop=!Flag_Stop;//Thay đổi trạng thái dừng hoạt động
	  if(tmp==2)Flag_Show=!Flag_Show;//Thay đổi trạng thái hiển thị giao diện
  }
  
  /**************************************************************************
  Chức năng: Kiểm tra và xử lý trường hợp xảy ra sự cố
  Đặc điểm: Kiểm tra điều kiện an toàn của hệ thống và tắt motor khi cần thiết
  **************************************************************************/
  u8 Turn_Off( int voltage)
  {
	  u8 temp;
	  if(Angle_Balance<(ZHONGZHI-500)||Angle_Balance>ZHONGZHI+500||voltage<1110||Flag_Stop==1)//Kiểm tra điều kiện an toàn
	  { 												  
		temp=1; 
		Motor_A=0;
		Motor_B=0;	  
		Flag_Stop=1;
			  PWMA1=0;
			  PWMA2=0;
			  PWMB1=0;
			  PWMB2=0;		  
	}
	  else
		temp=0;
		return temp;		  
  }
  
  /**************************************************************************
  Chức năng: Trả về giá trị tuyệt đối của một số nguyên
  Đặc điểm: Nhận vào một số nguyên và trả về giá trị tuyệt đối của nó
  **************************************************************************/
  int myabs(int a)
  { 		 
	  int temp;
	  if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
  }
  
  /**************************************************************************
  Chức năng: Điều khiển PD theo góc
  Đặc điểm: Nhận vào giá trị góc và tính toán PWM điều khiển
  **************************************************************************/
  int balance(float Angle)
  {  
	 float Bias;					   //Độ lệch góc
	   static float Last_Bias,D_Bias;	 //Biến cho PID
	   int balance; 					 //PWM điều khiển 
	   Bias=Angle-ZHONGZHI; 			 //Tính độ lệch góc
	   D_Bias=Bias-Last_Bias;			 //Tính độ lệch góc tương đối giữa các vòng lặp
	   balance=-Balance_KP*Bias-D_Bias*Balance_KD;	 //Tính toán PWM điều khiển theo PD
	 Last_Bias=Bias;				   //Lưu lại giá trị lần trước
	   return balance;
  }
  
  /**************************************************************************
  Chức năng: Điều khiển tốc độ dựa trên vận tốc
  Đặc điểm: Tính toán và điều chỉnh PWM theo vận tốc của các bánh xe
  **************************************************************************/
  int velocity(int encoder_left,int encoder_right)
  {  
	  static float Velocity,Encoder_Least,Encoder,Movement;
		static float Encoder_Integral,Target_Velocity=300;
		
		if(1==Flag_Qian)	  Movement=-Target_Velocity/Flag_Velocity;			   //Nếu đang lùi
		else if(1==Flag_Hou)  Movement=Target_Velocity/Flag_Velocity;		  //Nếu đang tiến
		else  Movement=0;														//Nếu không tiến không lùi
		
		Encoder_Least =(encoder_left+encoder_right)-0;					  //Lấy sự chênh lệch tốc độ
		  Encoder *= 0.7;													 //Lọc tín hiệu
		  Encoder += Encoder_Least*0.3; 									 //Lọc tín hiệu  
		  Encoder_Integral +=Encoder;										//Tính tổng tích phân
		  Encoder_Integral +=Movement;										 //Tính tổng tích phân
		  if(Encoder_Integral>10000)	  Encoder_Integral=10000;				//Giới hạn giá trị tổng tích phân
		  if(Encoder_Integral<-10000) Encoder_Integral=-10000;				 //Giới hạn giá trị tổng tích phân
		  Velocity=Encoder*Velocity_Kp/10+Encoder_Integral*Velocity_Ki/10;	 //Tính tốc độ
		  if(Turn_Off(Voltage)==1)			 Encoder_Integral=0;			  //Nếu xảy ra sự cố, reset tổng tích phân
		return Velocity;
  }
  
  /**************************************************************************
  Chức năng: Điều khiển quay
  Đặc điểm: Tính toán và điều chỉnh PWM theo góc quay
  **************************************************************************/
  int turn(void)
  {
	  static float Turn_Target,Turn,Turn_Convert=2,Bias_Intergral,kp=16,ki=1.6;
		float Turn_Amplitude=150/Flag_Velocity,Bias;	
  
		if(1==Flag_Left)  Turn_Target-=Turn_Convert;	//Nếu quay trái
		else if(1==Flag_Right)	  Turn_Target+=Turn_Convert;  //Nếu quay phải
		else Turn_Target=0; 								 //Nếu không quay
		
	  if(Turn_Target>Turn_Amplitude)  Turn_Target=Turn_Amplitude;	 //Giới hạn tốc độ quay
		if(Turn_Target<-Turn_Amplitude) Turn_Target=-Turn_Amplitude;   //Giới hạn tốc độ quay	   
	  Bias=Encoder_Left-Encoder_Right;
		Bias_Intergral+=Bias;
		if(Bias_Intergral>5000)   Bias_Intergral=5000;				 //Giới hạn tổng tích phân
		  if(Bias_Intergral<-5000)	  Bias_Intergral=-5000; 			//Giới hạn tổng tích phân 
		  if(Turn_Off(Voltage)==1)			 Bias_Intergral=0;			//Nếu xảy ra sự cố, reset tổng tích phân
	  
		if(Flag_Left==1||Flag_Right==1) Turn=Turn_Target*10,Bias_Intergral=0;
		else Turn=-Bias*kp-Bias_Intergral*ki+Turn_Target*10;   //Tính toán PWM theo PD
  
		return Turn;
  }
