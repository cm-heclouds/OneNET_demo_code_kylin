#include "m6331.h"

/**
  * @brief  初始化MT6331，并配置路由器和连接服务器
  * @param  server:按AT命令配置的服务器地址和端口字符串
    * @param  ssid_pwd: 按AT命令配置的路由器SSID和密码
  * @retval NONE
  **/
#if 1
void M6331_Init(void)
{	 
		mDelay(2000);	
		mDelay(2000);
	  SendCmd("AT+SSYS?\r\n","OK",300);
		mDelay(500);
	  SendCmd("AT+SIM1\r\n","OK",300);
		mDelay(500);
	  SendCmd("AT+CPIN?\r\n", "+CPIN: READY",1000); //确保SIM卡PIN码解锁，返回READY，表示解锁成功
		mDelay(1500);
		SendCmd("AT+CREG?\r\n","0,5",1000);    //确认网络搜索成功,OK
		mDelay(1000);
		SendCmd("AT+CSQ\r\n","OK", 1000);//查看信号质量
		mDelay(1000);
		//SendCmd(CGDCONT,"OK", 1000);//
		//Delay(1000);
		SendCmd("AT+CGACT=1,1\r\n","OK", 1000);//激活
		mDelay(1000);
		SendCmd("AT+CGATT=1\r\n","OK", 1000);  //
		mDelay(1000);
		SendCmd("AT+CMMUX=0\r\n","OK", 1000);  //single way
	  mDelay(1000);
	  SendCmd("AT+CMMODE=1\r\n","OK",1000);   //配置透传
		mDelay(1000);
	  SendCmd("AT+CMTCFG=1,1024,1\r\n","OK",1000);   //配置透传，最大长度2000字节，间隔是100ms。配置成hex模式
		mDelay(1500);	
		//SendCmd("AT+IPSTART=\"TCP\",\"183.230.40.33\",80\r\n","CONNECT",1000); //连接EDP服务器
		SendCmd("AT+IPSTART=\"TCP\",\"183.230.40.39\",876\r\n","CONNECT",3000);//AT+IPSTART="TCP","183.230.40.39",876
	  mDelay(1000);
}

/**
  * @brief  通过USART2连接MT6331发送数据.
  * @param  buf: 要发送的数据缓冲区地址
    * @param  len: 要发送的数据缓长度
  * @retval 发送的数据长度
  **/
int32_t M6331_SendData(uint8_t * buf,uint32_t len)
{
	int32_t ret=0,i=0;
	memset(usart2_rcv_buf,0,strlen(usart2_rcv_buf));
	usart2_rcv_len=0;
	USART2_Write(USART2,buf,len);	
	mDelay(20);	
}

/**
  * @brief  通过USART2发送数据.
  * @param  buf: 要发送的数据缓冲区地址
    * @param  len: 要发送的数据缓长度
  * @retval 发送的数据长度
  **/
int32_t USART2_SendData(uint8_t * buf,uint32_t len)
{
	M6331_SendData(buf,len);
	return len;
}
#endif