#ifndef __LED_H
#define __LED_H
#include "sys.h"
//LED �˿ڶ���
#define LED PCout(2) // PA4
void LED_Init(void);  //��ʼ��
void Led_Flash(u16 time);
#endif

