#ifndef _ESP8266_H
#define _ESP8266_H
#include "stm32f10x.h"
#include "common.h"
#include "delay.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

typedef enum{
  STA,
  AP,
  STA_AP  
} ENUM_Net_ModeTypeDef;


typedef enum{
	 enumTCP,
	 enumUDP,
} ENUM_NetPro_TypeDef;
	

typedef enum{
	Multiple_ID_0 = 0,
	Multiple_ID_1 = 1,
	Multiple_ID_2 = 2,
	Multiple_ID_3 = 3,
	Multiple_ID_4 = 4,
	Single_ID_0 = 5,
} ENUM_ID_NO_TypeDef;
	

typedef enum{
	OPEN = 0,
	WEP = 1,
	WPA_PSK = 2,
	WPA2_PSK = 3,
	WPA_WPA2_PSK = 4,
} ENUM_AP_PsdMode_TypeDef;


#define esp8266_ip "192.168.4.1"
#define esp8266_user "Oliver_private_parking_lot"
#define esp8266_password "123456789"
#define esp8266_port "80" 
#define esp8266_overTime "1800"  
/******************************* ESP8266 外部全局变量声明 ***************************/
#define RX_BUF_MAX_LEN  1024   //最大接收缓存字节数

extern struct  STRUCT_USARTx_Fram   //串口数据帧的处理结构体
{
    char  Data_RX_BUF [ RX_BUF_MAX_LEN ];
    union {
        __IO u16 InfAll;
        struct {
              __IO u16 FramLength       :15; // 14:0 
              __IO u16 FramFinishFlag   :1;  // 15 
        } InfBit;
    }; 	
} strEsp8266_Fram_Record;

#define ESP8266_Usart( fmt, ... ) USART_printf(USART2, fmt, ##__VA_ARGS__ ) 
#define PC_Usart( fmt, ... )   printf ( fmt, ##__VA_ARGS__ )  

#define ESP8266_CH_ENABLE()  GPIO_SetBits(GPIOA,GPIO_Pin_0)
#define ESP8266_CH_DISABLE() GPIO_ResetBits(GPIOA,GPIO_Pin_0)

#define ESP8266_RST_HIGH_LEVEL() GPIO_SetBits(GPIOA,GPIO_Pin_1)
#define ESP8266_RST_LOW_LEVEL() GPIO_ResetBits(GPIOA,GPIO_Pin_1)

void esp_GPIO_Config(void);
void esp_USART_Config(void);
void esp_NVIC_Config(void);
void ESP8266_Init(void);
void ESP8266_Rst ( void );
bool ESP8266_Cmd ( char * cmd, char * reply1, char * reply2, u32 waittime );
void ESP8266_AT_Test ( void );
bool ESP8266_Net_Mode_Choose ( ENUM_Net_ModeTypeDef enumMode );
bool ESP8266_JoinAP ( char * pSSID, char * pPassWord );
bool ESP8266_BuildAP ( char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode );
bool ESP8266_Enable_MultipleId ( FunctionalState enumEnUnvarnishTx );
bool ESP8266_Link_Server ( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id);
bool ESP8266_StartOrShutServer ( FunctionalState enumMode, char * pPortNum, char * pTimeOver );
uint8_t ESP8266_Get_LinkStatus ( void );
uint8_t ESP8266_Get_IdLinkStatus ( void );
uint8_t ESP8266_Inquire_ApIp ( char * pApIp, uint8_t ucArrayLength );
bool ESP8266_UnvarnishSend ( void );
void ESP8266_ExitUnvarnishSend ( void );
bool ESP8266_SendString ( FunctionalState enumEnUnvarnishTx, char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId );
char * ESP8266_ReceiveString ( FunctionalState enumEnUnvarnishTx );
uint8_t ESP8266_CWLIF ( char * pStaIp );
uint8_t ESP8266_CIPAP ( char * pApIp );

#endif
