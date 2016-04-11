#include "stm32f10x.h"
#include "stm32f10x_i2c.h"
#include "hal_i2c.h"
#include "stdio.h"
#include "utils.h"
#define AT24CX_ADDR 0x50

/* 从AT24CXX的指定地址开始读出一个字节 */
uint8_t AT24CXX_ReadByte(uint8_t ReadAddr);

/*  */
/**
* @brief  向AT24CXX的指定地址开始写入一个字节
* @param  WriteAddr:E2PROM中偏移地址
* @param  Data:写入的数据
* @retval None
**/
void AT24CXX_WriteByte(uint8_t WriteAddr, uint8_t Data);
/**
* @brief  AR24CXX E2PROM读写测试循环.
* @param  None
* @retval None
**/
void AT24CXX_loop(void);
