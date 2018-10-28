#ifndef __UART_H__
#define __UART_H__
  
  void UART_Init(void);
  void Uart_SendString();
  void Uart_SendChar(unsigned char dat);
  void send_hx711(int w);
#endif