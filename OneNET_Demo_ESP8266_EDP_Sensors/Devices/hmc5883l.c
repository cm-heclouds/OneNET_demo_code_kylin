#include "stm32f10x.h"
#include "stm32f10x_i2c.h"
#include "hal_i2c.h"
#include "stdio.h"
#include "hmc5883l.h"
#include "utils.h"
#define HMC5883L_ADDR 0x1e

/* 从HMC5883L的指定地址开始读出一个字节 */
uint8_t HMC5883L_ReadByte(uint8_t ReadAddr)
{
    uint8_t ret = 0;
    Hal_I2C_ByteRead(I2C2, HMC5883L_ADDR,  ReadAddr, &ret);
    return ret;
}

/* 向HMC5883L的指定地址开始写入一个字节 */
void HMC5883L_WriteByte(uint8_t WriteAddr, uint8_t *Data)
{
    Hal_I2C_ByteWrite(I2C2, HMC5883L_ADDR, WriteAddr, Data);
}

void HMC5883L_Init(void)
{
    uint8_t val = 0;
    val = 0x70;
    HMC5883L_WriteByte(HMC58X3_R_CONFA, &val); /*配置寄存器 A*/
    val = 0xA0;
    HMC5883L_WriteByte(HMC58X3_R_CONFB, &val); /*配置寄存器 B*/
    val = 0;
    HMC5883L_WriteByte(HMC58X3_R_MODE, &val); /*配置模式寄存器，连续测量模式，参见手册*/
}
/**
  * @brief  获取HMC5883L传感器数值，各方向LSB RAW数据.
  * @param  data_out:存储传感器采集结果
  * @retval None
  **/
void HMC5883L_GetXYZ(int16_t data_out[3])
{
    uint8_t data[6];
    HMC5883L_Init();
    mDelay(200);
    Hal_I2C_MutiRead(I2C2, data, HMC5883L_ADDR, 0x3, 6);
    mDelay(10);
    data_out[0] = (int16_t)(data[1] + ((uint16_t)data[0] << 8));
    data_out[1] = (int16_t)(data[3] + ((uint16_t)data[1] << 8));
    data_out[2] = (int16_t)(data[5] + ((uint16_t)data[2] << 8));

    if(data_out[0] < 0)
    {
        data_out[0] = -data_out[0];
        printf("%s  x %d\n", __func__, data_out[0]);
    }
    if(data_out[1] < 0)
    {
        data_out[1] = -data_out[1];
        printf("%s z  %d\n", __func__, data_out[1]);
    }
    if(data_out[2] < 0)
    {
        data_out[2] = -data_out[2];
        printf("%s y %d\n", __func__, data_out[2]);
    }

    printf("HMC5883L ******X=0x%x,Z=0x%x,y=0x%x*****\n", (int16_t)data_out[0], (int16_t)data_out[1], (int16_t)data_out[2]);
    printf("status reg:0x%x\n", HMC5883L_ReadByte(9));
}
