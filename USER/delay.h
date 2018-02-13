#ifndef _DELAY_H
#define _DELAY_H
#include "stm32f10x.h"
void DelayInit(void);
void Delay(u32 nTime);
void DelayUs(u32 nTime);
void DelayS(u32 nTime);
#endif
