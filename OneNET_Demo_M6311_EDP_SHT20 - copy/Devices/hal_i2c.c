#include "stm32f10x_i2c.h"
#include "hal_i2c.h"
#include "utils.h"
#include <stdio.h>

#define I2C_Speed              200000
#define I2C_SLAVE_ADDRESS7    0

void GPIO_I2C_Configuration(I2C_TypeDef* I2Cx);
void I2C_Configuration(I2C_TypeDef* I2Cx);

/**
  * @brief  I2C 使用的GPIO配置
  * @param  I2Cx：选择I2C接口，I2C1，I2C2
  * @retval None
  **/
void GPIO_I2C_Configuration(I2C_TypeDef* I2Cx)
{
#ifdef STM32_PHER_I2C1
    if(I2Cx == I2C1)
    {
        GPIO_InitTypeDef  GPIO_I2C1_InitStructure;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

        GPIO_I2C1_InitStructure.GPIO_Pin =  STM32_PHER_I2C1_SDA | STM32_PHER_I2C1_SCL;
        GPIO_I2C1_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_I2C1_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
        GPIO_Init(GPIOB, &GPIO_I2C1_InitStructure);
    }
#endif

#ifdef STM32_PHER_I2C2
    if(I2Cx == I2C2)
    {
        GPIO_InitTypeDef  GPIO_I2C2_InitStructure;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        printf("%s %d\n", __func__, __LINE__);
        GPIO_I2C2_InitStructure.GPIO_Pin =  STM32_PHER_I2C2_SDA | STM32_PHER_I2C2_SCL;
        GPIO_I2C2_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_I2C2_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
        GPIO_Init(GPIOB, &GPIO_I2C2_InitStructure);
    }
#endif
}
/**
  * @brief  I2C控制器配置
  * @param  I2Cx：选择I2C接口，I2C1，I2C2
  * @retval None
  **/
void I2C_Configuration(I2C_TypeDef* I2Cx)
{
#ifdef STM32_PHER_I2C1
    if(I2Cx == I2C1)
    {
        I2C_InitTypeDef  I2C_InitStructure;
        I2C_DeInit(I2Cx);
        I2C_Cmd(I2Cx, DISABLE);
        //printf("%s %d\n",__func__,__LINE__);
        /* I2C configuration */
        I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
        I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
        I2C_InitStructure.I2C_OwnAddress1 = I2C_SLAVE_ADDRESS7;
        I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
        I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
        I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
        I2C_Cmd(I2Cx, ENABLE);
        I2C_Init(I2Cx, &I2C_InitStructure);
        I2C_AcknowledgeConfig(I2Cx, ENABLE);
    }
#endif

#ifdef STM32_PHER_I2C2
    if(I2Cx == I2C2)
    {
        I2C_InitTypeDef  I2C_InitStructure;
        //printf("%s %d\n",__func__,__LINE__);
        I2C_DeInit(I2Cx);
        I2C_Cmd(I2Cx, DISABLE);
        /* I2C configuration */
        I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
        I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
        I2C_InitStructure.I2C_OwnAddress1 = I2C_SLAVE_ADDRESS7;
        I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
        I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
        I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;

        I2C_Cmd(I2Cx, ENABLE);
        I2C_Init(I2Cx, &I2C_InitStructure);
        I2C_AcknowledgeConfig(I2Cx, ENABLE);
    }
#endif
}
/**
  * @brief  初始化所使用的I2C接口
  * @param  None
  * @retval None
  **/
void Hal_I2C_Init()
{
    printf("%s %d\n", __func__, __LINE__);
    /* GPIO configuration first must*/
#ifdef STM32_PHER_I2C1
    GPIO_I2C_Configuration(I2C1);
    /* I2C configuration */
    I2C_Configuration(I2C1);
#endif

#ifdef STM32_PHER_I2C2
    GPIO_I2C_Configuration(I2C2);
    /* I2C configuration */
    I2C_Configuration(I2C2);
#endif
    Check_I2C_Device_Ready();
}
/**
  * @brief  I2C写数据
  * @param  I2Cx：I2C1，I2C2，选择接口号
  * @param  i2c_slave_addr：从设备地址
  * @param  RegAddr：需要写入数据的寄存器地址
  * @param  data：需要写入的寄存器值
  * @retval ret:0,操作成功，-1，失败
  **/
