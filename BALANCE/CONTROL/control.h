#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
#define PI 3.14159265
void Kinematic_Analysis(float velocity,float turn);
void TIM1_UP_IRQHandler(void) ;
void Set_Pwm(int motor_a,int motor_b);
void Key(void);
void Xianfu_Pwm(void);
u8 Turn_Off( int voltage);
void Get_Angle(u8 way);
int myabs(int a);
int balance(float angle);
int velocity(int encoder_left,int encoder_right);
int turn(void);//转向控制
void Usart_Control(void);
#endif
