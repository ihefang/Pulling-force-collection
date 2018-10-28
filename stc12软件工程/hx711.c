#include<reg52.h>
#include "hx711.h"

sbit ADDO = P3^3; 
sbit ADSK = P3^2; 

/****************************************************************************** 
hx711读取AD值
******************************************************************************/
//

unsigned long ReadCount(void)
{ 
     
	unsigned long Count; 
	unsigned int ADval;
	unsigned char i;
	ADSK=0;  
	ADDO=1; 
	Count=0; 
	while(ADDO);
    for(i=0;i<24;i++)   //读24个bit
    { 
      	ADSK=1; 
      	Count=Count<<1; 
      	ADSK=0; 
       	if(ADDO)Count++; 
   	} 
    ADSK=1; //发送第25个脉冲，表示下次转换使用A通道128db
   	Count=Count^0x800000;
	 	
   	ADSK=0;	
   	ADval = Count>>8;

    ADval = ADval - 30000;

   	return(ADval); //返回为  16位		
} 

		  
