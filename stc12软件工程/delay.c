#include "delay.h"
#include <reg52.h>              
/****************************************************************************** 
 uS延时函数，含有输入参数 unsigned char t，无返回值
 unsigned char 是定义无符号字符变量，其值的范围是
 0~255 这里使用晶振12M，精确延时请使用汇编,大致延时
 长度如下 T=tx2+5 uS 			 						
******************************************************************************
void DelayUs(unsigned char t)	//改为24M
{   
 while(--t);
}
/****************************************************************************** 
 mS延时函数，含有输入参数 unsigned char t，无返回值
 unsigned char 是定义无符号字符变量，其值的范围是
 0~255 这里使用晶振12M，精确延时请使用汇编				
******************************************************************************
void DelayMs(unsigned int t)	 	//改为24M
{ 
	 while(t--)
	 {
	     //大致延时1mS
		 DelayUs(220);
		 DelayUs(220);
	 }
}
/***********************************************************/ 
void  delay_ms(unsigned int ms)
{
	unsigned int i;
	do
	{
		i = MAIN_Fosc / 14000L;	//1T
		while(--i)	;   //8T per loop at 12T or 6T mode, 14T at 1T mode
     }while(--ms);
}
