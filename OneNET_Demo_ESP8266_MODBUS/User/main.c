/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "usart1.h"
#include "usart2.h"
#include "utils.h"
#include "esp8266.h"
#include "modbus.h"

int main(void)
{
    /*初始化串口1 用于打印调试信息， 115200 8N1*/
    USART1_Init();
    /*初始化串口2 用于和ESP8266进行数据通信， 115200 8N1*/
    USART2_Init();
    mDelay(1000);
    /*初始化ESP8266模组，并连接CWJAP指定的AP，与MODBUS_CIPSTART配置的MODBUS服务器建立tcp连接*/
    ESP8266_Init((int8_t *)MODBUS_CIPSTART, (int8_t *)CWJAP);
   	//MT6331_Init();
    /*进入modbus协议测试主循环*/
    MODBUS_Loop();
}

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
