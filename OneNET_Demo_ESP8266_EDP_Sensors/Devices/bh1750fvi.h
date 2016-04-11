#include "stm32f10x.h"
#include "stm32f10x_i2c.h"
#include "hal_i2c.h"
#include "stdio.h"

#define BH1750FVI_ADDR 0x23

#define BH1750_DOWN   0x0
#define BH1750_ON   0x01
#define BH1750_RSET 0x07
//commended
#define BH1750_Con_High_RM  0x10
//commended
#define BH1750_Con_High_RM2  0x11
#define BH1750_Con_Low_RM  0x13
//auto power down
#define BH1750_Once_High_RM  0x20
#define BH1750_Once_High_RM2  0x21
#define BH1750_Once_Low_RM  0x23

/* 从BH1750FVI的指定地址开始读出一个字节 */
uint8_t BH1750FVI_ReadByte(uint8_t ReadAddr);
/* 向BH1750FVI的指定地址开始写入一个字节 */

void BH1750FVI_WriteByte(uint8_t WriteAddr, uint8_t *Data);
/**
  * @brief  测试循环
  * @param  None
  * @retval None
  **/
void BH1750FVI_loop(void);

/**
  * @brief  初始化传感器
  * @param  None
  * @retval None
  **/
void BH1750_Init(void);

/**
  * @brief  获得传感器数值
  * @param  None
  * @retval 返回光照，单位lx
  **/
uint16_t Read_BH1750(void);



