#include "main.h"


void mDelay(uint32_t i)
{
		uint32_t j=0;
		for(;i>0;i--)
		{
				for(j=0;j<12000;j++);
		}
}


/**
  * @brief  按键初始化函数
**/
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	
    //PC13--SW5  PC12--SW4   PC11--SW3
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_12|GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
		//PD2--SW2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource2);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line2;	
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 			
		NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x06;	
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								
  	NVIC_Init(&NVIC_InitStructure);
		
		
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource11);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line11;	
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x06;	
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								
  	NVIC_Init(&NVIC_InitStructure);
		
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource12);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line12;	
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x06;	
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								
  	NVIC_Init(&NVIC_InitStructure);
		
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource13);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line13;	
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 
		NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x06;	
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								
  	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  按键中断处理函数
**/
void EXTI15_10_IRQHandler(void)
{		
		mDelay(15);  
		if(EXTI_GetITStatus(EXTI_Line11)==SET) 
		{
				if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11)==0)	  
				{
						if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_8))
						{
								LED_GREEN_OFF;
						}
						else
						{
								LED_GREEN_ON;
						}
				}		 
		}
		
		if(EXTI_GetITStatus(EXTI_Line12)==SET) 
		{
				if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)==0)	  
				{
						if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_12))
						{
								LED_BLUE_OFF;
						}
						else
						{
								LED_BLUE_ON;
						}
				}		 
		}
		
		if(EXTI_GetITStatus(EXTI_Line13)==SET) 
		{
				if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)==0)	  
				{
						if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_10))
						{
								LED_YELLOW_OFF;
						}
						else
						{
								LED_YELLOW_ON;
						}
				}	
		}
		EXTI_ClearITPendingBit(EXTI_Line11);
		EXTI_ClearITPendingBit(EXTI_Line12);
	 	EXTI_ClearITPendingBit(EXTI_Line13);
		//mDelay(10);	
}


void EXTI2_IRQHandler(void)
{               	
		mDelay(15);
		if(EXTI_GetITStatus(EXTI_Line2)==SET) 
		{
				if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)==0)	  
				{
						if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_7))
						{
								LED_RED_OFF;
						}
						else
						{
								LED_RED_ON;
						}
				}	
		}
		EXTI_ClearITPendingBit(EXTI_Line2); 
		//mDelay(10);		
}

