/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
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
#include "stm32f10x_it.h"
#include "main.h"



extern uint32_t SystickTime;
extern __IO uint32_t TimeDisplay;

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    //SystickTime_Increase();
    //SystickTime++;
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

//外部中断，存/取PE[15:7]数据
void EXTI0_IRQHandler(void)
{
    
}

/**
  * @brief  This function handles usart1 global interrupt request.
  * @param  None
  * @retval : None
  */
void USART1_IRQHandler(void)
{
		unsigned int data;

    if(USART1->SR & 0x0F)
    {
        // See if we have some kind of error
        // Clear interrupt (do nothing about it!)
        data = USART1->DR;
    }
    else if(USART1->SR & USART_FLAG_RXNE)      //Receive Data Reg Full Flag
    {		
        data = USART1->DR;
				//usart1_putrxchar(data);     //Insert received character into buffer                     
    }
		else
		{;}
}

/**
  * @brief  This function handles usart2 global interrupt request.
  * @param  None
  * @retval : None
  */
void USART2_IRQHandler(void)
{
		unsigned int data;

    if(USART2->SR & 0x0F)
    {
        // See if we have some kind of error
        // Clear interrupt (do nothing about it!)
        data = USART2->DR;
    }
		else if(USART2->SR & USART_FLAG_RXNE)   //Receive Data Reg Full Flag
    {		
        data = USART2->DR;		
				usart2_rcv_buf[usart2_rcv_len++]=data;
				if(usart2_rcv_len>=MAX_RCV_LEN-1)
				{
						usart2_rcv_len=0;
						memset(usart2_rcv_buf,0,MAX_RCV_LEN);
				}
				
				if((data=='+')&&(rcv_cmd_flag==0))
				{
						rcv_cmd_start=1;
				}
				
				if(rcv_cmd_start==1)
				{
						usart2_cmd_buf[usart2_cmd_len++]=data;		
						if(usart2_cmd_len>=MAX_CMD_LEN-1)
						{
								usart2_cmd_len=0;
								memset(usart2_cmd_buf,0,MAX_CMD_LEN);
								rcv_cmd_start=0;
						}
						
						if(usart2_cmd_len==4)
						{
								if((NULL == strstr(usart2_cmd_buf,"+IPD")))
								{
										usart2_cmd_len=0;
										memset(usart2_cmd_buf,0,MAX_CMD_LEN);
										rcv_cmd_start=0;
								}
						}
						
						if((usart2_cmd_len>6)&&(data==':')&&(command_len==0))
						{
								sscanf(usart2_cmd_buf,"+IPD,%d",&command_len); 
						}
						
						if((usart2_cmd_len>=9)&&(command_len==0))
						{
								rcv_cmd_start=0;
								usart2_cmd_len=0;
								memset(usart2_cmd_buf,0,MAX_CMD_LEN);   
						}
						
						if(command_len!=0)
						{
								if((++command_len1)>command_len)
								{
										//usart1_write(USART1,usart2_cmd_buf,usart2_cmd_len);
										rcv_cmd_start=0;
										rcv_cmd_flag=1;
										
								}	
						}
				}	  
    }
		else
		{
				;
		}
}

/**
  * @brief  This function handles RTC global interrupt request.
  * @param  None
  * @retval : None
  */
void RTC_IRQHandler(void)
{
   
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
