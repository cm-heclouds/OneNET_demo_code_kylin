#include "stm32f10x.h"
#include "stdio.h"
#include "hal_i2c.h"
#include "stm32f10x.h"
#include "stdio.h"
#include "hal_i2c.h"

/**
  * @brief  reset SHT20模块.
  * @param  None
  * @retval None
  **/
void SHT20_reset(void);

/**
  * @brief  读取 SHT20模块的用户寄存器
  * @param  None
  * @retval 返回用户寄存器值
  **/
uint8_t  SHT20_read_user_reg(void);

/**
  * @brief  写 SHT20模块的用户寄存器，写之前先读
  * @param  None
  * @retval None
  **/
void SHT20_write_user_reg(uint8_t val);

/**
  * @brief  保持模式读SHT20模块温度或湿度
  * @param  cmd：SHT20_Measurement_RH_HM or SHT20_Measurement_T_HM
  * @param  pMeasurand：返回结果
  * @retval ret:0,成功，<0 失败
  **/
int8_t SHT2x_MeasureHM(uint8_t cmd, uint16_t *pMeasurand);

/**
  * @brief  非保持模式读SHT20模块温度或湿度
  * @param  cmd：SHT20_Measurement_RH_NHM or SHT20_Measurement_T_NHM
  * @param  pMeasurand：返回结果
  * @param  timeout：超时次数
  * @retval ret:0,成功，<0 失败
  **/
int8_t SHT2x_MeasurePoll(uint8_t cmd, uint16_t *pMeasurand, int32_t timeout);

/**
  * @brief  读SHT20模块序列号
  * @param  SerialNumber：8字节数据，用于存储返回的序列号
  * @retval ret:0,成功，<0 失败
  **/
int8_t SHT2x_GetSerialNumber(uint8_t SerialNumber[]);

/*SHT20 设备操作相关宏定义，详见手册*/
#define SHT20_ADDRESS  0X40
#define SHT20_Measurement_RH_HM  0XE5
#define SHT20_Measurement_T_HM  0XE3
#define SHT20_Measurement_RH_NHM  0XF5
#define SHT20_Measurement_T_NHM  0XF3
#define SHT20_READ_REG  0XE7
#define SHT20_WRITE_REG  0XE6
#define SHT20_SOFT_RESET  0XFE
