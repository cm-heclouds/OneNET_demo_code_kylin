#ifndef USART1_H_H
#define USART1_H_H
/*
 *  @brief USART1初始化函数
 */
extern void USART1_Init(void);
/*
 *  @brief USART1串口发送api
 */
extern void USART1_Write(USART_TypeDef* USARTx, uint8_t *Data, uint32_t len);

extern uint8_t usart1_rcv_buf[256];
extern volatile uint32_t  usart1_rcv_len;
extern volatile uint32_t  usart1_rcv_flag;
extern volatile uint32_t  usart1_rcv_start;

#endif

