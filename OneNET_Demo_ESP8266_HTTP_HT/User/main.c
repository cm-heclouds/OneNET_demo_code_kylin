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

#include "stm32f10x.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "usart1.h"
#include "usart2.h"
#include "utils.h"
#include "sht20.h"
#include "hal_i2c.h"
#include "esp8266.h"


#define API_KEY     "DXZcCKxqrpxZJKWFnbMzxIjeITk="		//需要定义为用户自己的参数
#define DEV_ID      "1078702"							//需要定义为用户自己的参数

/**
  * @brief      利用I2C接口，采集温湿度传感器的值，使用HTTP协议上传到OneNET
  * @attention  使用UART2连接ESP8266模块，使用透传模式发送和接收数据
  *	            使用UART1作为调试打印串口，使用printf将从该接口打印消息
  *			
  */
uint32_t HTTP_PostPkt(char *pkt, char *key, char *devid, char *dsid, char *val);
int main(void)
{
    uint16_t temp, humi;    //温湿度
    char HTTP_Buf[400];     //HTTP报文缓存区
    char tempStr[5];       //字符串格式温度
    char humiStr[5];       //字符串格式湿度
    int len;

    USART1_Config();        //USART1作为调试串口
    USART2_Config();        //USART2用于连接ESP8266模块
    Hal_I2C_Init();			//I2C初始化，用于连接温湿度传感器

    ESP8266_Init();         //ESP8266初始化
		printf("ESP8266 init over\r\n");
    //SHT20_loop();         //
	
    while(1)
    {
        /* 获取温湿度 */
        SHT2x_MeasureHM(SHT20_Measurement_T_HM, &temp);
        mDelay(500);
        SHT2x_MeasureHM(SHT20_Measurement_RH_HM, &humi);

        /* 转化为字符串形式 */
        sprintf(tempStr, "%d", temp);
        sprintf(humiStr, "%d", humi);

        //printf("%s   %s\r\n", tempStr, humiStr);

        USART2_Clear();
        len = HTTP_PostPkt(HTTP_Buf, API_KEY, DEV_ID, "temp", tempStr); //HTTP组包
        USART2_Write(USART2, (unsigned char *)(HTTP_Buf), len);			//报文发送
        printf("send HTTP msg:\r\n%s\r\n", HTTP_Buf);

        mDelay(1000);
        printf("rcv response:\r\n%s\r\n", usart2_rcv_buf);

        USART2_Clear();
        len = HTTP_PostPkt(HTTP_Buf, API_KEY, DEV_ID, "humi", humiStr); //HTTP组包
        USART2_Write(USART2, (unsigned char *)(HTTP_Buf), len);			//报文发送
        printf("send HTTP msg:\r\n%s\r\n", HTTP_Buf);

        mDelay(1000);
        printf("rcv response:\r\n%s\r\n", usart2_rcv_buf);

        mDelay(5000);
    }
}

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
