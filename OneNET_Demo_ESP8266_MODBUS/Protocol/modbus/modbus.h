#ifndef __MODBUS_HEADER__
#define __MODBUS_HEADER__

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

#define MAXLINE 128
#define MY_MODBUS_ADDR 0x11
#define MB_FUNC_READ_HOLDING 3

/**
 * @brief  modbus协议主循环
 * @param  None
 * @retval None
 **/
void MODBUS_Loop(void);

/**
 * @brief  modbus协议命令处理入口
 * @param  None
 * @retval None
 **/
void Process_Cmd(uint8_t * buf, int32_t len);
/*
 *  @brief modbus登陆鉴权
 * @param  None
 * @retval None
 */
void MODBUS_Login(void);
#endif
