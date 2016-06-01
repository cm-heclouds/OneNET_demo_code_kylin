#include "esp8266.h"
#include "usart2.h"

/*
 *  @brief ESP8266模块初始化
 */
void ESP8266_Init(void)
{
#if 1
    SendCmd(AT, "OK", 1000);		//模块有效性检查
    SendCmd(CWMODE, "OK", 1000);	//模块工作模式
    SendCmd(RST, "OK", 2000);		//模块重置
    SendCmd(CIFSR, "OK", 1000);		//查询网络信息
    SendCmd(CWJAP, "OK", 2000);		//配置需要连接的WIFI热点SSID和密码
    SendCmd(CIPSTART, "OK", 2000);	//TCP连接
    SendCmd(CIPMODE0, "OK", 1000);	//配置非透传模式
	//SendCmd(CIPSEND, ">", 1000);	//进入透传模式
	USART2_Clear();
#endif
}
