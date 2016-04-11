#ifndef USART2_H_H
#define USART2_H_H

#define MAX_RCV_LEN  1024
#define MAX_CMD_LEN  512


extern void USART2_Init(void);
extern void usart2_write(USART_TypeDef* USARTx, uint8_t *Data,uint32_t len);

extern volatile unsigned char  rcv_cmd_start;
extern volatile unsigned char  rcv_cmd_flag;

extern unsigned char  usart2_rcv_buf[MAX_RCV_LEN];
extern volatile unsigned int   usart2_rcv_len;

extern unsigned char  usart2_cmd_buf[MAX_CMD_LEN];
extern volatile unsigned int   usart2_cmd_len;

extern volatile unsigned int   command_len;
extern volatile unsigned int   command_len1;


#endif

