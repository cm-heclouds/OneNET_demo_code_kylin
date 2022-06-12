#ifndef __Hal_I2C_H
#define __Hal_I2C_H

#include "stm32f10x.h"

/**
  * @brief  I2C 使用的GPIO配置
  * @param  I2Cx：选择I2C接口，I2C1，I2C2
  * @retval None
  **/
void GPIO_I2C_Configuration(I2C_TypeDef* I2Cx);

/**
  * @brief  I2C控制器配置
  * @param  I2Cx：选择I2C接口，I2C1，I2C2
  * @retval None
  **/
void I2C_Configuration(I2C_TypeDef* I2Cx);

/**
  * @brief  初始化所使用的I2C接口
  * @param  None
  * @retval None
  **/
void Hal_I2C_Init(void);  
	
/**
  * @brief  用户自行实现，用于检测I2C设备是否准备好，否则继续调用I2C可能会陷入死循环或其他错误状态
  * @param  None
  * @retval None
  **/
void Check_I2C_Device_Ready(void);

/**
  * @brief  I2C写数据
  * @param  I2Cx：I2C1，I2C2，选择接口号
  * @param  i2c_slave_addr：从设备地址
  * @param  RegAddr：需要写入数据的寄存器地址
  * @param  data：需要写入的寄存器值
  * @retval ret:0,操作成功，-1，失败
  **/
int8_t Hal_I2C_ByteWrite(I2C_TypeDef* I2Cx,uint16_t i2c_slave_addr, uint8_t RegAddr,uint8_t *data); 

/**
  * @brief  I2C读数据
  * @param  I2Cx：I2C1，I2C2，选择接口号
  * @param  i2c_slave_addr：从设备地址
  * @param  RegAddr：需要读取数据的寄存器地址
  * @param  val：保存I2C读取返回值
  * @retval ret:0,操作成功，-1，失败
  **/
int8_t Hal_I2C_ByteRead(I2C_TypeDef* I2Cx,uint16_t i2c_slave_addr, uint8_t regAddr,uint8_t *val);  

/**
  * @brief  I2C一次读多个数据
  * @param  I2Cx：I2C1，I2C2，选择接口号
  * @param  buf：存储返回数据的缓冲区
  * @param  RegAddr：需要读取数据的寄存器地址
  * @param  i2c_slave_addr：从设备地址
  * @param  num：读取字节数
  * @retval ret:0,操作成功，-1，失败
  **/
int8_t Hal_I2C_MutiRead(I2C_TypeDef* I2Cx,uint8_t* buf, uint8_t i2c_slave_addr, uint8_t regAddr,uint8_t num);

#define STM32_PHER_I2C2
//#define STM32_PHER_I2C1

#ifdef STM32_PHER_I2C2
#define STM32_PHER_I2C2_SDA GPIO_Pin_11
#define STM32_PHER_I2C2_SCL GPIO_Pin_10
#endif 

#ifdef STM32_PHER_I2C1
#define STM32_PHER_I2C1_SDA GPIO_Pin_11
#define STM32_PHER_I2C1_SCL GPIO_Pin_10
#endif
#endif /* __I2C_EE_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/


