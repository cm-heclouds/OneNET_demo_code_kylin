#include "stm32f10x.h"
#include "stm32f10x_i2c.h"
#include "hal_i2c.h"
#include "stdio.h"
#include "at24c02.h"

/* 从AT24CXX的指定地址开始读出一个字节 */
uint8_t AT24CXX_ReadByte(uint8_t ReadAddr)
{
    uint8_t ret = 0;
    Hal_I2C_ByteRead(I2C2, AT24CX_ADDR, ReadAddr, &ret);
    return ret;
}

/* 向AT24CXX的指定地址开始写入一个字节 */
void AT24CXX_WriteByte(uint8_t WriteAddr, uint8_t Data)
{
    Hal_I2C_ByteWrite(I2C2, AT24CX_ADDR, WriteAddr, &Data);
}

void AT24CXX_loop(void)
{
    uint32_t i;
    uint8_t buf[256] = {0};

    Hal_I2C_Init();
    printf("%s %d\n", __func__, __LINE__);
    for(i = 0; i < 255; i++)
    {
        AT24CXX_WriteByte(i, (u8)((i + 255) % 255));
        printf("i:%d val=:%d\n", i, (u8)((i + 255) % 255));
        /* NOTE: 根据datasheet，两次字节写之间必须延迟5ms，也就是stop和start之间的间隔时间*/
        mDelay(30);
    }
    printf("Write completed \r\n");
    printf("*********\r\n");
    for(i = 0; i < 255; i++)
    {
        printf("i:%d val=:%d\n", i, (u8)AT24CXX_ReadByte(i));
        mDelay(30);
    }
    printf("\r\n");
}