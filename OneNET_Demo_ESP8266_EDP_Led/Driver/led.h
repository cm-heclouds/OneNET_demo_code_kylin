#ifndef LED_H_H
#define LED_H_H

#define  LED_RED_OFF       GPIO_ResetBits(GPIOC,GPIO_Pin_7)
#define  LED_GREEN_OFF     GPIO_ResetBits(GPIOC,GPIO_Pin_8)
#define  LED_YELLOW_OFF    GPIO_ResetBits(GPIOC,GPIO_Pin_10)
#define  LED_BLUE_OFF      GPIO_ResetBits(GPIOA,GPIO_Pin_12)

#define  LED_RED_ON        GPIO_SetBits(GPIOC,GPIO_Pin_7)
#define  LED_GREEN_ON      GPIO_SetBits(GPIOC,GPIO_Pin_8)
#define  LED_YELLOW_ON     GPIO_SetBits(GPIOC,GPIO_Pin_10)
#define  LED_BLUE_ON       GPIO_SetBits(GPIOA,GPIO_Pin_12)


#define  LED_R    0X01
#define  LED_G    0X02
#define  LED_Y    0X04
#define  LED_B    0X08

#define  LED_OFF  0
#define  LED_ON   (!LED_OFF)

extern volatile unsigned char  red_value;
extern volatile unsigned char  yellow_value;
extern volatile unsigned char  blue_value;
extern volatile unsigned char  green_value;

extern void LED_Init(void);
extern void LED_CmdCtl(void);
extern void LED_Switch(const uint8_t statu,const uint8_t mode);
extern void LED_GetValue(void);


#endif
