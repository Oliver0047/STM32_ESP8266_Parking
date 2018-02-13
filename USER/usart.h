#ifndef _USART_H
#define _USART_H
#include "stdio.h"
void USART_Config(unsigned int bound);
void NVIC_Configuration(void);
void USART1_Init(void);
int fputc(int ch,FILE *f);
#endif
