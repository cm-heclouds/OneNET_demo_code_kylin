#include "stm32f10x.h"
#include "stdio.h"
#include "hal_i2c.h"
#include "stm32f10x.h"
#include "stdio.h"
#include "hal_i2c.h"
#include "sht20.h"
#include "utils.h"

const int16_t POLYNOMIAL = 0x131;
int8_t SHT2x_CheckCrc(int8_t data[], int8_t nbrOfBytes, int8_t checksum);
float SHT2x_CalcTemperatureC(int16_t u16sT);
float SHT2x_CalcRH(uint16_t u16sRH);

/**
  * @brief  reset SHT20模块.
  * @param  None
  * @retval None
  **/
void SHT20_reset(void)
{
    //printf("%s %d\n",__func__,__LINE__);
    Hal_I2C_ByteWrite(I2C2, SHT20_ADDRESS, SHT20_SOFT_RESET, NULL);
}

/**
  * @brief  读取 SHT20模块的用户寄存器
  * @param  None
  * @retval 返回用户寄存器值
  **/
uint8_t  SHT20_read_user_reg(void)
{
    uint8_t val = 0;
    Hal_I2C_ByteRead(I2C2, SHT20_ADDRESS, SHT20_READ_REG, &val);
    return val;
}

/**
  * @brief  写 SHT20模块的用户寄存器，写之前先读
  * @param  None
  * @retval None
  **/
void SHT20_write_user_reg(uint8_t val)
{
    Hal_I2C_ByteWrite(I2C2, SHT20_ADDRESS, SHT20_WRITE_REG, &val);
}

/**
  * @brief  保持模式读SHT20模块温度或湿度
  * @param  cmd：SHT20_Measurement_RH_HM or SHT20_Measurement_T_HM
  * @param  pMeasurand：返回结果
  * @retval ret:0,成功，<0 失败
  **/
int8_t SHT2x_MeasureHM(uint8_t cmd, uint16_t *pMeasurand)
{
    int8_t  checksum, addr;  //checksum
    uint8_t  data[2];    //data array for checksum verification
    uint16_t tmp;
    float t;
    //start
    addr = (int8_t)(SHT20_ADDRESS << 1);
    I2C_AcknowledgeConfig(I2C2, ENABLE);

    I2C_GenerateSTART(I2C2, ENABLE);      /**********start**********/
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C2, addr, I2C_Direction_Transmitter); /*******device addr********/
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    //send cmd
    I2C_SendData(I2C2, cmd);    /*****reg addr**********/
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    //start again
    mDelay(150);
    I2C_GenerateSTART(I2C2, ENABLE);      /**********start**********/
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));
    //send read
    mDelay(150);
    I2C_Send7bitAddress(I2C2, addr, I2C_Direction_Receiver); /*******device addr********/
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    mDelay(300);//wait enough，下次改成SCL超时查询的方式
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
    data[0] = I2C_ReceiveData(I2C2);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
    data[1] = I2C_ReceiveData(I2C2);

    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
    checksum = I2C_ReceiveData(I2C2);

    //I2C_AcknowledgeConfig(I2C2, DISABLE);//如果调用了，那么crc无效
    I2C_GenerateSTOP(I2C2, ENABLE);   /********stop******/
    SHT2x_CheckCrc((int8_t*)data, 2, checksum);
    tmp = (data[0] << 8) + data[1];
    if(cmd == SHT20_Measurement_T_HM)
    {
        t = SHT2x_CalcTemperatureC(tmp);
        printf("%s data[0]=%d,data[1]=%d,checksum=%d,t=%f\n", __func__, data[0], data[1], SHT2x_CheckCrc((int8_t*)data, 2, checksum), t);
    }
    else
    {
        t = SHT2x_CalcRH(tmp);
        printf("%s data[0]=%d,data[1]=%d,checksum=%d,rh=%f%\n", __func__, data[0], data[1], SHT2x_CheckCrc((int8_t*)data, 2, checksum), t);
    }
    if(pMeasurand)
    {
        *pMeasurand = (uint16_t)t;
    }
    printf("%s %d\n", __func__, *pMeasurand);
    return 0;
}
#if 0
/**
  * @brief  非保持模式读SHT20模块温度或湿度
  * @param  cmd：SHT20_Measurement_RH_NHM or SHT20_Measurement_T_NHM
  * @param  pMeasurand：返回结果
  * @param  timeout：超时次数
  * @retval ret:0,成功，<0 失败
  **/
