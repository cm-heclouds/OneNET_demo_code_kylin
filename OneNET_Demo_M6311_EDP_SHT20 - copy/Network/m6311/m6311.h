#include "stm32f10x.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "usart1.h"
#include "usart2.h"
#include "EdpKit.h"
#include "utils.h"

#define  AT                 "AT\r\n"
#define  CPIN               "AT+CPIN?\r\n"
#define  MODE               "AT+CIPMODE=0\r\n"
#define  CGCLASS            "AT+CGCLASS=\"B\"\r\n"
#define  CGDCONT            "AT+CGDCONT?\r\n"
#define  CGATT              "AT+CGATT=1\r\n"
#define  CIPCSGP            "AT+CIPCSGP=1,\"CMNET\"\r\n"
#define  CLPORT             "AT+CLPORT=\"TCP\",\"2000\"\r\n"
#define  CGACT             "AT+CGACT=1,1\r\n"
#define  CIPSTART           "AT+CIPSTART=\"TCP\",\"183.230.40.33\",\"80\"\r\n"
#define  CIPSEND            "AT+CIPSEND\r\n"
#define  CIPSTATUS          "AT+CIPSTATUS\r\n"
#define  CIPSHUT            "AT+CIPSHUT\r\n"

/**
  * @brief  初始化M6311，并配置路由器和连接服务器
  * @param  server:按AT命令配置的服务器地址和端口字符串
    * @param  ssid_pwd: 按AT命令配置的路由器SSID和密码
  * @retval NONE
  **/

void M6311_Init(void);
/**
  * @brief  通过USART2发送数据.
  * @param  buf: 要发送的数据缓冲区地址
    * @param  len: 要发送的数据缓长度
  * @retval 发送的数据长度
  **/
int32_t USART2_SendData(uint8_t * buf,uint32_t len);

