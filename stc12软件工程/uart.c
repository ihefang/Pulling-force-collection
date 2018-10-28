#include<reg52.h>
#include "uart.h"

#define SEND_DATA_MOD	1
/****************************************************************************** 
计算波特率		
******************************************************************************/
#define T1_TimerReload		(256 - MAIN_Fosc / 192 / Baudrate0)		//Calculate the timer1 reload value	at 12T mode

void UART_Init(void)
{
	SCON = 0x50; //串口工作方式为1 ， 串行允许接收
	TMOD |= 0x20; //定时器1 工作在方式2
	PCON |= 0x80; //SMOD = 1; 波特率加倍
	TH1 = T1_TimerReload; //波特率 9600bps fosc=24MHz
	ES = 1; //开串口中断	
}
/****************************************************************************** 
串口发送一个字符串			
******************************************************************************/
/*
void Uart_SendString(unsigned char dat)
{	 
  unsigned  char i=0; 
	   
  //while(trdata[i]!=0x00)
  //{
		  //SBUF=trdata[i];
	      //while(!TI);
	      //TI=0;
	      //i++;
  //}
} */

/****************************************************************************** 
串口发送一个字节			
******************************************************************************/
void Uart_SendChar(unsigned char dat)
{
	SBUF = dat; //待发送的数据写入缓冲区
	while(!TI); //等待发送完成
	TI = 0; //清零发送标志位
}
   
/****************************************************************************** 
串口接收中断函数			
******************************************************************************/
extern char update;
void INT_UartRcv(void) interrupt 4
{
	unsigned char Rcv = 0;
	if(RI) //查询接收标志位（有数据发送过来时置为1）
	{
		RI = 0;		 //接收标志位清零
		Rcv = SBUF; //存储缓冲区的数据
		//判断是否为下载流信息
		//if(0x7F == Rcv)	update=1;
		//IAP_CONTR = IAP_ENABLE;//允许IAP, 也就是直接进入下载程序
		//Uart_SendChar(Rcv); //让从计算机上传到单片机的数据 ， 传回的计算机显示
	}
}

//void send_hx711(int *p)//(0x)48 58 2D 1A 0A 
//{	
//	//unsigned int a,b,c,d;				 // H  X  -  6666 
//	int w;
//	char i = 0;
//
//    for(i = 0;i < 1;i++)
//	{	
//	    w = *p++;
//		Uart_SendChar('H');  //发送帧头 hx711 数据
//	    Uart_SendChar('X');  //发送帧头 hx711 数据
//	    if(w < 0)
//		{
//		  w = -w;
//		  Uart_SendChar((char)45);  //发送负号 (0x2D)
//		}
//		else
//		{
//		  Uart_SendChar((char)43);  //发送正号 (0x2B)
//		}
//		Uart_SendChar((char)((w&0xff00)>>8));  //发送高8位
//	    Uart_SendChar((char)(w&0xff));
//	}
//

//#if SEND_DATA_MOD
//	Uart_SendChar((char)((w&0xff00)>>8));  //发送高8位
//    Uart_SendChar((char)(w&0xff));		   //发送低8位
//#else	
//    
//    a=w%10000/1000;
//    b=w%1000/100;
//    c=w%100/10;
//    d=w%10;
//    Uart_SendChar((char)(a+48));
//    Uart_SendChar((char)(b+48));
//    Uart_SendChar((char)(c+48));    
//    Uart_SendChar((char)(d+48)); 
//#endif  
//}


void send_hx711(int num)//(0x)48 58 2D 1A 0A 
{		
	Uart_SendChar('H');  //发送帧头 hx711 数据
	Uart_SendChar('X');  //发送帧头 hx711 数据
	if(num < 0)
	{
	   num = -num;
	   Uart_SendChar((char)45);  //发送负号 (0x2D)
	}
	else
	{
	   Uart_SendChar((char)43);  //发送正号 (0x2B)
	}
	Uart_SendChar((char)((num&0xff00)>>8));  //发送高8位
	Uart_SendChar((char)(num&0xff)); 
}
