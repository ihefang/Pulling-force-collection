#include <reg52.h> 
#include "oled_display.h"

#include "oled.h"

extern char key_line;
extern char key_page;
extern char key_sure;


void display(void)
{
  refresh_oled();
  display_flag();  
  lcd_user_write();
}

void display_flag(void)
{
  static char perline=0;
  static char cleline=3;
  static char nowline=0;
  static unsigned char count=0;
  if(++count>20) count=0;
  nowline=key_line*2;
  OLED_ShowString(0,cleline," ");
  if(key_sure<0 && count>10) OLED_ShowString(0,nowline," ");
  else OLED_ShowString(0,nowline,">"); 
  
  if(perline!=nowline) cleline=perline;
  perline=nowline;  
}

void refresh_oled(void)
{ 
  static char per_page=0;
  static char per_sure=0;
  if(key_page!=per_page) OLED_Clear();
  per_page=key_page;
  if(key_page!=per_sure) OLED_Clear();
  per_sure=key_page;
  //if(Key_State.KEY_STATE==2 && Key_State.KEY_NUM==2) OLED_init();	//
}
extern unsigned int  hx711date;
extern unsigned int InitForceValue;
extern unsigned int f_val;


extern char tiaoling;

void lcd_user_write(void)
{   int i = 0;
    if(key_page==0)
	{
	  OLED_ShowCHinese(10,0,20);//tiaoling
	  OLED_ShowCHinese(28,0,21);
	  display_num(70,0,tiaoling);
	   
	  OLED_ShowCHinese(10,2,22);//³õ(22) Öµ(23)
	  OLED_ShowCHinese(28,2,23);
	  display_num(70,2,InitForceValue);	//32213
	    
	  OLED_ShowCHinese(10,4,24);//li(24) Öµ(23)
	  OLED_ShowCHinese(28,4,23);
	  display_num(70,4, f_val); 
	}


    if(key_page==1)
	{
	  OLED_ShowCHinese(10,2,20);//tiaoling
	  OLED_ShowCHinese(28,2,21);
	}
}
