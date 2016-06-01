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
#include "esp8266.h"

/*
        */
/*----------------------------错误码-----------------------------------------*/
#define ERR_CREATE_SOCKET   -1
#define ERR_HOSTBYNAME      -2
#define ERR_CONNECT         -3
#define ERR_SEND            -4
#define ERR_TIMEOUT         -5
#define ERR_RECV            -6

void Recv_Thread_Func(void);
void Connect_RequestType1(char *devid, char *api_key);


/**
  * @brief  EDP数据包发送
  * @param  buffer: 要发送的数据缓冲区地址
  * @param  len: 要发送的数据缓长度
  * @param  sockfd：兼容linux socket api: STM32下无意义
  * @retval 发送的数据长度
  */
int32_t DoSend(int32_t sockfd, const unsigned char *buffer, uint32_t len)
{
    USART2_Write(USART2, (unsigned char *)(buffer), len);
    /* wululu test print send bytes */
    hexdump((const unsigned char *)buffer, len);
    return len;
}

/**
  * @brief  非透传模式发送EDP报文
  */
void sendPkt(EdpPacket **p)
{
    char sendBuf[30] = {0};
    EdpPacket* edpPkt = *p;

    /* 非透传模式先发送AT+CIPSEND=X */
    sprintf(sendBuf, "AT+CIPSEND=%d\r\n", edpPkt->_write_pos);
    SendCmd(sendBuf, ">", 500);
    USART2_Clear();

    /* EDP设备连接包，发送 */
    USART2_Write(USART2, (uint8_t*)(edpPkt->_data), edpPkt->_write_pos);    //串口发送
    hexdump(edpPkt->_data, edpPkt->_write_pos);     //打印发送内容
    DeleteBuffer(&edpPkt);      //完成发送需要将缓冲区释放
}


/**
  * @brief     串口数据解析
  * @param     buffer，串口接收数据缓存
  * @param     plen， 用于保存解析后EDP数据的长度
  * @attention 由于串口数据参杂着8266模块的上报信息，需要将串口数据进行解析
  *            根据模块手册，若收到 +IPD, 则表明收到了服务器的下发数据
  * @retval    串口数据中EDP数据的首指针，若没有数据则返回NULL
  */
char *uartDataParse(char *buffer, int32_t *plen)
{
    char *p;
    char *pnum;
    int32_t len;
    if((p = strstr(buffer, "CLOSED")) != NULL)
    {
        printf("tcp connection closed\r\n");
    }
    if((p = strstr(buffer, "WIFI DISCONNECT")) != NULL)
    {
        printf("wifi disconnected\r\n");
    }
    if((p = strstr(buffer, "WIFI CONNECTED")) != NULL)
    {
        printf("wifi connected\r\n");
    }
    if((p = strstr(buffer, "+IPD")) != NULL)
    {
        pnum = p + 5;       //跳过头部的 "+IPD,"，指向长度字段的首地址
        p = strstr(p, ":"); //指向长度字段末尾
        *(p++) = '\0';      //长度字段末尾添加结束符，p指向服务器下发的第一个字节
        printf("rcv %d data from OneNET:\r\n", (len = atoi(pnum)));
        hexdump((const unsigned char *)p, len);    //打印接收数据
        *plen = len;
        return p;
    }
    return NULL;
}


/**
  * @brief     接收线程函数
  * @attention 原SDK中采用socket接收数据方式，本例中将其修改为串口接收，
  */
