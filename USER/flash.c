/************************************************************************************
*  Copyright (c), 2015, HelTec Automation Technology co.,LTD.
*            All rights reserved.
*
* File name: flash.c
* Project  : HelTec
* Processor: STM32F103C8T6
* Compiler : MDK for ARM - 4.72.1.0
* 
* Author : Aaron.Lee
* Version: 1.00
* Date   : 2014.4.20
* Email  : leehunter8801@gmail.com
* Modification: none
* 
* Description: 惠利特自动化自制字库芯片的驱动文件，仅适用于惠利特自动化(Heltec.taobao.com)所售字库版OLED显示屏
*
* Others: none;
*
* Function List:
*
* 1. void FF_Init(void) -- 字库芯片初始化
* 2. u8 Write_FF(u8 value) -- 向字库芯片写一个byte的数据(命令)
* 3. void Read_FLASH(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead) -- 读取字库芯片中的内容
*
* History: none;
*
*************************************************************************************/

#include "flash.h"
#include "delay.h"
#include "OLED.h"

void FF_CS(u8 value)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_13,(BitAction)value); //CS2
}
void FF_SO(u8 value)
{
	GPIO_WriteBit(GPIOC, GPIO_Pin_1,(BitAction)value); //SDI
}
void FF_CLK(u8 value)
{
	GPIO_WriteBit(GPIOC, GPIO_Pin_0,(BitAction)value); //CLK
}

void FF_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PC2 -- FSO(读取Flash芯片的状态)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	FF_CS(1);
	FF_CLK(1);
}

u8 Write_FF(u8 value)
{
	u8 i;
	u8 temp=0;
	FF_CLK(1);
	for(i=0;i<8;i++)
	{
	 FF_CLK(0);
	 DelayUs(2);
	 if((value&0x80)==0x80)
		 FF_SO(1);
	 else
		 FF_SO(0); 
	 value<<=1;
	 DelayUs(2);
	 FF_CLK(1);
	 DelayUs(2);
	 temp<<=1;
	 if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)) //GPIOC.2 为FSO脚
			temp++;
	}
	return(temp);
}

void Read_FLASH(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
	OLED_CS_Set();//取消OLED选中
	FF_CS(0);//选中FLASH芯片
	Write_FF(0x03);
	Write_FF((ReadAddr & 0xFF0000) >> 16);
	Write_FF((ReadAddr& 0xFF00) >> 8);
	Write_FF(ReadAddr & 0xFF);
	while(NumByteToRead--)
	{
		*pBuffer = Write_FF(0xA5);
		pBuffer++;
	}
	FF_CS(1);
	OLED_CS_Clr();
}
