#include "oled.h"
#include "oledfont.h"  	 
#include "delay.h"
/****************************************************************************** 
OLED的显存
存放格式如下.
[0]0 1 2 3 ... 127	
[1]0 1 2 3 ... 127	
[2]0 1 2 3 ... 127	
[3]0 1 2 3 ... 127	
[4]0 1 2 3 ... 127	
[5]0 1 2 3 ... 127	
[6]0 1 2 3 ... 127	
[7]0 1 2 3 ... 127 			
******************************************************************************/			   
#if OLED_MODE==1
/****************************************************************************** 
向SSD1106写入一个字节。
dat:要写入的数据/命令
cmd:数据/命令标志 0,表示命令;1,表示数据;			
******************************************************************************/


void OLED_WR_Byte(u8 dat,u8 cmd)
{
	DATAOUT(dat);	    
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		   
	OLED_CS_Clr();
	OLED_WR_Clr();	 
	OLED_WR_Set();
	OLED_CS_Set();	  
	OLED_DC_Set();	 
} 	    	    
#else

/****************************************************************************** 
向SSD1306写入一个字节。
dat:要写入的数据/命令
cmd:数据/命令标志 0,表示命令;1,表示数据;			
******************************************************************************/

void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();
	  		  
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
			{
		   OLED_SDIN_Set();
			}
        else
		   OLED_SDIN_Clr();
	    OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set();   	  
} 
#endif
/****************************************************************************** 
设置显示位置			
******************************************************************************/
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
}   	  
 
/****************************************************************************** 
开启OLED显示 			
******************************************************************************/  
/*
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
 */
/****************************************************************************** 
关闭OLED显示			
******************************************************************************/    
/*
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
*/
	
/****************************************************************************** 
清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!			
******************************************************************************/  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}



/****************************************************************************** 
在指定位置显示一个字符,包括部分字符
x:0~127
y:0~63
mode:0,反白显示;1,正常显示				 
size:选择字体 16/12 			
******************************************************************************/
void OLED_ShowChar(u8 x,u8 y,u8 chr)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(SIZE ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}
			else 
			{	
				OLED_Set_Pos(x,y+1);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);	
			}
}

/****************************************************************************** 
m^n函数			
******************************************************************************/
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
	
/****************************************************************************** 
显示2个数字
x,y :起点坐标	 
len :数字的位数
size:字体大小
num:数值(0~4294967295);			
******************************************************************************/ 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ');
				continue;
			}
			else enshow=1; 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0'); 
	}
} 

/****************************************************************************** 
显示一个字符号串			
******************************************************************************/
void OLED_ShowString(u8 x,u8 y,u8 *chr)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{	
	    OLED_ShowChar(x,y,chr[j]);
		x+=8;
		if(x>120){x=0;y+=2;}
		j++;
	}
}

