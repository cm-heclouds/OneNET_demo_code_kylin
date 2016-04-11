#ifndef USART2_H_H
#define USART2_H_H

#define MAX_RCV_LEN  1024

extern void usart2_config(void);
extern void usart2_write(USART_TypeDef* USARTx, uint8_t *Data,uint32_t len);


extern unsigned char  usart2_rcv_buf[MAX_RCV_LEN];
extern volatile unsigned int   usart2_rcv_len;

#endif

