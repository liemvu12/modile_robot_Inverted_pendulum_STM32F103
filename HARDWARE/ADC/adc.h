#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"
#define Battery_Ch 11
  
void Adc_Init_(void);
u16 Get_Adc(u8 ch);
int Get_battery_volt(void);   
void  Angle_Adc_Init(void);
u16 Get_Adc_Average(u8 ch,u8 times);
#endif 
