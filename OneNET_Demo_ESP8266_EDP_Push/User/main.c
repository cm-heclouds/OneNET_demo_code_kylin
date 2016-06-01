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
#include "usart1.h"
#include "usart2.h"
#include "utils.h"
#include "Edpkit.h"
#include "esp8266.h"

#define API_KEY     "DXZcCKxqrpxZJKWFnbMzxIjeITk="  //API_KEY 需要修改为用户自己的对应参数
#define DEV_ID      "1078702"                       //设备ID  需要修改为用户自己的对应参数
void sendPkt(EdpPacket **p);
void Recv_Thread_Func(void);
/**
  * @brief     实现EDP连接，心跳包保持在线，接收透传消息并返回
  * @attention 使用UART2连接ESP8266模块，使用非透传模式发送和接收数据
  *			   使用UART1作为调试打印串口，使用printf将从该接口打印消息
  *			   Recv_Thread_Func函数是调用的官方edp_sdk中提供的函数，在其上加以修改
  *			   其中提供了所有的EDP包的解析函数
  */
int main(void)
{
    EdpPacket* edpPkt;
	uint32_t timeCount = 0;

    USART1_Config();        //USART1作为打印串口
    USART2_Config();        //USART2连接ESP8266模块

    mDelay(2000);			

    ESP8266_Init();         //模块初始化，非透传模式
	
	/* EDP设备连接包，组包 */
	edpPkt = PacketConnect1(DEV_ID, API_KEY);
	printf("send connect to server, bytes: %d\r\n", edpPkt->_write_pos);

	sendPkt(&edpPkt);				// 非透传模式发送报文
    mDelay(1000);
	Recv_Thread_Func();				//接收数据
		
	/* 循环检测串口接收数据，每200s发送一包心跳包 */
    while(1)
    {
		/* 间隔一段时间发送一次ping包 */
		if(timeCount >= 20)
		{
			timeCount = 0;
			
			/* EDP设备心跳包，组包 */
			edpPkt = PacketPing();
			printf("send ping pkt to server, bytes: %d\r\n", edpPkt->_write_pos);
			sendPkt(&edpPkt);		//非透传模式发送
		}
		
		Recv_Thread_Func();			//接收数据
        USART2_Clear();
		timeCount++;
		mDelay(100);
    }
}

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
