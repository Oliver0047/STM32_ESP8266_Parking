#ifndef _PWM_OUT_H
#define _PWM_OUT_H
#include "stm32f10x.h"
void TIM3_GPIO_Config(void);
void TIM3_PWM_Init(void);
void Gate1Open(void);
void Gate1Close(void);
void Gate2Open(void);
void Gate2Close(void);
#endif
