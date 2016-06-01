#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_exti.h"
#include "misc.h"
#include "usart2.h"
#include "stm32f10x.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "utils.h"
#include "usart2.h"
#include "esp8266.h"
#include "modbus.h"

int8_t req[52];

void Process_Cmd(uint8_t * buf, int32_t len);
void MODBUS_Init_Login_Data(void);
void MODBUS_Login(void);
void Insert_Crc16(uint8_t *buf, int32_t len_to_calc);
void Read_HoldingRegs(uint8_t * buf, uint8_t * rsp_buf, int32_t *len);

/*
 *初始化modbus鉴权数据，共52字节。
 */
void MODBUS_Init_Login_Data(void)
{
    int32_t i = 0;
    int8_t type[11] = "type";/*暂时不支持，服务器不处理*/
    int8_t name[9] = "name";/*暂时不支持，服务器不处理*/
    int8_t phone[12] = "12345670";/*创建MODBUS设备时对应的卡号信息，鉴权信息*/
    int8_t pwd[9] = "12345670";/*创建MODBUS设备时对应的密码信息，鉴权信息*/
    int8_t p_id[11] = "39484";/*设备所在项目ID*/
    memset(req, 0, 52);
    memcpy(req, type, 11);
    memcpy(req + 11, name, 9);
    memcpy(req + 11 + 9, phone, 12);
    memcpy(req + 11 + 9 + 12, pwd, 9);
    memcpy(req + 11 + 9 + 12 + 9, p_id, 11);

    for(i = 0; i < 52; i++)
    {
        printf("0x%x ", req[i]);
    }
    printf("\n");
}

/**
 * @brief  modbus协议主循环
 * @param  None
 * @retval None
 **/
void MODBUS_Loop(void)
{
    int32_t i = 0;
    int32_t rcv_len = 0;
    uint8_t buffer[MAXLINE];
    printf("MODBUS Login\r\n");
    /*1.modbus首先登陆，发送鉴权信息*/
    MODBUS_Login();
	  printf("\r\n");
    while (1)
    {
        /*查询WIFI是否接收到数据*/
        rcv_len = USART2_GetRcvNum();
        //USART2_GetRcvData(buffer, rcv_len);
        mDelay(1000);
        mDelay(1000);
        if (rcv_len <= 0)
        {
            mDelay(1000);
            continue;
        }
        else
        {		mDelay(50);
					  rcv_len = USART2_GetRcvNum();
            USART2_GetRcvData(buffer, rcv_len);
            printf("\r\nI GET DATA:\r\n");
            for (i = 0; i < rcv_len; i++)
            {
                printf("0x%x ", buffer[i]);
            }
            printf("\r\n");
            /*处理MODBUS协议命令*/
            Process_Cmd(buffer, rcv_len);
        }
    }
}

/**
 * @brief  modbus协议命令处理入口
 * @param  None
 * @retval None
 **/
void Process_Cmd(uint8_t * buf, int32_t len)
{
    uint8_t rsp_buf[MAXLINE];
    int32_t rsp_len = 0;
    uint8_t addr = *buf;
    uint8_t func_code = *(buf + 1);

    printf("addr:0x%x,func_code: 0x%x\n", addr, func_code);

    if(addr != MY_MODBUS_ADDR)
    {
        printf("error addr\n");/*非本设备地址*/
        return;
    }
    switch (func_code)
    {
        /*处理0x3命令*/
        case MB_FUNC_READ_HOLDING:
            Read_HoldingRegs(buf, rsp_buf, &rsp_len);
            break;
        default:
            Read_HoldingRegs(buf, rsp_buf, &rsp_len);
            break;
    }
    /*返回命令响应数据*/
    USART2_SendData((int8_t*)rsp_buf, rsp_len);
    printf("%s send  %d bytes to server\n", __func__, rsp_len);
}


/*
 * @brief 处理0x3命令，按照modbus协议，是读取保持寄存器
 * @param  buf：需要处理的协议数据缓冲区
 * @param  rsp_buf：返回结果缓冲区
 * @param  len：需要处理的协议数据缓冲区长度
 * @retval None
 */

void Read_HoldingRegs(uint8_t * buf, uint8_t * rsp_buf, int32_t * len)
{
    int32_t i = 0;
    uint16_t start_addr = (buf[2] << 8) + buf[3];
    uint8_t read_reg_len = ((buf[4] << 4) & 0xf0) + (buf[5] & 0x0f);

    rsp_buf[0] = buf[0]; //copy addr;
    rsp_buf[1] = buf[1]; //copy func code;
    rsp_buf[2] = read_reg_len * 2;

    printf("from addr 0x%x,%d bytes to read\n", start_addr, read_reg_len);

    if(read_reg_len == 3)
    {
        //user data  in modbus protocol;
        rsp_buf[i + 3] = (unsigned char)rand();
        rsp_buf[i + 4] = (unsigned char)rand();
        rsp_buf[i + 5] = (unsigned char)rand();
        rsp_buf[i + 6] = (unsigned char)rand();
        rsp_buf[i + 7] = (unsigned char)rand();
        rsp_buf[i + 8] = (unsigned char)rand();
    }
    else
    {
        for(i = 0; i < (read_reg_len * 2); i++)
        {
            rsp_buf[i + 3] = 1; //(unsigned char)rand();
        }
    }
    Insert_Crc16(rsp_buf, 1 + 1 + 1 + read_reg_len * 2);
    *len = 1 + 1 + 1 + read_reg_len * 2 + 2; //1 byte addr,1 byte func code,1 byte reg_val_len,reg data len,2 bytes crc16;
}

/*
 * @brief modbus登陆鉴权
 * @param  None
 * @retval None
 */
void MODBUS_Login(void)
{
    MODBUS_Init_Login_Data();
    USART2_SendData(req, sizeof(req));
}

/*
 * @brief 按照modbus协议要求插入CRC16校验
 * @param  buf:需要计算crc的数据缓冲区，并保留了crc16的数据空间，以便存储计算的校验值
 * @param  len_to_calc:buf中实际需要计算crc的长度
 * @retval None
 */
void Insert_Crc16(uint8_t *buf, int32_t len_to_calc)
{
    uint8_t test_buf[256];//only for test
    uint8_t * tmp = test_buf;
    int32_t i = 0;
    uint16_t uscrc = usMBCRC16(buf, len_to_calc);
    buf[len_to_calc + 0] = uscrc & 0xff;
    buf[len_to_calc + 1] = (uscrc >> 8) & 0xff; /*make sure buf big enough to accept crc16*/
    printf("crc:0x%2x\n", uscrc);

    for(i = 0; i < len_to_calc + 2; i++)
    {
        printf("0x%x ", buf[i]);
    }
    printf("\n");
    i = 0;

    while (i < len_to_calc + 2)
    {
        *tmp++ = prvucMBBIN2CHAR((buf[i]) >> 4);    //high byte
        *tmp++ = prvucMBBIN2CHAR((buf[i]) & 0x0F);  //low byte
        i++;
    }
    printf("send data rsp:%s,%d\n", test_buf, strlen((const char *)test_buf));
}
