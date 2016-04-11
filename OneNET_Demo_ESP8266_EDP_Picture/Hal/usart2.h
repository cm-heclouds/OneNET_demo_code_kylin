#ifndef USART2_H_H
#define USART2_H_H

#include <stm32f10x.h>
#include <stdint.h>
#define MAX_RCV_LEN  1024

extern void USART2_Config(void);
extern void USART2_Write(USART_TypeDef* USARTx, uint8_t *Data,uint8_t len);
extern void USART2_Clear(void);
extern volatile unsigned char  gprs_ready_flag;
extern volatile unsigned char  gprs_ready_count;

extern unsigned char  usart2_rcv_buf[MAX_RCV_LEN];
extern volatile unsigned int   usart2_rcv_len;
void SendCmd(char* cmd, char* result, int timeOut);
#endif

