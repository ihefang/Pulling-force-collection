#include<reg52.h>
#include "uart.h"

#define SEND_DATA_MOD	1
/****************************************************************************** 
���㲨����		
******************************************************************************/
#define T1_TimerReload		(256 - MAIN_Fosc / 192 / Baudrate0)		//Calculate the timer1 reload value	at 12T mode

void UART_Init(void)
{
	SCON = 0x50; //���ڹ�����ʽΪ1 �� �����������
	TMOD |= 0x20; //��ʱ��1 �����ڷ�ʽ2
	PCON |= 0x80; //SMOD = 1; �����ʼӱ�
	TH1 = T1_TimerReload; //������ 9600bps fosc=24MHz
	ES = 1; //�������ж�	
}
/****************************************************************************** 
���ڷ���һ���ַ���			
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
���ڷ���һ���ֽ�			
******************************************************************************/
void Uart_SendChar(unsigned char dat)
{
	SBUF = dat; //�����͵�����д�뻺����
	while(!TI); //�ȴ��������
	TI = 0; //���㷢�ͱ�־λ
}
   
/****************************************************************************** 
���ڽ����жϺ���			
******************************************************************************/
extern char update;
void INT_UartRcv(void) interrupt 4
{
	unsigned char Rcv = 0;
	if(RI) //��ѯ���ձ�־λ�������ݷ��͹���ʱ��Ϊ1��
	{
		RI = 0;		 //���ձ�־λ����
		Rcv = SBUF; //�洢������������
		//�ж��Ƿ�Ϊ��������Ϣ
		//if(0x7F == Rcv)	update=1;
		//IAP_CONTR = IAP_ENABLE;//����IAP, Ҳ����ֱ�ӽ������س���
		//Uart_SendChar(Rcv); //�ôӼ�����ϴ�����Ƭ�������� �� ���صļ������ʾ
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
//		Uart_SendChar('H');  //����֡ͷ hx711 ����
//	    Uart_SendChar('X');  //����֡ͷ hx711 ����
//	    if(w < 0)
//		{
//		  w = -w;
//		  Uart_SendChar((char)45);  //���͸��� (0x2D)
//		}
//		else
//		{
//		  Uart_SendChar((char)43);  //�������� (0x2B)
//		}
//		Uart_SendChar((char)((w&0xff00)>>8));  //���͸�8λ
//	    Uart_SendChar((char)(w&0xff));
//	}
//

//#if SEND_DATA_MOD
//	Uart_SendChar((char)((w&0xff00)>>8));  //���͸�8λ
//    Uart_SendChar((char)(w&0xff));		   //���͵�8λ
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
	Uart_SendChar('H');  //����֡ͷ hx711 ����
	Uart_SendChar('X');  //����֡ͷ hx711 ����
	if(num < 0)
	{
	   num = -num;
	   Uart_SendChar((char)45);  //���͸��� (0x2D)
	}
	else
	{
	   Uart_SendChar((char)43);  //�������� (0x2B)
	}
	Uart_SendChar((char)((num&0xff00)>>8));  //���͸�8λ
	Uart_SendChar((char)(num&0xff)); 
}