int8_t SHT2x_MeasurePoll(uint8_t cmd, uint16_t *pMeasurand, int32_t timeout)
{
    uint8_t  checksum, addr;  //checksum
    uint8_t  data[2];    //data array for checksum verification
    int8_t  error = 0;  //error variable
    int16_t i = 0;      //counting variable
    float t;
    int16_t tmp;
    int32_t tmp_time;
    addr = SHT20_ADDRESS << 1;
    I2C_AcknowledgeConfig(I2C2, ENABLE);
    I2C_GenerateSTART(I2C2, ENABLE);      /**********start**********/
    uDelay(500);
    while((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)));
    I2C_Send7bitAddress(I2C2, addr, I2C_Direction_Transmitter); /*******device addr********/
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData(I2C2, cmd);    /*****reg addr**********/
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTOP(I2C2, ENABLE);   /********stop******/
    I2C_AcknowledgeConfig(I2C2, ENABLE);
    do
    {
        I2C_GenerateSTART(I2C2, ENABLE);      /**********start**********/
        while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));
        if(i++ >= 200)
        {
            break;
        }
        I2C_Send7bitAddress(I2C2, addr, I2C_Direction_Receiver); /*******device addr********/
        while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));//必须等待才行，不过也要看运气
        mDelay(1500);//延迟要足够才行
        if(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED))//I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED
        {
            continue;
        }
        else
        {
            break;
        }
    }
    while((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)));
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
    if (i >= 200)
    {
        printf("timeout\n");
        goto timeout_l;
    }
    //-- read two data bytes and one checksum byte --
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
    data[0] = I2C_ReceiveData(I2C2);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
    data[1] = I2C_ReceiveData(I2C2);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
    checksum = I2C_ReceiveData(I2C2);;
    //-- verify checksum --
    error |= SHT2x_CheckCrc (data, 2, checksum);
    I2C_GenerateSTOP(I2C2, ENABLE);   /********stop******/
    tmp = (data[0] << 8) + data[1];
    if(cmd == SHT20_Measurement_T_HM)
    {
        t = SHT2x_CalcTemperatureC(tmp);
        printf("%s data[0]=%d,data[1]=%d,checksum=%d,t=%f\n", __func__, data[0], data[1], SHT2x_CheckCrc(data, 2, checksum), t);
    }
    else
    {
        t = SHT2x_CalcRH(tmp);
        printf("%s data[0]=%d,data[1]=%d,checksum=%d,rh=%f%\n", __func__, data[0], data[1], SHT2x_CheckCrc(data, 2, checksum), t);
    }
    if(pMeasurand)
    {
        *pMeasurand = t;
    }
    return 0;
timeout_l:
    return -1;
}
#endif
void SHT20_loop(void)
{
    unsigned char val = 0x3;
    unsigned char data[18];
    printf("%s %d\n", __func__, __LINE__);
    while(1)
    {
        Hal_I2C_Init();
        SHT20_read_user_reg();
        mDelay(200);//延迟，设备没有那么快的响应时间，否则总线处理忙等待
        SHT2x_MeasureHM(SHT20_Measurement_T_HM, NULL);
        mDelay(1000);
        //SHT2x_MeasurePoll(SHT20_Measurement_RH_HM,NULL,1000);//没有调试
        mDelay(1500);
        //SHT2x_MeasurePoll(SHT20_Measurement_T_HM,NULL,1000);//没有调试
        mDelay(1500);
        SHT2x_MeasureHM(SHT20_Measurement_RH_HM, NULL);
        mDelay(1500);
        SHT2x_GetSerialNumber(data);
        mDelay(1500);
        SHT20_read_user_reg();
        mDelay(1500);
        SHT20_write_user_reg(val);
        mDelay(1500);
        SHT20_read_user_reg();
        mDelay(1500);
        SHT20_reset();
        mDelay(1500);
    }
}

