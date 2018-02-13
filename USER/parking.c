#include "stm32f10x.h"
#include "esp8266.h"
#include "usart.h"
#include "delay.h"
#include "PWM_OUT.h"
#include "I2COLED.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "bmp.h"
char cStr [100] = { 0 };//sprintf接收
char *p,*pCh;//wifi接收首地址，匹配首地址
char oledcar[50],oledinfo[50];//OLED车辆信息
u8 ucId;//信道
int carnum=0,carin=0,carout=0;//停车场车辆数，进出汽车数量
u8 link_status;//连接状态
/*客户端网页*/
char page[]="<html><body><a href=\"/ENTER\"><input type=\"Button\" value=\"ENTER\" style=\"position:absolute;top:50%;left:40%\"></input></a><a href=\"/PLAY\"><input type=\"Button\" value=\"PLAY\" style=\"position:absolute;top:50%;left:50%\"></input></a><a href=\"/LEAVE\"><input type=\"Button\" value=\"LEAVE\" style=\"position:absolute;top:50%;left:60%\"></input></a></body></html>";
/*标题显示*/
void printInfo(void)
{
    OLED_Fill(0x00);//清屏
    OLED_ShowStr(10,0,(unsigned char*)"Oliver-parking lot!",1);//显示标题
    OLED_ShowStr(20,2,(unsigned char*)"Welcome home!",1);//显示信息
}
int main(void)
{
    u8 i;
    DelayInit();//延时初始化
    TIM3_GPIO_Config();//TIM两个PWM通道GPIO初始化
    TIM3_PWM_Init();//PWM初始化
    USART1_Init();//PC端串口
    I2C_Configuration();//I2C初始化
    OLED_Init();//OLED初始化
       
    ESP8266_Init();//引脚配置，串口配置
    ESP8266_CH_ENABLE();//使能ESP8266
    ESP8266_AT_Test ();//测试是否正常
    ESP8266_Net_Mode_Choose ( AP );//选择AP模式，发出wifi信号，稳定
    ESP8266_Rst();//复位有效
    while(!ESP8266_CIPAP(esp8266_ip ));//IP正确与否
    while(!ESP8266_BuildAP(esp8266_user, esp8266_password, WPA_WPA2_PSK) );//设置ID，密码，信道，加密方式
    ESP8266_Enable_MultipleId ( ENABLE );//启动多连接，最多可连接5个客户端
    while(!ESP8266_StartOrShutServer(ENABLE, esp8266_port,esp8266_overTime));//设置端口号和超时时间
    ESP8266_Inquire_ApIp(cStr, 20 );//获取本机IP
    /*接收数据区和标志清零*/
	strEsp8266_Fram_Record.InfBit.FramLength = 0;
	strEsp8266_Fram_Record.InfBit.FramFinishFlag = 0;	
    ESP8266_UnvarnishSend();//开启透传模式
    sprintf(oledinfo,"       (^ ^)");
    while(1)
    {
        printInfo();//显示标题
        OLED_ShowStr(0,4,(unsigned char*)oledinfo,1);//显示车辆进出情况
        sprintf(oledcar,"Car number: %d",carnum);
        OLED_ShowStr(20,6,(unsigned char*)oledcar,1);//显示停车场情况
        link_status=ESP8266_Get_IdLinkStatus();//获取当下的连接情况
        for(i=0;i<5;i++)
        {
            if(((link_status>>i) & 0x01)!=0)
            {
                ESP8266_SendString ( DISABLE, page, strlen ( page ), ( ENUM_ID_NO_TypeDef ) i );//给每个客户端发送网页
            }
        }
        p=ESP8266_ReceiveString(DISABLE);//接收数据，p为接收到的数据的首地址
        if(strEsp8266_Fram_Record .InfBit .FramFinishFlag )//如果接收到了数据
		{
			USART_ITConfig (USART2, USART_IT_RXNE, DISABLE ); //禁用串口接收中断
            printf("接收到的数据: %s\r\n",strEsp8266_Fram_Record.Data_RX_BUF);
			if ( ((bool)strstr (strEsp8266_Fram_Record.Data_RX_BUF, "ENTER" ) ) != 0 ) //如果是进车信号
            {
                if(carnum==5)//车位已经满了
                {
                    sprintf(oledinfo,"Filled!Filled!Filled!");
                }
                else
                {
                    Gate1Open();//打开进车栏杆
                    carin++;//进车数加1
                    carnum++;//车位减1
                    sprintf(oledinfo,"  %d cars has entered!",carin);
                    
                } 
            }
            else if ( ((bool)strstr (strEsp8266_Fram_Record.Data_RX_BUF, "LEAVE" ) ) != 0 ) //如果是出车信号
            {
                if(carnum==0)//没车，就不能出车
                {
                    sprintf(oledinfo,"  Are you kidding?");
                }
                else
                {
                    
                    carnum--;//车位加1
                    Gate2Open();//打开出车栏杆
                    carout++;//出车数加1
                    sprintf(oledinfo,"  %d cars has leaved!",carout);
                }
            }
            else if( ((bool)strstr (strEsp8266_Fram_Record.Data_RX_BUF, "PLAY" ) ) != 0 )
            {
                OLED_DrawBMP(0,0,128,8,BMP2);
                DelayS(2);
            }
            if ( ( pCh = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+IPD," ) ) != 0 ) //发送反馈信息——本机IP
			{
				ucId = * ( pCh + strlen ( "+IPD," ) ) - '0';//多连接容易导致信道切换，所以需要实时获取信道
				ESP8266_SendString ( DISABLE, cStr, strlen ( cStr ), ( ENUM_ID_NO_TypeDef ) ucId );//发送数据
			}
            DelayS(3);//延时3s后关闭栏杆
            Gate1Close();
            Gate2Close();
        }
        strEsp8266_Fram_Record .InfBit .FramLength = 0;
	    strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;		
		USART_ITConfig (USART2, USART_IT_RXNE, ENABLE ); //使能串口接收中断
    }
}
