#include "stm32f10x.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "usart1.h"
#include "usart2.h"
#include "EdpKit.h"

#define  GPRS_PWR_ON   GPIO_SetBits(GPIOA,GPIO_Pin_0)

extern void GpioInit(void);
