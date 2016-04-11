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

#include "cJSON.h"
#include "esp8266.h"
#include "Edpkit.h"


#define API_KEY     "DXZcCKxqrpxZJKWFnbMzxIjeITk="  //API_KEY 需要修改为用户自己的对应参数
#define DEV_ID      "1078702"                       //设备ID  需要修改为用户自己的对应参数
#define PKT_SIZE 	200								//图片分包大小，根据模块特性修改，分包过大容易导致模块丢包

#include "image_2k.c"   //图片文件二进制数组

/**
  * @brief      实现EDP连接，每5s上传一次图片数据到平台
  * @attention  使用UART2连接ESP8266模块，使用透传模式发送和接收数据
  *			    使用UART1作为调试打印串口，使用printf将从该接口打印消息
  *			    Recv_Thread_Func函数是调用的官方edp_sdk中提供的函数，在其上加以修改
  *			    其中提供了所有的EDP包的解析函数
  */
int main(void)
{
    cJSON *desc_json;
    char text[] = "{\"ds_id\":\"pic\"}";	//图像属性：数据流ID -> pic
    EdpPacket* send_pkg;

    USART1_Config();        //USART1作为打印串口
    USART2_Config();        //USART2连接ESP8266模块

    mDelay(3000);

init:

    ESP8266_Init();         //模块初始化
	printf("ESP8266 init over\r\n");
	
	/* 发送EDP设备连接包 */
	send_pkg = PacketConnect1(DEV_ID, API_KEY);
    if(send_pkg == NULL)
    {
		printf("conn pkt create error\r\n");
        return;
    }
	printf("send connect to server, bytes: %d\r\n", send_pkg->_write_pos);
    USART2_Write(USART2, (uint8_t*)(send_pkg->_data), send_pkg->_write_pos);	//串口发送
	hexdump(send_pkg->_data, send_pkg->_write_pos);	//打印发送内容
    DeleteBuffer(&send_pkg);		//完成发送需要将缓冲区释放
    mDelay(1000);
	
	Recv_Thread_Func();				//查看接收数据，此处只用于解析连接应答
		
	desc_json = cJSON_Parse(text);	//图像属性，CJSON格式化

	/* 循环发送图片数据 */
    while(1)
    {
        uint8_t *p = (uint8_t *)image;
        int32_t tmp = sizeof(image);

        //Recv_Thread_Func();

        /* 上传图片 - 发送EDP包头 */
		send_pkg = PacketSavedataBin(NULL, desc_json, image, sizeof(image));//创建EDP包头，不包含二进制数据
		if(send_pkg == NULL)
		{
			printf("save bin pkt create error\r\n");
			return;
		}
		printf("send EDP head\r\n");
		USART2_Write(USART2, (uint8_t*)(send_pkg->_data), send_pkg->_write_pos);//串口发送
		hexdump(send_pkg->_data, send_pkg->_write_pos);	//打印发送内容
		DeleteBuffer(&send_pkg);		//完成发送需要将缓冲区释放

        /* 上传图片 - 图片分片发送 */
		printf("send pic");
        while(tmp > 0)
        {
            printf(".");
            if(tmp >= PKT_SIZE)
            {
                USART2_Write(USART2, p, PKT_SIZE);	//串口发送分片
                p += PKT_SIZE;
                tmp -= PKT_SIZE;
            }
            else
            {
                USART2_Write(USART2, p, tmp);		//串口发送最后一个分片
                tmp = 0;
				printf("\r\npic send over\r\n");
            }
            mDelay(1000);	//1s发送一个分片
        }

        USART2_Clear();
        mDelay(5000);
    }
}

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
