#include "stm32f10x.h"
#include "stdio.h"
#include "hal_i2c.h"
#include "utils.h"
#define ADXL345_ADDRESS 0x53
void ADXL345_init(void)
{
    uint8_t data_fmt_val = 0;
    uint8_t pw_ctl = 0x28, ret = 0;
    uint8_t int_en = 0x00;
    uint8_t devid;

    //0 ±16g，13位模式
#define DATA_FORMAT_REG  0x31
    //0x08 测量模式
#define POWER_CTL                0x2d
    //0x80 使能DATA_READY中断,需要吗，需要禁止的吧。
#define INT_ENABLE       0x2e
#define BW_RATE 0x2c
#define OFSX 0x1e
#define OFSY 0x1f
#define OFSZ 0x20
    mDelay(20);
    Hal_I2C_ByteRead(I2C2, ADXL345_ADDRESS, 0x00, &devid);
    printf("\r\ndevice ID=%x,\r\n", devid);
#if 0
    mDelay(500);
    Hal_I2C_ByteWrite(I2C2, ADXL345_ADDRESS, POWER_CTL, &pw_ctl); //链接使能,测量模式
    mDelay(2000);
#endif
    ret = 0X2B;
    Hal_I2C_ByteWrite(I2C2, ADXL345_ADDRESS, DATA_FORMAT_REG, &ret); //低电平中断输出,13位全分辨率,输出数据右对齐,16g量程
    ret = 0x0A;
    Hal_I2C_ByteWrite(I2C2, ADXL345_ADDRESS, BW_RATE, &ret); //数据输出速度为100Hz
    ret = 0x28;
    Hal_I2C_ByteWrite(I2C2, ADXL345_ADDRESS, POWER_CTL, &ret);    //链接使能,测量模式
    ret = 0x00;
    Hal_I2C_ByteWrite(I2C2, ADXL345_ADDRESS, INT_ENABLE, &ret);     //不使用中断
    ret = 0x00;
    Hal_I2C_ByteWrite(I2C2, ADXL345_ADDRESS, OFSX, &ret);
    ret = 0x00;
    Hal_I2C_ByteWrite(I2C2, ADXL345_ADDRESS, OFSY, &ret);
    ret = 0x00;
    Hal_I2C_ByteWrite(I2C2, ADXL345_ADDRESS, OFSZ, &ret);
    mDelay(500);
}
uint8_t data[6];
/**
  * @brief  获取ADXL345传感器数值，各方向LSB RAW数据.
  * @param  data_out:存储传感器采集结果
  * @retval None
  **/
void ADXL345_GETXYZ(int16_t data_out[3])
{
    float f1, f2, f3;
    ADXL345_init(); //每次读写寄存器之前都要先读device ID

    Hal_I2C_MutiRead(I2C2, data, ADXL345_ADDRESS, 0x32, 6);
    data_out[0] = (int16_t)(data[0] + ((uint16_t)data[1] << 8));
    data_out[1] = (int16_t)(data[2] + ((uint16_t)data[3] << 8));
    data_out[2] = (int16_t)(data[4] + ((uint16_t)data[5] << 8));
#if 0
    if(data_out[0] < 0)
    {
        data_out[0] = -data_out[0];
        f1 = (float)(data_out[0] * 3.9);
        printf("%s f1  -%f\n", __func__, f1);
    }
    if(data_out[1] < 0)
    {
        data_out[1] = -data_out[1];
        f2 = (float)(data_out[1] * 3.9);
        printf("%s f2 -%f\n", __func__, f2);
    }
    if(data_out[2] < 0)
    {
        data_out[2] = -data_out[2];
        f3 = (float)(data_out[2] * 3.9);
        printf("%s f3- %f\n", __func__, f3);
    }
#endif
    printf("ADXL345 ******X=%d,Y=%d,Z=%d*****\n", data_out[0], data_out[1], data_out[2]);
}
