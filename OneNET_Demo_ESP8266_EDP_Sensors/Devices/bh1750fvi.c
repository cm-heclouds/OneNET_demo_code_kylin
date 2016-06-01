#include "stm32f10x.h"
#include "stm32f10x_i2c.h"
#include "hal_i2c.h"
#include "stdio.h"
#include "bh1750fvi.h"
#include "utils.h"

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

void BH1750_Init(void);
/* 从BH1750FVI的指定地址开始读出一个字节 */
uint8_t BH1750FVI_ReadByte(uint8_t ReadAddr)
{
    uint8_t ret = 0;
    Hal_I2C_ByteRead(I2C2, BH1750FVI_ADDR,  ReadAddr, &ret);
    return ret;
}

/* 向BH1750FVI的指定地址开始写入一个字节 */
void BH1750FVI_WriteByte(uint8_t WriteAddr, uint8_t *Data)
{
    Hal_I2C_ByteWrite(I2C2, BH1750FVI_ADDR, WriteAddr, Data);
}

void BH1750FVI_loop(void)
{
    BH1750_Init();
    printf("BH1750 %d\n", Read_BH1750());
}

/**
  * @brief  初始化传感器
  * @param  None
  * @retval None
  **/
void BH1750_Init(void)
{
    mDelay(100);
    BH1750FVI_WriteByte(BH1750_ON, NULL);    //power on
    mDelay(2000);
    BH1750FVI_WriteByte(BH1750_RSET, NULL); //clear
    mDelay(2000);
    BH1750FVI_WriteByte(BH1750_Con_High_RM, NULL); //连续H分辨率模式，至少120ms，之后自动断电模式
    mDelay(2000);
}

/**
  * @brief  获得传感器数值
  * @param  None
  * @retval 返回光照，单位lx
  **/
uint16_t Read_BH1750(void)
{
    uint8_t addr, num = 2;
    uint8_t data[2], *buf = data;
    uint16_t result = 0;
    float result_lx = 0;
    BH1750_Init();
    I2C_AcknowledgeConfig(I2C2, ENABLE);
    /* Send START condition */
    I2C_GenerateSTART(I2C2, ENABLE);/***start *******/
    addr = BH1750FVI_ADDR << 1;
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/* Test on EV5 and clear it */
    I2C_Send7bitAddress(I2C2, addr, I2C_Direction_Receiver); /***device addr**/
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)); /* Test on EV6 and clear it */

    while(num)
    {
        if(num == 1)
        {
            /*the last one*/
            I2C_AcknowledgeConfig(I2C2, DISABLE);/* Disable Acknowledgement */
            I2C_GenerateSTOP(I2C2, ENABLE);/* Send STOP Condition */
        }

        /* Test on EV7 and clear it */
        if(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED))
        {
            /* Read a byte from the EEPROM */
            *buf = I2C_ReceiveData(I2C2);
            buf++; /* Point to the next location where the byte read will be saved */
            num--;        /* Decrement the read bytes counter */
        }
    }
    /* Enable Acknowledgement to be ready for another reception */
    I2C_AcknowledgeConfig(I2C2, ENABLE);
    result = (uint16_t)((data[0] << 8) + data[1]); //合成数据，即光照数据

    result_lx = (float)result / 1.2;
    printf("BH1750FVI:%f %d\n", result_lx, (uint16_t)result_lx);
    return  (uint16_t)result_lx;
}

