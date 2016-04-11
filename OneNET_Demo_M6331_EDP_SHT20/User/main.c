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
#include "EdpKit.h"
#include "gpio.h"
#include "utils.h"
#include "hal_i2c.h"
#include "EdpDemo.h"
#include "m6331.h"
void Delay(unsigned int i)
{
		unsigned int j=0;
		for(;i>0;i--)
		{
				for(j=0;j<10000;j++);
		}
}
unsigned char post[]="POST http://api.heclouds.com/devices/775733/datapoints HTTP/1.1\r\n\
api-key: nCVNXYCoX68IHG4DgpyNu5aTXfY=\r\n\
Host: api.heclouds.com\r\n\
Content-Length: 62\r\n\
\r\n\
{\"datastreams\":[{\"id\":\"sostime\",\"datapoints\":[{\"value\":41}]}]}";

uint8_t con[]={
0x10,0x2F,0x00,0x03,0x45,0x44,0x50,0x01,0x40,0x00,0x80,0x00,0x06,0x37,0x36,0x38,0x35,0x39,0x36,0x00,0x1C,0x6E,0x43
,0x56,0x4E,0x58,0x59,0x43,0x6F,0x58,0x36,0x38,0x49,0x48,0x47,0x34,0x44,0x67,0x70,0x79,0x4E,0x75,0x35,0x61,0x54,0x58,0x66,0x59,0x3D};
	
int main(void)
{
	#if 1
    /*GPIO初始化*/
    GpioInit();
    /*MT6331上电*/
    GPRS_PWR_ON;
		mDelay(500);
		mDelay(2000);
    /*初始化串口1 用于调试信息输出*/
    USART1_Init();
    /*初始化串口2 用于MT6331通信*/
    USART2_Init();
    mDelay(2000);
    /*I2C初始化，使用I2C2*/
	#endif
    Hal_I2C_Init();
    /*MT6331 GPRS模组初始化*/
    //ESP8266_Init(EDP_CIPSTART,CWJAP);
	  M6331_Init();
    /*EDP协议测试主循环*/
		mDelay(2000);
    while(1)
    {
        EDP_Loop();
    }
}

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
