#ifndef __KEY_H__
#define __KEY_H__

typedef struct
{
   unsigned char KEY_NUM;
   unsigned char KEY_STATE;       
}KEY_NUM_STATE;//按键号及按键状态

unsigned char key_check(void);
void get_key(void);//按键设置
void key_place(void);//按键功能函数
void key_function(char  parame_page,char parame_line,char *parame_addr,char variation);
void parame_set(void);
void long_push(void);
#endif
