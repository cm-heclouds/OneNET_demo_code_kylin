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
//USE_STDPERIPH_DRIVER, STM32F10X_HD, USE_STM3210B_EVAL

/***头文件调用****/
#include "stm32f10x.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "usart1.h"
#include "usart2.h"
#include "utils.h"
#include "esp8266.h"
#include "hal_i2c.h"
#include "mqtt_loop.h"

int main(void)
{
    USART1_Init();
    USART2_Init();
    mDelay(1000);
    Hal_I2C_Init();
    /*初始化8266，并连接CWJAP配置的AP，与MQTT_CIPSTART配置的服务器建立TCP连接*/
    ESP8266_Init((int8_t *)MQTT_CIPSTART, (int8_t *)CWJAP);
    /*进入MQTT协议测试主循环*/
    MQTT_Loop();
}

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
