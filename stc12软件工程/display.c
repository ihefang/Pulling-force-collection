#include"display.h"
#include"delay.h"

#define DataPort P0 //定义数据端口 


unsigned char code DuanMa[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};// 显示段码值0~9
unsigned char code WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//分别对应相应的数码管点亮,即位码
unsigned char TempData[8]; //存储显示值的全局变量

/****************************************************************************** 
 显示函数，用于动态扫描数码管
 输入参数 FirstBit 表示需要显示的第一位，如赋值2表示从第三个数码管开始显示
 如输入0表示从第一个显示。
 Num表示需要显示的位数，如需要显示99两位数值则该值输入2
******************************************************************************/
void Display(unsigned char FirstBit,unsigned char Num)
{
      static unsigned char i=0;
	   DataPort=0xff;   //清空数据，防止有交替重影
       P2=WeiMa[i+FirstBit]; //取位码 
       DataPort=TempData[i]; //取显示数据，段码
	   i++;
       if(i==Num)
	      i=0;
}