int8_t Hal_I2C_ByteWrite(I2C_TypeDef* I2Cx, uint16_t i2c_slave_addr, uint8_t regAddr, uint8_t *data)
{
    uint16_t tempADD;

    tempADD = i2c_slave_addr << 1;
    I2C_AcknowledgeConfig(I2Cx, ENABLE);
    I2C_GenerateSTART(I2Cx, ENABLE);           //*****start  *****//
    /* Test on EV5 and clear it */
    while((!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)));
    I2C_Send7bitAddress(I2Cx, tempADD, I2C_Direction_Transmitter);  /***device addr*/
    while((!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)));

    I2C_Cmd(I2Cx, ENABLE);
    I2C_SendData(I2Cx, regAddr);   /*send reg to write *************/
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    if(data)
    {
        /* Send the byte to be written */
        I2C_SendData(I2Cx, *data);   /******send data*********/
        while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }
    /* Send STOP condition */
    I2C_GenerateSTOP(I2Cx, ENABLE);  /******stop****/
    return 0;
}
/**
  * @brief  I2C读数据
  * @param  I2Cx：I2C1，I2C2，选择接口号
  * @param  i2c_slave_addr：从设备地址
  * @param  RegAddr：需要读取数据的寄存器地址
  * @param  val：保存I2C读取返回值
  * @retval ret:0,操作成功，-1，失败
  **/
int8_t Hal_I2C_ByteRead(I2C_TypeDef* I2Cx, uint16_t i2c_slave_addr, uint8_t regAddr, uint8_t *val)
{
    uint16_t addr;
    uint8_t rData;

    addr = i2c_slave_addr << 1;
    I2C_AcknowledgeConfig(I2Cx, ENABLE);

    I2C_GenerateSTART(I2Cx, ENABLE);      /**********start**********/
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2Cx, addr, I2C_Direction_Transmitter); /*******device addr********/
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2Cx, regAddr);    /*****reg addr**********/
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTART(I2Cx, ENABLE); /***restart *************/
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))  /* Test on EV5 and clear it */
        I2C_Send7bitAddress(I2Cx, addr, I2C_Direction_Receiver);    /*******device addr******/
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)); /* Test on EV6 and clear it */

    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)); /* EV7 */
    rData = I2C_ReceiveData(I2Cx);   /***get data****/
    I2C_AcknowledgeConfig(I2Cx, DISABLE);
    I2C_GenerateSTOP(I2Cx, ENABLE);   /********stop******/
    *val = rData;
    return 0;
}
/**
  * @brief  I2C一次读多个数据
  * @param  I2Cx：I2C1，I2C2，选择接口号
  * @param  buf：存储返回数据的缓冲区
  * @param  RegAddr：需要读取数据的寄存器地址
  * @param  i2c_slave_addr：从设备地址
  * @param  num：读取字节数
  * @retval ret:0,操作成功，-1，失败
  **/
int8_t Hal_I2C_MutiRead(I2C_TypeDef* I2Cx, uint8_t* buf, uint8_t i2c_slave_addr, uint8_t regAddr, uint8_t num)
{
    uint8_t addr;
    I2C_AcknowledgeConfig(I2Cx, ENABLE);

    /* Send START condition */
    I2C_GenerateSTART(I2Cx, ENABLE);/***start *******/

    addr = i2c_slave_addr << 1;

    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));/* Test on EV5 and clear it */

    I2C_Send7bitAddress(I2Cx, addr, I2C_Direction_Transmitter); /***device addr**/

    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); /* Test on EV6 and clear it */
    I2C_SendData(I2Cx, regAddr);   /* send reg addr */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)); /* Test on EV8 and clear it */

    I2C_GenerateSTART(I2Cx, ENABLE); /*restart  */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));  /* Test on EV5 and clear it */

    I2C_Send7bitAddress(I2Cx, addr, I2C_Direction_Receiver);  /* re send device addr */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) ; /* Test on EV6 and clear it */
    /* While there is data to be read */
    while(num)
    {
        if(num == 1)
        {
            /*the last one*/
            I2C_AcknowledgeConfig(I2Cx, DISABLE);/* Disable Acknowledgement */
            I2C_GenerateSTOP(I2Cx, ENABLE);/* Send STOP Condition */
        }

        /* Test on EV7 and clear it */
        if(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
        {
            /* Read a byte from the EEPROM */
            *buf = I2C_ReceiveData(I2Cx);
            buf++; /* Point to the next location where the byte read will be saved */
            num--;        /* Decrement the read bytes counter */
        }
    }
    /* Enable Acknowledgement to be ready for another reception */
    I2C_AcknowledgeConfig(I2Cx, ENABLE);
    return 0;
}
/**
  * @brief  用户自行实现，用于检测I2C设备是否准备好，否则继续调用I2C可能会陷入死循环或其他错误状态
  * @param  None
  * @retval None
  **/
void Check_I2C_Device_Ready(void)
{
    printf("%s\n you must call this func first to make sure your dev exist,or I2C will loop in while(wait) \n", __func__);
}


