#ifndef LED_H_H
#define LED_H_H

#define  LED_RED_OFF       GPIO_ResetBits(GPIOC,GPIO_Pin_7)  //熄灭红色LED指示灯
#define  LED_GREEN_OFF     GPIO_ResetBits(GPIOC,GPIO_Pin_8)  //熄灭绿色LED指示灯
#define  LED_YELLOW_OFF    GPIO_ResetBits(GPIOC,GPIO_Pin_10) //熄灭黄色LED指示灯
#define  LED_BLUE_OFF      GPIO_ResetBits(GPIOA,GPIO_Pin_12) //熄灭蓝色LED指示灯

#define  LED_RED_ON        GPIO_SetBits(GPIOC,GPIO_Pin_7)    //点亮红色LED指示灯
#define  LED_GREEN_ON      GPIO_SetBits(GPIOC,GPIO_Pin_8)    //点亮绿色LED指示灯
#define  LED_YELLOW_ON     GPIO_SetBits(GPIOC,GPIO_Pin_10)   //点亮黄色LED指示灯
#define  LED_BLUE_ON       GPIO_SetBits(GPIOA,GPIO_Pin_12)   //点亮蓝色LED指示灯


#define  LED_R    0X01
#define  LED_G    0X02
#define  LED_Y    0X04
#define  LED_B    0X08

#define  LED_OFF  0
#define  LED_ON   (!LED_OFF)


extern void LED_Init(void);
extern void LED_Switch(const uint8_t statu,const uint8_t mode);

extern void Delay(unsigned int i);
#endif
