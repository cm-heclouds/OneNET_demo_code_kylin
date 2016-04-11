#include "main.h"


volatile unsigned char  red_value=0;
volatile unsigned char  yellow_value=0;
volatile unsigned char  blue_value=0;
volatile unsigned char  green_value=0;

/**
  * @brief  LED指示灯初始化函数
**/
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //PC7--对应开发板红色指示灯;PC8--对应开发板绿色指示灯；PC10--对应开发板黄色指示灯
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
		//PA12---对应开发板蓝色指示灯
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}

/**
  * @brief  分析平台下发的LED控制命令
**/
void LED_CmdCtl(void)
{
		Buffer *buf=NewBuffer();
		char * cmdid=NULL;
		uint16_t cmdid_len=0;
		char * req=NULL;
		uint32_t req_len=0;
		
		printf("%s\r\n","[LED_CmdCtl]ENTER cmdctrl...");
 		WriteBytes(buf,strstr(usart2_cmd_buf,":")+1,command_len);	
		UnpackCmdReq(buf, &cmdid, &cmdid_len, &req, &req_len); 
	
		if((NULL != strstr(req, "LED11")))
		{
				LED_RED_ON;	
		}
		if((NULL != strstr(req, "LED21")))
		{
				LED_GREEN_ON;	
		}
		if((NULL != strstr(req, "LED31")))
		{
				LED_YELLOW_ON;
		}
		if((NULL != strstr(req, "LED41")))
		{
				LED_BLUE_ON;	
		}
		
		if((NULL != strstr(req, "LED10")))
		{
				LED_RED_OFF;	
		}
		if((NULL != strstr(req, "LED20")))
		{
				LED_GREEN_OFF;
		}
		if((NULL != strstr(req, "LED30")))
		{
				LED_YELLOW_OFF;	
		}
		if((NULL != strstr(req, "LED40")))
		{
				LED_BLUE_OFF;	
		}
		DeleteBuffer(&buf);	
		printf("%s\r\n","[LED_CmdCtl]EXIT cmdctrl...");
}

/**
  * @brief  LED指示灯操作函数，点亮和熄灭指示灯
**/
void LED_Switch(const uint8_t statu,const uint8_t mode)
{
		if(statu)
		{
				LED_RED_OFF;
				LED_GREEN_OFF;
				LED_YELLOW_OFF;
				LED_BLUE_OFF;
			
				if(mode&0x01)
				{
						LED_RED_ON;
				}
				if(mode&0x02)
				{
						LED_GREEN_ON;
				}
				if(mode&0x04)
				{
						LED_YELLOW_ON;
				}
				if(mode&0x08)
				{
						LED_BLUE_ON;
				}
		}
		else
		{
				if(mode&0x01)
				{
						LED_RED_OFF;
				}
				if(mode&0x02)
				{
						LED_GREEN_OFF;
				}
				if(mode&0x04)
				{
						LED_YELLOW_OFF;
				}
				if(mode&0x08)
				{
						LED_BLUE_OFF;
				}
		}
}

/**
  * @brief  获取LED的当前状态值
**/
void LED_GetValue(void)
{
		if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_7))
		{
				red_value=1;
		}
		else
		{
				red_value=0;
		}
		if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_12))
		{
				blue_value=1;
		}
		else
		{
				blue_value=0;
		}
		if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_8))
		{
				green_value=1;
		}
		else
		{
				green_value=0;
		}
		if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_10))								
		{
				yellow_value=1;
		}
		else
		{
				yellow_value=0;
		}
}







