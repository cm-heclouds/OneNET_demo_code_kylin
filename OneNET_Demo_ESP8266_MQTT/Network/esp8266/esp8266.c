#include "esp8266.h"

uint32_t ESP8266_SendData(int8_t * buf, uint32_t len);

/**
  * @brief  初始化ESP8266，并配置路由器和连接服务器
  * @param  server:按AT命令配置的服务器地址和端口字符串
    * @param  ssid_pwd: 按AT命令配置的路由器SSID和密码
  * @retval NONE
  **/

void ESP8266_Init(int8_t * server, int8_t * ssid_pwd)
{
    SendCmd(AT, "OK", 1000); /*进入AT命令模式*/
    SendCmd(CWMODE, "OK", 1000);
    SendCmd(RST, "OK", 2000);   /*重置模组式*/
    SendCmd(CIFSR, "OK", 1000); /*查询设备IP，可放在需要的位置*/
    SendCmd(ssid_pwd, "OK", 2000);  /*配置要连接的路由器SSID和密码*/
    SendCmd(server, "OK", 2000); /*与服务器建立TCP连接*/
    SendCmd(CIPMODE, "OK", 1000); /*透传模式*/
    memset(usart2_rcv_buf, 0, strlen((const char *)usart2_rcv_buf)); /*先清除接收缓冲区*/
    usart2_rcv_len = 0;
    USART2_Write(USART2, CIPSEND, strlen(CIPSEND)); /*开始透传*/
    mDelay(500);
    if((NULL != strstr((const char *)usart2_rcv_buf, "ERROR")))
    {
        return;
    }
}

/**
  * @brief  通过USART2连接ESP8266发送数据.
  * @param  buf: 要发送的数据缓冲区地址
    * @param  len: 要发送的数据缓长度
  * @retval 发送的数据长度
  **/
uint32_t ESP8266_SendData(int8_t * buf, uint32_t len)
{
    memset(usart2_rcv_buf, 0, strlen((const char *)usart2_rcv_buf));
    usart2_rcv_len = 0;
    USART2_Write(USART2, (uint8_t*)buf, len);
    mDelay(100);
		return len;
}

/**
  * @brief  通过USART2发送数据.
  * @param  buf: 要发送的数据缓冲区地址
    * @param  len: 要发送的数据缓长度
  * @retval 发送的数据长度
  **/
uint32_t USART2_SendData(int8_t * buf, uint32_t len)
{
    ESP8266_SendData(buf, len);
    return len;
}
