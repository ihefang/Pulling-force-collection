#include<reg52.h>
#include "hx711.h"

sbit ADDO = P3^3; 
sbit ADSK = P3^2; 

/****************************************************************************** 
hx711��ȡADֵ
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
    for(i=0;i<24;i++)   //��24��bit
    { 
      	ADSK=1; 
      	Count=Count<<1; 
      	ADSK=0; 
       	if(ADDO)Count++; 
   	} 
    ADSK=1; //���͵�25�����壬��ʾ�´�ת��ʹ��Aͨ��128db
   	Count=Count^0x800000;
	 	
   	ADSK=0;	
   	ADval = Count>>8;

    ADval = ADval - 30000;

   	return(ADval); //����Ϊ  16λ		
} 

		  
