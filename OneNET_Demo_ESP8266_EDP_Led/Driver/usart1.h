#ifndef USART1_H_H
#define USART1_H_H

extern void USART1_Init(void);
extern void usart1_write(USART_TypeDef* USARTx, uint8_t *Data,uint8_t len);

extern unsigned char usart1_rcv_buf[512];
extern volatile unsigned int  usart1_rcv_len;
extern volatile unsigned int  usart1_rcv_flag;
extern volatile unsigned int  usart1_rcv_start;

#endif

