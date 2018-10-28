#include "read voltage value.h"
#include "i2c.h"

#define AddWr 0x90   //写数据地址 
#define AddRd 0x91   //读数据地址
extern bit ack;
/****************************************************************************** 
             读AD转值程序
输入参数 Chl 表示需要转换的通道，范围从0-3
返回值范围0-255				
******************************************************************************/
unsigned char ReadADC(unsigned char Chl)
 {
   unsigned char Val;
   Start_I2c();               //启动总线
   SendByte(AddWr);             //发送器件地址
     if(ack==0)return(0);
   SendByte(0x40|Chl);            //发送器件子地址
     if(ack==0)return(0);
   Start_I2c();
   SendByte(AddWr+1);
      if(ack==0)return(0);
   Val=RcvByte();
   NoAck_I2c();                 //发送非应位
   Stop_I2c();                  //结束总线
  return(Val);
 }
/****************************************************************************** 
               写入DA转换数值
输入参数：dat 表示需要转换的DA数值，范围是0-255				
******************************************************************************/
bit WriteDAC(unsigned char dat)
{
   Start_I2c();               //启动总线
   SendByte(AddWr);             //发送器件地址
     if(ack==0)return(0);
   SendByte(0x40);            //发送器件子地址
     if(ack==0)return(0);
   SendByte(dat);             //发送数据
     if(ack==0)return(0);
   Stop_I2c();     
}





