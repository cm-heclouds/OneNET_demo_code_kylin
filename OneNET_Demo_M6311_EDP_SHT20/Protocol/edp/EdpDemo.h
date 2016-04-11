#ifndef __EDPDEMO_KIT_H__
#define __EDPDEMO_KIT_H__

#include <stdio.h>
#include <stdlib.h>
#if defined(_LINUX)
//#include <unistd.h>
//#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <getopt.h>
#include <libgen.h>
#endif
#include <string.h>

#include "EdpKit.h"
#include "stm32f10x.h"
#include "usart2.h"
#include "utils.h"
#include "sht20.h"

/*----------------------------错误码-----------------------------------------*/
#define ERR_CREATE_SOCKET   -1
#define ERR_HOSTBYNAME      -2
#define ERR_CONNECT         -3
#define ERR_SEND            -4
#define ERR_TIMEOUT         -5
#define ERR_RECV            -6

/**
 * @brief  EDP登录连接，采用协议中第一种方式登录连接，鉴权信息为deviceid和api key
 * @param  api_key: 该设备的api key或项目master key
* @param  devid: 登录的EDP设备ID
 * @retval 发送的数据长度
 **/
void Connect_RequestType1(int8_t *devid, int8_t *api_key);

/**
 * @brief  EDP数据包发送
 * @param  buffer: 要发送的数据缓冲区地址
* @param  len: 要发送的数据缓长度
* @param  sockfd：兼容linux socket api: STM32下无意义
 * @retval 发送的数据长度
 **/
int32_t DoSend(int32_t sockfd, const uint8_t *buffer, uint32_t len);

/*
 *  @brief  EDP协议向自己透传数据，用于测试，将src_dev替换成目标DEVICE ID即可
 */
void Push_DataToMyself(void);
/*
 *  @brief  EDP协议向Onenet上传温度信息，数据点格式TYPE=3
 */
void Save_TemperatueToOneNet(void);
/*
 *  @brief  EDP协议向Onenet上传湿度信息，数据点格式TYPE=3
 */
void Save_HumToOneNet(void);

/*
 *  @brief  EDP协议向Onenet上传温湿度信息，使用简单字符串格式，数据点格式TYPE=5，FEILD type 2.
 */
void Save_TempHumToOneNet(void);
/*
 *  @brief  EDP协议测试主循环
 */
void EDP_Loop(void);
/*
 *  @brief  串口接收处理线程
 */
void Recv_Thread_Func(void);
/*
 *  @brief  发送PING包维持心跳
 */
void Ping_Server(void);
#endif
