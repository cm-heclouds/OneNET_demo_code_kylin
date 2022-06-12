#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_exti.h"
#include "misc.h"
#include "usart2.h"
#include "stm32f10x.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "utils.h"
#include "usart2.h"

uint8_t  usart2_rcv_buf[MAX_RCV_LEN];
volatile uint32_t   usart2_rcv_len = 0;

/*
 *  @brief USART2初始化函数
 */
void USART2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* config USART2 clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    /* USART2 GPIO config */
    /* Configure USART2 Tx (PA.02) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* Configure USART2 Rx (PA.03) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* USART2 mode config */
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    USART_Cmd(USART2, ENABLE);

    //Enable usart2 receive interrupt
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/*
*  @brief USART2串口发送api
*/
void USART2_Write(USART_TypeDef* USARTx, uint8_t *Data, uint32_t len)
{
    uint32_t i;
    USART_ClearFlag(USARTx, USART_FLAG_TC);
    for(i = 0; i < len; i++)
    {
        USART_SendData(USARTx, *Data++);
        while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
    }
}
/*
 *  @brief USART2串口发送AT命令用
 */
void SendCmd(uint8_t* cmd, int8_t* result, int32_t timeOut)
{
    while(1)
    {
        memset(usart2_rcv_buf, 0, strlen((const char *)usart2_rcv_buf));
        usart2_rcv_len = 0;

        USART2_Write(USART2, cmd, strlen((const char *)cmd));
        mDelay(timeOut);
        printf("%s %d cmd:%s,rsp:%s\n", __func__, __LINE__, cmd, usart2_rcv_buf);
			
        if((NULL != strstr((const char *)usart2_rcv_buf, "ERROR")))
        {
            continue;
        }
				
        if((NULL != strstr((const char *)usart2_rcv_buf, (const char *)result)))
        {
            break;
        }
        else
        {
            mDelay(100);
        }
    }
}
#if 1
/*
 *  @brief 返回USART2已接收的数据长度
 */
uint32_t USART2_GetRcvNum(void)
{
    return usart2_rcv_len;
}
/*
 *  @brief 返回USART2已接收的数据到buf，长度为rcv_len
 */
void  USART2_GetRcvData(uint8_t *buf, uint32_t rcv_len)
{
    if(buf)
    {
        memcpy(buf, usart2_rcv_buf, rcv_len);
    }
    memset(usart2_rcv_buf, 0, sizeof(usart2_rcv_buf));
    usart2_rcv_len = 0;
}
#endif
