#ifndef __USART_H
#define __USART_H
#include "sys.h"	 
void usart1_send(u8 data);
void uart1_init(u32 pclk2,u32 bound);
void uart3_init(u32 pclk2,u32 bound);
void USART3_IRQHandler(void);
#endif	  
