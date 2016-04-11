#include "stm32f10x.h"
#include "stdio.h"
#include "hal_i2c.h"
#include "utils.h"
#define ADXL345_ADDRESS 0x53
//#define ADXL345_ADDRESS 0x1D

/**
  * @brief  ADXL345模块初始化.
  * @param  None
  * @retval None
  **/
void ADXL345_init(void);

/**
  * @brief  获取ADXL345传感器数值，各方向LSB RAW数据.X:data_out[0],Y:data_out[1],Z:data_out[2]
  * @param  data_out:存储传感器采集结果
  * @retval None
  **/
void ADXL345_GETXYZ(int16_t data_out[3])；