int8_t SHT2x_CheckCrc(int8_t data[], int8_t nbrOfBytes, int8_t checksum)
{
    int8_t crc = 0;
    int8_t bit;
    int8_t byteCtr;
    //calculates 8-Bit checksum with given polynomial
    for (byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr)
    {
        crc ^= (data[byteCtr]);
        for ( bit = 8; bit > 0; --bit)
        {
            if (crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
            else crc = (crc << 1);
        }
    }
    if (crc != checksum) return 1;
    else return 0;
}

float SHT2x_CalcTemperatureC(int16_t u16sT)
{
    float temperatureC;            // variable for result

    u16sT &= ~0x0003;           // clear bits [1..0] (status bits)
    //-- calculate temperature [癈] --
    temperatureC = -46.85 + 175.72 / 65536 * (float)u16sT; //T= -46.85 + 175.72 * ST/2^16
    return temperatureC;
}


float SHT2x_CalcRH(uint16_t u16sRH)
{
    float humidityRH;              // variable for result
    u16sRH &= ~0x0003;          // clear bits [1..0] (status bits)
    //-- calculate relative humidity [%RH] --
    //humidityRH = -6.0 + 125.0/65536 * (float)u16sRH; // RH= -6 + 125 * SRH/2^16
    humidityRH = ((float)u16sRH * 0.00190735) - 6;
    return humidityRH;
}

/**
  * @brief  读SHT20模块序列号
  * @param  SerialNumber：8字节数据，用于存储返回的序列号
  * @retval ret:0,成功，<0 失败
  **/
int8_t SHT2x_GetSerialNumber(uint8_t SerialNumber[])
{
    int8_t  error = 0, addr, i = 0;                    //error variable
    //Read from memory location 1
    addr = (int8_t)(SHT20_ADDRESS << 1);
    I2C_AcknowledgeConfig(I2C2, ENABLE);

    I2C_GenerateSTART(I2C2, ENABLE);      /**********start**********/
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C2, addr, I2C_Direction_Transmitter); /*******device addr********/
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2C2, 0xFA);    /*****reg addr**********/
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_SendData(I2C2, 0x0F);    /*****reg addr**********/
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    mDelay(100);
    I2C_GenerateSTART(I2C2, ENABLE);      /**********start**********/
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C2, addr, I2C_Direction_Receiver); /*******device addr********/
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
    SerialNumber[5] = I2C_ReceiveData(I2C2);    /***get data****/  ; //Read SNB_3

    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
    I2C_ReceiveData(I2C2);                    //Read CRC SNB_3 (CRC is not analyzed)

    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
    SerialNumber[4] = I2C_ReceiveData(I2C2); ; //Read SNB_2

    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
    I2C_ReceiveData(I2C2);                      //Read CRC SNB_2 (CRC is not analyzed)

    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
    SerialNumber[3] = I2C_ReceiveData(I2C2); ; //Read SNB_1

    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
    I2C_ReceiveData(I2C2);                     //Read CRC SNB_1 (CRC is not analyzed)

    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
    SerialNumber[2] = I2C_ReceiveData(I2C2); ; //Read SNB_0

    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
    I2C_ReceiveData(I2C2);                  //Read CRC SNB_0 (CRC is not analyzed)

    I2C_AcknowledgeConfig(I2C2, DISABLE);  //这个非常关键，否则时序不对，必须NACK，否则接下来的数据还是这个mem location的。
    I2C_GenerateSTOP(I2C2, ENABLE);   /********stop******/
    mDelay(100);

    //Read from memory location 2
    I2C_GenerateSTART(I2C2, ENABLE);      /**********start**********/
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C2, addr, I2C_Direction_Transmitter); /*******device addr********/
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData(I2C2, 0xFC);    /*****reg addr**********/
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_SendData(I2C2, 0xC9);    /*****reg addr**********/
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    mDelay(100);

    I2C_GenerateSTART(I2C2, ENABLE);      /**********start**********/
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C2, addr, I2C_Direction_Receiver); /*******device addr********/
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
    SerialNumber[1] = I2C_ReceiveData(I2C2); //Read SNC_1

    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
    SerialNumber[0] = I2C_ReceiveData(I2C2); //Read SNC_0

    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
    I2C_ReceiveData(I2C2);                   //Read CRC SNC0/1 (CRC is not analyzed)

    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
    SerialNumber[7] = I2C_ReceiveData(I2C2); //Read SNA_1

    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
    SerialNumber[6] = I2C_ReceiveData(I2C2); //Read SNA_0

    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
    I2C_ReceiveData(I2C2);                //Read CRC SNA0/1 (CRC is not analyzed)

    I2C_AcknowledgeConfig(I2C2, DISABLE);
    I2C_GenerateSTOP(I2C2, ENABLE);   /********stop******/

    printf("%s %d u8SerialNumber: \n", __func__, __LINE__);
    for(i = 0; i < 8; i++)
    {
        printf("0x%x ", SerialNumber[i]);
    }
    printf("\n");
    return error;
}