/****************************************************************************** 
显示汉字			
******************************************************************************/
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}
/****************************************************************************** 
功能描述：显示显示BMP图片128×64起始点坐标(x,y)
          x的范围0～127
		  y为页的范围0～7
******************************************************************************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
        for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 
/****************************************************************************** 
初始化SSD1306				
******************************************************************************/				    
void OLED_Init(void)
{
 
    OLED_RST_Set();
	delay_ms(100);
	OLED_RST_Clr();
	delay_ms(100);
	OLED_RST_Set(); 

	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
	OLED_Clear();
	OLED_Set_Pos(0,0); 	
}
/****************************************************************************** 
功能描述：显示数字
          x的范围0～127
		  y为页的范围0～7
******************************************************************************/
void display_num(char x,char y,int parame)
{

   int a;//整数
   float b;//小数
   float parame1;
  //uint xiao;
  //uint zheng;
  uint wan=0;
  uint qian=0;
  uint bai=0;
  uint shi=0;
  uint ge=0;
  uint ling=0;
  uint lingling=0;
 char the_num[6]={0};

  if(parame==0)//此条件屏蔽可显示刷新频率很高参数的 最后状态 即（参数死态）
  {
   the_num[0]=0+0x30;
   the_num[1]=' ';
   the_num[2]=' ';
   the_num[3]=' ';
   the_num[4]=' ';
   the_num[5]='\0';
   //LCD_Print(x,y,&the_num[0]);
   OLED_ShowString(x,y,&the_num[0]);
  }
  
  if(parame<0)//负数
  { 
    parame1=0-parame;   
    the_num[0]='-';
    if(parame1>999&&parame1<10000)
    {
          qian =parame1/1000;
          bai=(parame1-qian*1000)/100;
          shi =(parame1-qian*1000-bai*100)/10;
          ge =parame1-qian*1000-bai*100-shi*10;
          the_num[0]='-';
          the_num[1]=qian+0x30;
          the_num[2]=bai+0x30;
          the_num[3]=shi+0x30;
          the_num[4]=ge+0x30;
          the_num[5]='\0';
          OLED_ShowString(x,y,&the_num[0]);
     }
  if(parame1>99&&parame1<1000)
    {
          bai =parame1/100;
          shi=(parame1-bai*100)/10;
          ge =(parame1-bai*100-shi*10)/1;
          the_num[0]='-';
          the_num[1]=bai+0x30;
          the_num[2]=shi+0x30;
          the_num[3]=ge+0x30;       
          the_num[4]=' ';
          the_num[5]='\0';          
          OLED_ShowString(x,y,&the_num[0]);
     }
  if(parame1>9&&parame1<100)
    {
          shi =parame1/10;
          ge=(parame1-shi*10)/1;
          the_num[0]='-';
          the_num[1]=shi+0x30;
          the_num[2]=ge+0x30;
          the_num[3]=' ';
          the_num[4]=' ';
          the_num[5]='\0'; 
          OLED_ShowString(x,y,&the_num[0]);
     }
  if(parame1>=1&&parame1<10)
    {
          ge =parame1/1;
          the_num[0]='-';
          the_num[1]=ge+0x30;
          the_num[2]=' ';
          the_num[3]=' ';
          the_num[4]=' ';
          the_num[5]='\0'; 
          OLED_ShowString(x,y,&the_num[0]);
     }
  }
  if(parame>=0)
  {
    a=parame/1;
    b=parame-a;
     
  }  
  if(b==0)//整数
  {
       if(parame>9999)
       {
          wan =parame/10000;
          qian=(parame-wan*10000)/1000;
          bai =(parame-wan*10000-qian*1000)/100;
          shi =(parame-wan*10000-qian*1000-bai*100)/10;
          ge  =parame-wan*10000-qian*1000-bai*100-shi*10;
          the_num[0]=wan+0x30;
          the_num[1]=qian+0x30;
          the_num[2]=bai+0x30;
          the_num[3]=shi+0x30;
          the_num[4]=ge+0x30;
          the_num[5]='\0';
          OLED_ShowString(x,y,&the_num[0]);
       }
       if(parame>999&&parame<10000)
       {
          qian=parame/1000;
          bai=(parame-qian*1000)/100;
          shi=(parame-qian*1000-bai*100)/10;
          ge=parame-qian*1000-bai*100-shi*10;
          the_num[0]=qian+0x30;
          the_num[1]=bai+0x30;
          the_num[2]=shi+0x30;
          the_num[3]=ge+0x30;
          the_num[4]=' ';
          the_num[5]='\0';           
          OLED_ShowString(x,y,&the_num[0]);
       }
      if(parame>99&&parame<1000)//3位
      {
          bai=parame/100;
          shi=(parame-bai*100)/10;
          ge=(parame-bai*100-shi*10)/1;
          the_num[0]=bai+0x30;
          the_num[1]=shi+0x30;
          the_num[2]=ge+0x30;
          the_num[3]=' ';
          the_num[4]=' ';
          the_num[5]='\0';           
          OLED_ShowString(x,y,&the_num[0]);
       }
       if(parame>9&&parame<100)
       {
          shi =parame/10;
          ge=(parame-shi*10)/1;
          the_num[0]=shi+0x30;
          the_num[1]=ge+0x30;
          the_num[2]=' ';
          the_num[3]=' ';
          the_num[4]=' ';
          the_num[5]='\0';          
          OLED_ShowString(x,y,&the_num[0]);
       }
  if(parame>=1&&parame<10)
    {
          ge =parame/1;
          the_num[0]=ge+0x30;
          the_num[1]=' ';
          the_num[2]=' ';
          the_num[3]=' ';
          the_num[4]=' ';
          the_num[5]='\0';           
          OLED_ShowString(x,y,&the_num[0]);
     }
  }
 /*
  if(b>0)//小数
  {   
       parame=parame+0.001;
       zheng = parame/1;
       xiao  = (parame-zheng)*100;
       
       if(zheng>9&&zheng<100)
       {
          shi=zheng/10;
          ge=(zheng-shi*10);
          if(xiao>9&&xiao<100)
          {
            ling=xiao/10;
            lingling=(xiao-ling*10);           
          }
          if(xiao>0&&xiao<10)
          {
            ling=0;
            lingling=xiao/1;          
          } 
          the_num[0]=shi+0x30;
          the_num[1]=ge+0x30;
          the_num[2]='.';
          the_num[3]=ling+0x30;
          the_num[4]=lingling+0x30;
          the_num[5]='\0';
          OLED_ShowString(x,y,&the_num[0]);
       }
       
       
       if(zheng>0&&zheng<10)
       {
          ge=zheng;
          if(xiao>9&&xiao<100)
          {
            ling=xiao/10;
            lingling=(int)(xiao-ling*10);           
          }									
          if(xiao>0&&xiao<10)
          {
            ling=0;
            lingling=(int)xiao;          
          } 
          the_num[0]=ge+0x30;
          the_num[1]='.';
          the_num[2]=ling+0x30;
          the_num[3]=lingling+0x30;
          the_num[4]=' ';
          the_num[5]='\0';          
          OLED_ShowString(x,y,&the_num[0]);
       }        
       if(zheng<1)
       {
          ge=0;
          if(xiao>9&&xiao<100)
          {
            ling=xiao/10;
            lingling=(xiao-ling*10);           
          }
          if(xiao>1&&xiao<10)			
          {
            ling=0;
            lingling=xiao/1;          
          } 
          the_num[0]=ge+0x30;
          the_num[1]='.';
          the_num[2]=ling+0x30;
          the_num[3]=lingling+0x30;
          the_num[4]=' ';
          the_num[5]='\0';          
          OLED_ShowString(x,y,&the_num[0]);
       }        
  }		 */
}
/******************************************************/  







