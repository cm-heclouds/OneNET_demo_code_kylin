#ifndef __ESP8266_HEADER__
#define __ESP8266_HEADER__

#include "stm32f10x.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "usart1.h"
#include "usart2.h"
#include "utils.h"

#define AT          "AT\r\n"
#define CWMODE      "AT+CWMODE=3\r\n"
#define RST         "AT+RST\r\n"
#define CIFSR       "AT+CIFSR\r\n"
#define CWJAP       "AT+CWJAP=\"onenet\",\"\"\r\n"
#define CIPSTART    "AT+CIPSTART=\"TCP\",\"183.230.40.39\",876\r\n"
#define MODBUS_CIPSTART    "AT+CIPSTART=\"TCP\",\"183.230.40.42\",987\r\n"
#define MQTT_CIPSTART    "AT+CIPSTART=\"TCP\",\"183.230.40.39\",6002\r\n"
#define EDP_CIPSTART    "AT+CIPSTART=\"TCP\",\"183.230.40.39\",876\r\n"
#define CIPMODE     "AT+CIPMODE=1\r\n"
#define CIPSEND     "AT+CIPSEND\r\n"
#define CIPSTATUS   "AT+CIPSTATUS\r\n"

/**
  * @brief  通过USART2发送数据.
  * @param  buf: 要发送的数据缓冲区地址
    * @param  len: 要发送的数据缓长度
  * @retval 发送的数据长度
  **/
uint32_t USART2_SendData(int8_t * buf, uint32_t len);

/**
  * @brief  初始化ESP8266，并配置路由器和连接协议服务器，比如MODBUS,HTTP,EDP,MQTT服务器的IP地址和端口
  * @param  server:按AT命令配置的服务器地址和端口字符串
    * @param  ssid_pwd: 按AT命令配置的路由器SSID和密码
  * @retval NONE
  **/
void ESP8266_Init(int8_t * server, int8_t * ssid_pwd);
#endif
