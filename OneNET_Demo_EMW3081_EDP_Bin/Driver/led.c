#include "main.h"

/**
  * @brief  延时函数
**/
void Delay(unsigned int i)
{
		unsigned int j=0;
		for(;i>0;i--)
		{
				for(j=0;j<10000;j++);
		}
}


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








