#ifndef EMW3081_H_H
#define EMW3081_H_H

/**
  * @brief  ONENET版本AT指令
**/
#define  REBOOT             "AT+REBOOT\r"   //重启EMW3081
#define  WSTATUS            "AT+WSTATUS\r"  //查询EMW3081网络状态
#define  CIOT               "AT+CIOT?\r"    //查询设备的本地参数是否存在
#define  CIOTMKEY           "AT+CIOTMKEY?\r"   //查询主APIKEY本地参数是否存在
#define  CIOTMKEYA          "AT+CIOTMKEYA=Sezt7lZFiwVUcYZeWLZM91kOMBw=\r"  //设置住APIKEY
#define  CIOTID             "AT+CIOTID?\r"  //查询设备ID
#define  CIOTIDC            "AT+CIOTIDC\r"  //删除设备ID
#define  CIOTINIT           "AT+CIOTINIT\r"  //设备参数初始化
#define  CIOTKEY            "AT+CIOTKEY?\r"  //查询设备APIKEY
#define  CIOTKEYC           "AT+CIOTKEYC\r"   //删除设备APIKEY
#define  CIOTSTATUS         "AT+CIOTSTATUS\r"   //查询设备连接状态
#define  CIOTSTART          "AT+CIOTSTART=0\r"   //启动设备的数据传送
#define  CIOTDAT            "AT+CIOTDAT=TEMP3,,20\r"  //向ONENET平台发送数据流名为TEMP3，值为20的数据点。


#define  CIOTBINSET         "AT+CIOTBINSET=PIC4,5636\r"  //发送二进制文件的参数设备信息
#define  CIOTBINEND         "AT+CIOTBINEND\r"  //结束二进制文件发送

extern unsigned char dat1[1024];
extern unsigned char dat2[1024];
extern unsigned char dat3[1024];
extern unsigned char dat4[1024];
extern unsigned char dat5[1024];
extern unsigned char dat6[1024];

extern void SendCmd(char* cmd, char* result, int timeOut);
extern void EMW3081_Reboot(void);
extern void EMW3081_SendBinDat(void);

#endif



