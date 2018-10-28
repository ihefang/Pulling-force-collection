#include <reg52.h> 
#include "key.h"

#define key_page_max=2//��Ļҳ����

sbit k1=P0^4;
sbit k2=P0^5;
sbit k3=P0^6;

KEY_NUM_STATE KEY_N_S_NOW;
KEY_NUM_STATE KEY_N_S_PER;
KEY_NUM_STATE Key_State;
unsigned char push_time=15;



unsigned char key_check(void)
{
	if(k1==0) return 1;
	else if(k2==0) return 2;
	else if(k3==0) return 3;
	else return 0;
}

void get_key(void)//��������
{  
  static int key_loop_count=0;
  KEY_N_S_NOW.KEY_NUM=key_check(); 
  if(KEY_N_S_NOW.KEY_NUM==KEY_N_S_PER.KEY_NUM&&KEY_N_S_NOW.KEY_NUM!=0)key_loop_count++; 
  if(KEY_N_S_NOW.KEY_NUM==0 && KEY_N_S_PER.KEY_NUM==0 )//0-δ����
  {
    Key_State.KEY_NUM=KEY_N_S_NOW.KEY_NUM;
    Key_State.KEY_STATE=0; 
    key_loop_count=0;
  }    
  if(key_loop_count>push_time)//2-�ж��Ƿ񳤰�
  {
    Key_State.KEY_NUM=KEY_N_S_NOW.KEY_NUM;
    Key_State.KEY_STATE=2;  
    key_loop_count=100;
  }    
  if(KEY_N_S_NOW.KEY_NUM==0&&KEY_N_S_PER.KEY_NUM!=0&&key_loop_count<=push_time)//1-�ж��Ƿ�̰�
  {
    Key_State.KEY_NUM=KEY_N_S_PER.KEY_NUM;
    Key_State.KEY_STATE=1;  
    key_loop_count=0;
  }  
  KEY_N_S_PER.KEY_NUM=KEY_N_S_NOW.KEY_NUM;
  key_place();

}

char key_line=0;
char key_page=0;
char key_sure=0;

void key_place(void)//�������ܺ���
{  
    if(Key_State.KEY_STATE==1 && Key_State.KEY_NUM==3 && key_sure==0)//  ��λ��          
    {
       key_line+=1;                               
       if(key_line==4) 
	   {
	   	 key_line=0;//������1��
	   	 key_page++;
		 if(key_page=key_page_max) key_page=0;//���ص�0ҳ
	   }
    }    
    if(Key_State.KEY_STATE==1 && Key_State.KEY_NUM==1 && key_sure==0)            
    {
       key_line-=1;
       if(key_line==-1) 
	   {
	   	 key_line=3;//������4��          
	   	 key_page--;
		 if(key_page<0) key_page=0; 
	   }
    } 
////////////////////////////////////////////////////////////////////////////////    
    if(Key_State.KEY_STATE==1 && Key_State.KEY_NUM==2) key_sure=~key_sure; //ȷ����  
	parame_set();
	long_push();                   
}
char tiaoling=1;
void key_function(char  parame_page,char parame_line,char *parame_addr,char variation)
{
  if(key_sure<0 && variation>0)//�̰�
  {
      if(key_page==parame_page && key_line==parame_line)
      {
           if(Key_State.KEY_STATE==1 && Key_State.KEY_NUM==1) //��
           {
              *parame_addr=*parame_addr+variation;    
           }       
           if(Key_State.KEY_STATE==1 && Key_State.KEY_NUM==3) //��
           {
              *parame_addr=*parame_addr-variation;    
           } 
       }
   }
}



void long_push(void)
{
   //if(key_sure==0) //���� ��ѱ仯�����ó�0

   //key1����
   if(Key_State.KEY_STATE==2 && Key_State.KEY_NUM==1)  tiaoling=1;//����
   if(Key_State.KEY_STATE==2 && Key_State.KEY_NUM==2)  IAP_CONTR=0x20;//��λ
   //if(Key_State.KEY_STATE==2 && Key_State.KEY_NUM==3)  tiaoling=3;
          
}
void parame_set(void)
{
  key_function(0,0,&tiaoling,1);//���� �� 0 ҳ �� 0 �� ce ����
}








