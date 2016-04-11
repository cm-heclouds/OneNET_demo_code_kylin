#ifndef USART2_H_H
#define USART2_H_H
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define MAX_RCV_LEN 256

/*
 *  @brief USART2初始化函数
 */
extern void USART2_Init(void);
/*
 *  @brief USART2串口发送api
 */
extern void USART2_Write(USART_TypeDef* USARTx, uint8_t *Data, uint32_t len);
extern uint8_t  usart2_rcv_buf[MAX_RCV_LEN];

extern volatile uint32_t   usart2_rcv_len;
/*
 *  @brief USART2串口发送AT命令用
 */
void SendCmd(int8_t* cmd, int8_t* result, int32_t timeOut);
/*
 *  @brief 返回USART2已接收的数据长度
 */
uint32_t USART2_GetRcvNum(void);
/*
 *  @brief 返回USART2已接收的数据到buf，长度为rcv_len
 */
void  USART2_GetRcvData(uint8_t *buf, uint32_t rcv_len);
#endif

