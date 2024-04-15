#include "show.h"
  
  /**************************************************************************
   * Chức năng: Hiển thị trên OLED
   * Đầu vào: Không có
   * Đầu ra: Không có
  **************************************************************************/
  void oled_show(void)
  { 
	  OLED_ShowString(00, 0, "Flag_Stop");
	  OLED_ShowNumber(95, 0, Flag_Stop,1,12);
		  OLED_ShowString(00,10,"ADC"),
		  OLED_ShowNumber(30,10,Angle_Balance,4,12);
	  
  
		  //=============Hiển thị encoder bên trái=======================// 
								OLED_ShowString(00,20,"EncoLEFT");
		  if( Encoder_Left<0)	  OLED_ShowString(80,20,"-"),
								OLED_ShowNumber(95,20,-Encoder_Left,3,12);
		  else					  OLED_ShowString(80,20,"+"),
								OLED_ShowNumber(95,20, Encoder_Left,3,12);
	  //=============Hiển thị encoder bên phải=======================//		  
								OLED_ShowString(00,30,"EncoRIGHT");
		  if(Encoder_Right<0)	  OLED_ShowString(80,30,"-"),
								OLED_ShowNumber(95,30,-Encoder_Right,3,12);
		  else					  OLED_ShowString(80,30,"+"),
								OLED_ShowNumber(95,30,Encoder_Right,3,12);		  
  
  
								OLED_ShowString(00,40,"VOLTAGE");
								OLED_ShowString(68,40,".");
								OLED_ShowString(90,40,"V");
								OLED_ShowNumber(55,40,Voltage/100,2,12);
								OLED_ShowNumber(78,40,Voltage%100,2,12);
		   if(Voltage%100<10)	  OLED_ShowNumber(72,40,0,2,12);
							  
													  OLED_ShowString(00,50,"Motor");
													  if(Flag_Stop==0)	  OLED_ShowString(50,50,"O-N");
													  if(Flag_Stop==1)	  OLED_ShowString(50,50,"OFF");
		  
  
		  //=============Refresh=======================//
		  OLED_Refresh_Gram();	  
	  }
  
  /**************************************************************************
   * Chức năng: Hiển thị trên ứng dụng di động
   * Đầu vào: Không có
   * Đầu ra: Không có
  **************************************************************************/
  void APP_Show(void)
  {    
		static u8 flag;
		int app_2,app_3,app_4;
		  app_4=(Voltage-740)*2/3;		  if(app_4<0)app_4=0;if(app_4>100)app_4=100;   //Tính toán dòng từ điện áp
		  app_3=Encoder_Right*1.1; if(app_3<0)app_3=-app_3; 							 //Dòng từ encoder được chuyển đổi sang độ
		  app_2=Encoder_Left*1.1;  if(app_2<0)app_2=-app_2;
		flag=!flag;
	  if(PID_Send==1)//Nếu có yêu cầu gửi PID
	  {
		  printf("{C%d:%d:%d:%d:%d:%d:%d:%d:%d}$",(int)(Balance_KP),(int)(Balance_KD),(int)(Velocity_Kp),(int)(Velocity_Ki),0,0,0,0,0);//Gửi tới ứng dụng
		  PID_Send=0; 
	  }   
	 else if(flag==0)// 
	 printf("{A%d:%d:%d:%d}$",(u8)app_2,(u8)app_3,app_4,(int)((Angle_Balance-ZHONGZHI)/11.3)); //Gửi tới ứng dụng
	   else
	   printf("{B%d:%d:%d:%d}$",(int)Angle_Balance,Voltage,Encoder_Left,Encoder_Right);//Gửi tới ứng dụng  để hiển thị
	   
  
  }
  
  /**************************************************************************
   * Chức năng: Gửi dữ liệu để vẽ biểu đồ trên máy tính
   * Đầu vào: Không có
   * Đầu ra: Không có
  **************************************************************************/
  void DataScope(void)
  {   
	  Vol=(float)Voltage/100;
		  DataScope_Get_Channel_Data( Angle_Balance, 1 );		//Lấy dữ liệu từ góc cân bằng
		  DataScope_Get_Channel_Data( Encoder_Left, 2 );		 //Lấy dữ liệu từ encoder bên trái 
		  DataScope_Get_Channel_Data( Encoder_Right, 3 );		 //Lấy dữ liệu từ encoder bên phải
	  
  //	  USART_SendData(USART1, 'a');
  //	  delay_ms(500);
  //	  DataScope_Get_Channel_Data( 0 , 4 );	 
  //	  DataScope_Get_Channel_Data(0, 5 ); //Thay thế bằng dữ liệu cần hiển thị
  //	  DataScope_Get_Channel_Data(0 , 6 );//Thay thế bằng dữ liệu cần hiển thị
  //	  DataScope_Get_Channel_Data(0, 7 );
  //	  DataScope_Get_Channel_Data( 0, 8 ); 
  //	  DataScope_Get_Channel_Data(0, 9 );  
  //	  DataScope_Get_Channel_Data( 0 , 10);
		  Send_Count = DataScope_Data_Generate(3);
		  for( i = 0 ; i < Send_Count; i++) 
		  {
		  while((USART1->SR&0X40)==0);	
		  USART1->DR = DataScope_OutPut_Buffer[i]; 
		  }
		  delay_ms(50);
  }
