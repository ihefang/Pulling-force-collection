#ifndef __KEY_H__
#define __KEY_H__

typedef struct
{
   unsigned char KEY_NUM;
   unsigned char KEY_STATE;       
}KEY_NUM_STATE;//�����ż�����״̬

unsigned char key_check(void);
void get_key(void);//��������
void key_place(void);//�������ܺ���
void key_function(char  parame_page,char parame_line,char *parame_addr,char variation);
void parame_set(void);
void long_push(void);
#endif
