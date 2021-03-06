/****************************************************************************** 
  拉力系统

  文 件 名   : main.c
  版 本 号   : v1.0  
  编译日期   : 20180103
  作    者   : hefang
  功能描述   : 24M晶振 
  
  接    口   ：
               I2C（电压）：
			   sbit SDA=P3^5;
               sbit SCL=P3^4;  
			   
			   OLED：
			   sbit OLED_CS=P2^4; //片选
               sbit OLED_RST =P2^2;//复位
               sbit OLED_DC =P2^3;//数据/命令控制
               sbit OLED_SCL=P2^0;//时钟 D0（SCLK�
               sbit OLED_SDIN=P2^1;//D1（MOSI） 数据

			   hx711:

			                      
-------------------------------------------------------------------------------
  更新日志   :
  日    期   : 
  作    者   : 
******************************************************************************/
#include <reg52.h>                
#include "read voltage value.h"
#include "i2c.h"
#include "delay.h"
#include "pit.h"
#include "uart.h"
#include "hx711.h"
#include "oled.h"
#include "bmp.h"	
#include "oled_display.h"
#define IAP_update() IAP_CONTR = IAP_ENABLE 

void os_init(void)
{
/********************************************/
  OLED_Init();			//初始化OLED 
  OLED_Clear();
  OLED_DrawBMP(0,0,128,8,BMP2);//何方
  OLED_ShowCHinese(91 ,6,0);//科
  OLED_ShowCHinese(109,6,1);//技
  delay_ms(1500);
/********************************************/
  OLED_Clear();
  OLED_ShowCHinese(0 ,0,2);//初始化定时器
  OLED_ShowCHinese(18,0,3);
  OLED_ShowCHinese(36,0,4);
  OLED_ShowCHinese(54,0,7);
  OLED_ShowCHinese(72,0,8);
  OLED_ShowCHinese(90,0,9);	
/********************************************/
  Timer0_Init();
/********************************************/
  delay_ms(100);
  OLED_ShowCHinese(0 ,2,2);//初始化串口
  OLED_ShowCHinese(18,2,3);
  OLED_ShowCHinese(36,2,4);
  OLED_ShowCHinese(54,2,10);
  OLED_ShowCHinese(72,2,11);	
/********************************************/
  UART_Init();
/********************************************/
  delay_ms(100);
  OLED_ShowCHinese(0 ,4,12);//打开中断
  OLED_ShowCHinese(18,4,13);
  OLED_ShowCHinese(36,4,14);
  OLED_ShowCHinese(54,4,15);
/********************************************/   
  TR0 = 1; //允许定时器0工作
  TR1 = 1; //允许定时器1工作
  ET0 = 1; //定时器0中断打开
  //ET1 = 1; //定时器1中断打开 (定时器1用作串口时关闭中断)
  EA  = 1; //总中断打开
/********************************************/
  delay_ms(100);
  OLED_ShowCHinese(0 ,6,16);//进入系统
  OLED_ShowCHinese(18,6,17);
  OLED_ShowCHinese(36,6,18);
  OLED_ShowCHinese(54,6,19);
  delay_ms(100);
  OLED_Clear();
}    
/****************************************************************************** 
			 						主程序
******************************************************************************/
unsigned int hx711date;
unsigned int InitForceValue;
int f_val;
extern char tiaoling;
char update=0;
extern int buff_hx[5];


main()
{
  os_init();
  delay_ms(200);
  InitForceValue=hx711date;
  delay_ms(200);
  InitForceValue=hx711date;
   
/********************************************/
  while (1)  //主循环
  {	 
    
	//InitForceValue=ReadCount();

	if(tiaoling==1)
	{
	  EA  = 0;
	  InitForceValue=ReadCount();
	  EA  = 1;
	  tiaoling=0;	
	}
	if(update==1) 
	{
	  IAP_update();
	}
    EA  = 0;
  	hx711date = ReadCount();
	EA  = 1;
	f_val = hx711date - InitForceValue;
	buff_hx[0] = f_val;
	
	display();	
  }
}


