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
  * @brief  使用esp8266模块和EDP协议向ONENET平台发送开发板四个LED指示灯的实时状态，并通过平台或者开发板按键控制LED。
**/
int main(void)
{	
		//SystemInit();
		LED_Init();    //LED指示灯初始化函数
		KEY_Init();	   //按键初始化函数
		USART1_Init(); //USART1串口初始化函数
		USART2_Init(); //USART2串口初始化函数
		
		while(1)
		{	
				LED_Switch(LED_ON,LED_R|LED_G|LED_Y|LED_B);	   //点亮开发板四个指示灯，表示程序进入主流程
				ESP8266_Init();    //ESP8266初始化
				ESP8266_DevLink(DEVICEID,APIKEY,20);    //和平台建立设备连接
				LED_Switch(LED_OFF,LED_R|LED_G|LED_Y|LED_B); //熄灭开发板四个指示灯，表示程序完成设备连接，进入数据发送循环
	
				while(1)
				{								
						if(!(ESP8266_CheckStatus(30)))    //检测ESP8266模块连接状态
						{
								ESP8266_SendDat();     //向平台发送数据
						}
						else
						{
								break;
						}
						
						if(rcv_cmd_flag==1)    //平台LED控制命令接收完成标记
						{
								LED_CmdCtl(); //命令处理
								memset(usart2_cmd_buf,0,usart2_cmd_len); //清除命令接收缓冲
								usart2_cmd_len=0;
								command_len1=0;
								command_len=0;  
								rcv_cmd_flag=0;
						}
				}
		}
		return 0;
}

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
