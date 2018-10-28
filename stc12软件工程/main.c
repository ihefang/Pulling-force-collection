/****************************************************************************** 
  À­Á¦ÏµÍ³

  ÎÄ ¼þ Ãû   : main.c
  °æ ±¾ ºÅ   : v1.0  
  ±àÒëÈÕÆÚ   : 20180103
  ×÷    Õß   : hefang
  ¹¦ÄÜÃèÊö   : 24M¾§Õñ 
  
  ½Ó    ¿Ú   £º
               I2C£¨µçÑ¹£©£º
			   sbit SDA=P3^5;
               sbit SCL=P3^4;  
			   
			   OLED£º
			   sbit OLED_CS=P2^4; //Æ¬Ñ¡
               sbit OLED_RST =P2^2;//¸´Î»
               sbit OLED_DC =P2^3;//Êý¾Ý/ÃüÁî¿ØÖÆ
               sbit OLED_SCL=P2^0;//Ê±ÖÓ D0£¨SCLK£
               sbit OLED_SDIN=P2^1;//D1£¨MOSI£© Êý¾Ý

			   hx711:

			                      
-------------------------------------------------------------------------------
  ¸üÐÂÈÕÖ¾   :
  ÈÕ    ÆÚ   : 
  ×÷    Õß   : 
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
  OLED_Init();			//³õÊ¼»¯OLED 
  OLED_Clear();
  OLED_DrawBMP(0,0,128,8,BMP2);//ºÎ·½
  OLED_ShowCHinese(91 ,6,0);//¿Æ
  OLED_ShowCHinese(109,6,1);//¼¼
  delay_ms(1500);
/********************************************/
  OLED_Clear();
  OLED_ShowCHinese(0 ,0,2);//³õÊ¼»¯¶¨Ê±Æ÷
  OLED_ShowCHinese(18,0,3);
  OLED_ShowCHinese(36,0,4);
  OLED_ShowCHinese(54,0,7);
  OLED_ShowCHinese(72,0,8);
  OLED_ShowCHinese(90,0,9);	
/********************************************/
  Timer0_Init();
/********************************************/
  delay_ms(100);
  OLED_ShowCHinese(0 ,2,2);//³õÊ¼»¯´®¿Ú
  OLED_ShowCHinese(18,2,3);
  OLED_ShowCHinese(36,2,4);
  OLED_ShowCHinese(54,2,10);
  OLED_ShowCHinese(72,2,11);	
/********************************************/
  UART_Init();
/********************************************/
  delay_ms(100);
  OLED_ShowCHinese(0 ,4,12);//´ò¿ªÖÐ¶Ï
  OLED_ShowCHinese(18,4,13);
  OLED_ShowCHinese(36,4,14);
  OLED_ShowCHinese(54,4,15);
/********************************************/   
  TR0 = 1; //ÔÊÐí¶¨Ê±Æ÷0¹¤×÷
  TR1 = 1; //ÔÊÐí¶¨Ê±Æ÷1¹¤×÷
  ET0 = 1; //¶¨Ê±Æ÷0ÖÐ¶Ï´ò¿ª
  //ET1 = 1; //¶¨Ê±Æ÷1ÖÐ¶Ï´ò¿ª (¶¨Ê±Æ÷1ÓÃ×÷´®¿ÚÊ±¹Ø±ÕÖÐ¶Ï)
  EA  = 1; //×ÜÖÐ¶Ï´ò¿ª
/********************************************/
  delay_ms(100);
  OLED_ShowCHinese(0 ,6,16);//½øÈëÏµÍ³
  OLED_ShowCHinese(18,6,17);
  OLED_ShowCHinese(36,6,18);
  OLED_ShowCHinese(54,6,19);
  delay_ms(100);
  OLED_Clear();
}    
/****************************************************************************** 
			 						Ö÷³ÌÐò
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
  while (1)  //Ö÷Ñ­»·
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


