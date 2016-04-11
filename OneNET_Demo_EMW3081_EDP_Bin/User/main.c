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

/***头文件引用****/
#include "main.h"


/**
  * @brief  使用EMW3081模块和EDP协议向ONENET平台发送一个二进制文件，其中二进制文件为一个图片文件。
**/
int main(void)
{
		//SystemInit();
		LED_Init();  //LED指示灯初始化函数
		usart1_config(); //USART1串口初始化函数
		usart2_config(); //USART2串口初始化函数
		Delay(2000);
		
		while(1)
		{
				LED_Switch(LED_ON,LED_R|LED_G|LED_Y|LED_B);  //点亮开发板四个指示灯，表示程序进入主流程
				EMW3081_Reboot();  //重启EMW3081模块
				printf("%s\r\n","[main]ENTER COITINIT.");
				SendCmd(CIOTINIT,"+OK",40);	 //初始化EMW3081在ONENET平台的设备参数
				printf("%s\r\n","[main]ENTER COITSTART.");
				SendCmd(CIOTSTART,"+OK",40); //启动EMW3081向平台发送数据
				LED_Switch(LED_OFF,LED_R|LED_G|LED_Y|LED_B); //熄灭开发板四个指示灯，表示程序完成设备连接，进入数据发送循环
				
				while(1)
				{	
						EMW3081_SendBinDat(); //使用EMW3081向平台发送数据

						if((NULL != strstr(usart2_rcv_buf, "+ERR"))) //检测EMW3081发送一个二进制文件过程是否出错
						{
								break; //如果文件发送出错，退出数据发送循环
						}				
				}	
		}	
}		
			
			

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