void Recv_Thread_Func(void)
{
    int error = 0;
    int rtn;
    uint8_t mtype, jsonorbin;
    uint8_t buffer[128] = {0};
    RecvBuffer *recv_buf = NewBuffer();
    EdpPacket *pkg;

    char *src_devid;
    char *push_data;
    uint32_t push_datalen;

    cJSON *desc_json;
    char *desc_json_str;
    char *save_bin;
    uint32_t save_binlen;
    char *json_ack;

    char *cmdid;
    uint16_t cmdid_len;
    char *cmd_req;
    uint32_t cmd_req_len;
    EdpPacket *edpPkt;
    char *ds_id;
    double dValue = 0;

    char *simple_str = NULL;

    while (error == 0)
    {
        int32_t rcv_len = 0;

        rcv_len = USART2_GetRcvNum();
        if (rcv_len <= 0)
        {
            //printf("No serial Data\r\n");
            break;
        }
        printf("rcv_len: %d\r\n", rcv_len);
        mDelay(100);
        USART2_GetRcvData(buffer, rcv_len);
        hexdump(buffer, rcv_len);   //打印接收数据

        /* 串口数据分析，区分WIFI模块的推送信息与服务器的下发信息 */
        //if((p = uartDataParse(buffer, &rcv_len)) == NULL)
        //{
        //    printf("No server Data\r\n");
        //    break;
        //}

        /* 成功接收了rcv_len个字节的数据，将其写入recv_buf，用于解析 */
        WriteBytes(recv_buf, buffer, rcv_len);
        while (1)
        {
            /* 获取一个完成的EDP包 */
            if ((pkg = GetEdpPacket(recv_buf)) == 0)
            {
                printf("need more bytes...\r\n");   //没有完整的EDP包
                break;
            }
            /* 获取这个EDP包的消息类型 */
            mtype = EdpPacketType(pkg);
            printf("message type=0x%02X\r\n", mtype);
            /* 根据这个EDP包的消息类型, 分别做EDP包解析 */
            switch (mtype)
            {
                case CONNRESP:
                    /* 解析EDP包 - 连接响应 */
                    rtn = UnpackConnectResp(pkg);
                    printf("recv connect resp, rtn: %d\r\n", rtn);
                    break;
                case PUSHDATA:
                    /* 解析EDP包 - 数据转发 */
                    UnpackPushdata(pkg, &src_devid, &push_data, &push_datalen);
                    printf("recv push data, src_devid: %s, push_data: %s, len: %d\r\n",
                           src_devid, push_data, push_datalen);


                    /* 在本例中，若收到透传过来的报文之后，将数据返回给源设备 */
                    edpPkt = PacketPushdata(src_devid, push_data, push_datalen);
                    printf("send ping pkt to server, bytes: %d\n", edpPkt->_write_pos);
                    sendPkt(&edpPkt);       //非透传模式发送

                    free(src_devid);
                    free(push_data);
                    break;
                case SAVEDATA:
                    /* 解析EDP包 - 数据存储 */
                    if (UnpackSavedata(pkg, &src_devid, &jsonorbin)
                        == 0)
                    {
                        if (jsonorbin == kTypeFullJson
                            || jsonorbin ==
                            kTypeSimpleJsonWithoutTime
                            || jsonorbin ==
                            kTypeSimpleJsonWithTime)
                        {
                            printf("json type is %d\n", jsonorbin);
                            /* 解析EDP包 - json数据存储 */
                            /* UnpackSavedataJson(pkg, &save_json); */
                            /* save_json_str=cJSON_Print(save_json); */
                            /* printf("recv save data json, src_devid: %s, json: %s\n", */
                            /*     src_devid, save_json_str); */
                            /* free(save_json_str); */
                            /* cJSON_Delete(save_json); */

                            /* UnpackSavedataInt(jsonorbin, pkg, &ds_id, &iValue); */
                            /* printf("ds_id = %s\nvalue= %d\n", ds_id, iValue); */

                            UnpackSavedataDouble((SaveDataType)jsonorbin, pkg, &ds_id, &dValue);
                            printf("ds_id = %s\nvalue = %f\r\n", ds_id, dValue);

                            /* UnpackSavedataString(jsonorbin, pkg, &ds_id, &cValue); */
                            /* printf("ds_id = %s\nvalue = %s\n", ds_id, cValue); */
                            /* free(cValue); */
                            free(ds_id);
                        }
                        else if (jsonorbin == kTypeBin)     /* 解析EDP包 - bin数据存储 */
                        {
                            UnpackSavedataBin(pkg, &desc_json, (uint8_t **)&save_bin, &save_binlen);
                            desc_json_str = cJSON_Print(desc_json);
                            printf("recv save data bin, src_devid: %s, desc json: %s, bin: %s, binlen: %d\r\n",
                                   src_devid, desc_json_str,
                                   save_bin, save_binlen);
                            free(desc_json_str);
                            cJSON_Delete(desc_json);
                            free(save_bin);
                        }
                        else if (jsonorbin == kTypeString)
                        {
                            UnpackSavedataSimpleString(pkg,
                                                       &simple_str);
                            printf("%s\n", simple_str);
                            free(simple_str);
                        }
                        free(src_devid);
                    }
                    else
                    {
                        printf("error\r\n");
                    }
                    break;
                case SAVEACK:
                    json_ack = NULL;
                    UnpackSavedataAck(pkg, &json_ack);
                    printf("save json ack = %s\r\n", json_ack);
                    free(json_ack);
                    break;
                case CMDREQ:
                    if (UnpackCmdReq
                        (pkg, &cmdid, &cmdid_len, &cmd_req,
                         &cmd_req_len) == 0)
                    {
#if 0
                        /*
                         * 用户按照自己的需求处理并返回，响应消息体可以为空，此处假设返回2个字符"ok"。
                         * 处理完后需要释放
                         */
                        cmd_resp_len = strlen(cmd_resp);
                        edpPkt =
                            PacketCmdResp(cmdid, cmdid_len,
                                          cmd_resp,
                                          cmd_resp_len);
#ifdef _ENCRYPT
                        if (g_is_encrypt)
                        {
                            SymmEncrypt(edpPkt);
                        }
#endif
                        DoSend(0, (const unsigned char *)
                               edpPkt->_data,
                               edpPkt->_write_pos);
                        DeleteBuffer(&edpPkt);
#endif

                        /* 收到服务器下发的EDP命令 */
                        printf("cmdid: %s\r\n", cmdid);
                        printf("cmd_req: %s\r\n", cmd_req);

                        /* 在这里添加需要实现的控制功能代码 */

                        free(cmdid);
                        free(cmd_req);
                    }
                    break;
                case PINGRESP:
                    /* 解析EDP包 - 心跳响应 */
                    UnpackPingResp(pkg);
                    printf("recv ping resp\r\n");
                    break;
                default:
                    /* 未知消息类型 */
                    error = 1;
                    printf("recv failed...\r\n");
                    break;
            }
            DeleteBuffer(&pkg);
        }
    }
    DeleteBuffer(&recv_buf);

#ifdef _DEBUG
    printf("[%s] recv thread end ...\r\n", __func__);
#endif
}
