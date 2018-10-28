#include<reg52.h>
#include "pit.h"
#include "read voltage value.h"
#include "uart.h"
#include "hx711.h"
#include "key.h"
#define Timer0_Reload_s			((MAIN_Fosc / 1000) * D_TIMER0 / 12000)
#if (Timer0_Reload_s < 65536)
	#define	Timer0_Reload		(65536 - Timer0_Reload_s)
#endif

//unsigned char num=0;
//unsigned char key_value=0;

extern unsigned int InitForceValue;				
extern unsigned int hx711date;
extern int f_val;	
/****************************************************************************** 
��ʱ����ʼ���ӳ��� (24M����)			
******************************************************************************/
void Timer0_Init(void)
{
  TMOD |= 0x01;	  //ʹ��ģʽ1��16λ��ʱ����ʹ��"|"���ſ�����ʹ�ö����ʱ��ʱ����Ӱ��		     
  TH0 = Timer0_Reload / 256;
  TL0 = Timer0_Reload % 256; 
}
//uchar Timer[10]={0,1,2,3,4,5,6,7,8,9};
//void time_event_cb(uchar num,uchar period,void (*period_event)())
//{
//  //static uchar count_time=0;
//  Timer[num]++;
//  if(Timer[num]==period) 
//  {
//  	period_event();
//	Timer[num]=0;
//  }
//}
/****************************************************************************** 
�û������¼�������			
******************************************************************************/
//int buff_hx[5] = {0};
//char is_send = 0;
//void event_1(void)
//{
//  num=ReadADC(0);
//  WriteDAC(num);
//  Uart_SendChar(num);
//}
//void event_2(void)
//{
//  static char count = 0;
//  hx711date = ReadCount();
//  f_val = hx711date	- InitForceValue;
//  if(is_send == 0)
//  {
//     buff_hx[count++] = f_val;
//  }
//  				
//  if(count == 5)
//  {
//    count = 0;
//	is_send = 1;
//  }
//}
//
//
//
//void event_3(void)
//{ 
//  //f_val = hx711date	- InitForceValue;
//  if(is_send)
//  {
//  	 send_hx711(buff_hx);
//	 is_send = 0;
//  }     
//}
//
//void event_4(void)
//{ 
//  get_key();
//}


//ע�ắ��
//void timer_event(void)
//{
  //time_event_cb(4,3,event_4);//��������
  //time_event_cb(1,1000,event_1);
  //time_event_cb(3,1,event_3);//����hx117��ֵ
  //time_event_cb(2,5,event_2);//��ֵ�ɼ�

//}

//ע�ắ��
void timer_event(void)
{
  static unsigned char count = 0;

  if(count == 0)
  {
  	 get_key();
  }
  if(count == 1)
  {
  	 send_hx711(f_val);  
  }	
  if(count++ == 1)	count = 0;
}
/****************************************************************************** 
��ʱ���ж��ӳ���			
******************************************************************************/
void Timer0_isr(void) interrupt 1 
{
	TR0 = 0;
	TH0 = Timer0_Reload / 256;
	TL0 = Timer0_Reload % 256; 
	TR0 = 1;	
	timer_event();
}

