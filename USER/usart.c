#include "usart.h"
#include "misc.h"
#include "stdio.h"
unsigned char x=0,y=0;
void USART_Config(unsigned int bound)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure. GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure. GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate=bound;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USART_Init(USART1,&USART_InitStructure);
	//USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
}
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void USART1_Init(void)
{
    USART_Config(115200);
    //NVIC_Configuration();
}
int fputc(int ch,FILE *f)
{
	USART_SendData(USART1, (unsigned char)ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)!=SET);
	return (ch);
}

//void USART1_IRQHandler(void)
//{
//	unsigned char code;
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
//	{ 	
//			code=USART_ReceiveData(USART1);
//			if(code>>6==0)
//			{
//				if((code&(0x3f))<60){
//					x=code&(0x3f);
//				}
//			}
//			else if(code>>6==3)
//			{
//				if((code&(0x3f))<50)
//				{
//					y=code&(0x3f);
//				}
//			}
//	} 
//}
