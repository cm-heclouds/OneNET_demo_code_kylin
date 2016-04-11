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
#include "EdpDemo.h"
#include "esp8266.h"

int8_t src_api_key[] = "nCVNXYCoX68IHG4DgpyNu5aTXfY=";
int8_t src_dev[] = "768596";
/**
 * @brief  EDP数据包发送
 * @param  buffer: 要发送的数据缓冲区地址
* @param  len: 要发送的数据缓长度
* @param  sockfd：兼容linux socket api: STM32下无意义
 * @retval 发送的数据长度
 **/
int32_t DoSend(int32_t sockfd, const uint8_t *buffer, uint32_t len)
{
    USART2_SendData((uint8_t *)(buffer), len);
    /* wululu test print send bytes */
    hexdump((const uint8_t *)buffer, len);
    return len;
}
/*
 *  @brief  EDP协议向自己透传数据，用于测试，将src_dev替换成目标DEVICE ID即可
 */
void Push_DataToMyself(void)
{
    EdpPacket* send_pkg;
    int8_t push_data[] = {44};
    printf("%s %d\n", __func__, __LINE__);
    send_pkg = PacketPushdata(src_dev, push_data, sizeof(push_data));
    DoSend(0, (const uint8_t *)send_pkg->_data, send_pkg->_write_pos);
    DeleteBuffer(&send_pkg);
    mDelay(1000);
}
/*
 *  @brief  EDP协议向Onenet上传温度信息，数据点格式TYPE=3
 */
void Save_TemperatueToOneNet(void)
{
    EdpPacket* send_pkg;
    uint16_t temperature[1];
    SHT20_read_user_reg();
    mDelay(200);//延迟，设备没有那么快的响应时间，否则总线处理忙等待
    SHT2x_MeasureHM(SHT20_Measurement_T_HM, temperature);

    printf("%s %d t:%d\n", __func__, __LINE__, (uint32_t)temperature[0]);

    send_pkg = PacketSavedataInt(kTypeSimpleJsonWithoutTime, NULL, "temperature", (uint32_t)temperature[0], 0, NULL);

    DoSend(0, (const uint8_t *)send_pkg->_data, send_pkg->_write_pos);
    DeleteBuffer(&send_pkg);
    mDelay(1000);
}


/*
 *  @brief  EDP协议向Onenet上传湿度信息，数据点格式TYPE=3
 */
void Save_HumToOneNet(void)
{
    EdpPacket* send_pkg;
    uint16_t hum[1];
    SHT20_read_user_reg();
    mDelay(200);//延迟，设备没有那么快的响应时间，否则总线处理忙等待
    SHT2x_MeasureHM(SHT20_Measurement_RH_NHM, hum);

    printf("%s %d hum:%d\n", __func__, __LINE__, (uint32_t)hum[0]);

    send_pkg = PacketSavedataInt(kTypeSimpleJsonWithoutTime, NULL, "hum", (uint32_t)hum[0], 0, NULL);

    DoSend(0, (const uint8_t *)send_pkg->_data, send_pkg->_write_pos);
    DeleteBuffer(&send_pkg);
    mDelay(1000);
}

/*
 *  @brief  EDP协议向Onenet上传温湿度信息，使用简单字符串格式，数据点格式TYPE=5，FEILD type 2.
 */
void Save_TempHumToOneNet(void)
{
    EdpPacket* send_pkg;
    uint16_t hum[1], temperature[1];
    int8_t data_string[64] = ",;temperature,%d;hum,%d", data_string_dst[64];

    SHT20_read_user_reg();
    mDelay(200);//延迟，设备没有那么快的响应时间，否则总线处理忙等待
    SHT2x_MeasureHM(SHT20_Measurement_RH_NHM, hum);
    mDelay(200);
    SHT2x_MeasureHM(SHT20_Measurement_T_HM, temperature);
    printf("%s %d hum:%d\n", __func__, __LINE__, (uint32_t)hum[0]);
    printf("%s %d t:%d\n", __func__, __LINE__, (uint32_t)temperature[0]);
    snprintf(data_string_dst, sizeof(data_string), data_string, (uint32_t)temperature[0], (uint32_t)hum[0]);
    printf("%s %d t:%s\n", __func__, __LINE__, data_string_dst);
    send_pkg = PacketSavedataSimpleString(NULL, data_string_dst);

    DoSend(0, (const uint8_t *)send_pkg->_data, send_pkg->_write_pos);
    DeleteBuffer(&send_pkg);
    mDelay(1000);
}
uint8_t data_string_dst[256];
uint8_t data_string_t[64];
uint8_t data_string_adxl[64];
uint8_t data_string_hmc5883l[64];

void Save_AllSensorsToOneNet(void)
{
    EdpPacket* send_pkg;
    uint16_t f;
    uint16_t hum[1], temperature[1];
    int16_t adxl[3], hmc5883l[3];
    /*读取温湿度*/
    memset(data_string_dst, 0, sizeof(data_string_dst));
    SHT20_read_user_reg();
    mDelay(200);//延迟，设备没有那么快的响应时间，否则总线处理忙等
    SHT2x_MeasureHM(SHT20_Measurement_T_HM, temperature);
    mDelay(1000);
    SHT2x_MeasureHM(SHT20_Measurement_RH_HM, hum);
    mDelay(400);
    f = (uint16_t)Read_BH1750();
    snprintf(data_string_t, sizeof(data_string_t), ",;BH1750FVI,%d;SHT20_temperature,%d;SHT20_hum,%d;", (uint16_t)f, (uint16_t)temperature[0], (uint16_t)hum[0]);
    /*读取BH1750FVI*/
    printf("%s\n", data_string_t);
    mDelay(400);
    /*读取ADXL345*/
    ADXL345_GETXYZ(adxl);
    snprintf(data_string_adxl, sizeof(data_string_adxl), "ADXL345_x,0x%x;ADXL345_y,0x%0x;ADXL345_z,0x%x;", (uint16_t)adxl[0], (uint16_t)adxl[1], (uint16_t)adxl[2]);
    printf("%s\n", data_string_adxl);
    mDelay(400);
    /*读取HMC588CL*/
    HMC5883L_GetXYZ(hmc5883l);

    snprintf(data_string_hmc5883l, sizeof(data_string_hmc5883l), "HMC5883L_x,0x%x;HMC5883L_y,0x%x;HMC5883L_z,0x%x", (uint16_t)hmc5883l[0], (uint16_t)hmc5883l[2], (uint16_t)hmc5883l[1]);
    printf("%s\n", data_string_hmc5883l);
    strcat(data_string_dst, data_string_t);
    strcat(data_string_dst, data_string_adxl);
    strcat(data_string_dst, data_string_hmc5883l);
    printf("%s\n", data_string_dst);
    send_pkg = PacketSavedataSimpleString(NULL, data_string_dst);

    DoSend(0, (const uint8_t *)send_pkg->_data, send_pkg->_write_pos);
    DeleteBuffer(&send_pkg);
    mDelay(1000);
}
/*
 *  @brief  EDP协议测试主循环
 */
void EDP_Loop(void)
{

    mDelay(2000);
    while(1)
    {
        Connect_RequestType1(src_dev, src_api_key);
        mDelay(1000);
        mDelay(1000);
        mDelay(1000);
        mDelay(1000);
        printf("%s %d\n", __func__, __LINE__);
        Recv_Thread_Func();
        mDelay(1000);
        mDelay(1000);
#if 0
        Ping_Server();
        printf("%s %d\n", __func__, __LINE__);
        Recv_Thread_Func();
        mDelay(1000);
        Save_TemperatueToOneNet();
        mDelay(1000);
        Recv_Thread_Func();
        mDelay(1000);
        Save_HumToOneNet();
        mDelay(1000);
        Recv_Thread_Func();
        mDelay(1000);
        Save_TempHumToOneNet();
#endif
        Save_AllSensorsToOneNet();
        mDelay(1000);
        Recv_Thread_Func();
        mDelay(1000);
    }
}

/*
 *  @brief  串口接收处理线程
 */
void Recv_Thread_Func(void)
{
    int32_t error = 0;
    int32_t rtn;
    uint8_t mtype, jsonorbin;
    uint8_t buffer[128];
    RecvBuffer *recv_buf = NewBuffer();
    EdpPacket *pkg;

    int8_t *src_devid;
    int8_t *push_data;
    uint32_t push_datalen;

    cJSON *desc_json;
    int8_t *desc_json_str;
    int8_t *save_bin;
    uint32_t save_binlen;
    int8_t *json_ack;

    int8_t *cmdid;
    uint16_t cmdid_len;
    int8_t *cmd_req;
    uint32_t cmd_req_len;
    EdpPacket *send_pkg;
    int8_t *ds_id;
    double dValue = 0;

    int8_t *simple_str = NULL;
    int8_t cmd_resp[] = "ok";
    uint32_t cmd_resp_len = 0;

    printf("\n[%s] recv thread start ...\r\n", __func__);

    while (error == 0)
    {
        /* 试着接收1024个字节的数据 */
        int32_t rcv_len = 0;

        rcv_len = USART2_GetRcvNum();
        if (rcv_len <= 0)
        {
            printf("%s %d No Data\n", __func__, __LINE__);
            break;
        }
        USART2_GetRcvData(buffer, rcv_len);
        printf("recv from server, bytes: %d\r\n", rcv_len);
        /* wululu test print send bytes */
        hexdump((const uint8_t *)buffer, rcv_len);
        printf("\n");
        /* 成功接收了n个字节的数据 */
        WriteBytes(recv_buf, buffer, rcv_len);
        while (1)
        {
            /* 获取一个完成的EDP包 */
            if ((pkg = GetEdpPacket(recv_buf)) == 0)
            {
                printf("need more bytes...\n");
                break;
            }
            /* 获取这个EDP包的消息类型 */
            mtype = EdpPacketType(pkg);
            printf("mtype=%d\n", mtype);
            /* 根据这个EDP包的消息类型, 分别做EDP包解析 */
            switch (mtype)
            {
                case CONNRESP:
                    /* 解析EDP包 - 连接响应 */
                    rtn = UnpackConnectResp(pkg);
                    printf("recv connect resp, rtn: %d\n", rtn);
                    break;
                case PUSHDATA:
                    /* 解析EDP包 - 数据转发 */
                    UnpackPushdata(pkg, &src_devid, &push_data,
                                   &push_datalen);
                    printf
                    ("recv push data, src_devid: %s, push_data: %s, len: %d\n",
                     src_devid, push_data, push_datalen);
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
                            printf("json type is %d\n",
                                   jsonorbin);
                            /* 解析EDP包 - json数据存储 */
                            /* UnpackSavedataJson(pkg, &save_json); */
                            /* save_json_str=cJSON_Print(save_json); */
                            /* printf("recv save data json, src_devid: %s, json: %s\n", */
                            /*     src_devid, save_json_str); */
                            /* free(save_json_str); */
                            /* cJSON_Delete(save_json); */

                            /* UnpackSavedataInt(jsonorbin, pkg, &ds_id, &iValue); */
                            /* printf("ds_id = %s\nvalue= %d\n", ds_id, iValue); */

                            UnpackSavedataDouble(jsonorbin,
                                                 pkg,
                                                 &ds_id,
                                                 &dValue);
                            printf
                            ("ds_id = %s\nvalue = %f\n",
                             ds_id, dValue);

                            /* UnpackSavedataString(jsonorbin, pkg, &ds_id, &cValue); */
                            /* printf("ds_id = %s\nvalue = %s\n", ds_id, cValue); */
                            /* free(cValue); */
                            free(ds_id);
                        }
                        else if (jsonorbin == kTypeBin)     /* 解析EDP包 - bin数据存储 */
                        {
                            UnpackSavedataBin(pkg,
                                              &desc_json,
                                              (uint8_t **) &
                                              save_bin,
                                              &save_binlen);
                            desc_json_str =
                                cJSON_Print(desc_json);
                            printf
                            ("recv save data bin, src_devid: %s, desc json: %s, bin: %s, binlen: %d\n",
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
                        printf("error\n");
                    }
                    break;
                case SAVEACK:
                    json_ack = NULL;
                    UnpackSavedataAck(pkg, &json_ack);
                    printf("save json ack = %s\n", json_ack);
                    free(json_ack);
                    break;
                case CMDREQ:
                    if (UnpackCmdReq
                        (pkg, &cmdid, &cmdid_len, &cmd_req,
                         &cmd_req_len) == 0)
                    {
                        /*
                         * 用户按照自己的需求处理并返回，响应消息体可以为空，此处假设返回2个字符"ok"。
                         * 处理完后需要释放
                         */
                        cmd_resp_len = strlen(cmd_resp);
                        send_pkg =
                            PacketCmdResp(cmdid, cmdid_len,
                                          cmd_resp,
                                          cmd_resp_len);
#ifdef _ENCRYPT
                        if (g_is_encrypt)
                        {
                            SymmEncrypt(send_pkg);
                        }
#endif
                        DoSend(0, (const uint8_t *)
                               send_pkg->_data,
                               send_pkg->_write_pos);
                        DeleteBuffer(&send_pkg);

                        free(cmdid);
                        free(cmd_req);
                    }
                    break;
                case PINGRESP:
                    /* 解析EDP包 - 心跳响应 */
                    UnpackPingResp(pkg);
                    printf("recv ping resp\n");
                    break;
                default:
                    /* 未知消息类型 */
                    error = 1;
                    printf("recv failed...\n");
                    break;
            }
            DeleteBuffer(&pkg);
        }
    }
    DeleteBuffer(&recv_buf);

#ifdef _DEBUG
    printf("[%s] recv thread end ...\n", __func__);
#endif
}

void Connect_RequestType1(int8_t *devid, int8_t *api_key)
{
    EdpPacket *send_pkg;

    send_pkg = PacketConnect1((const int8_t *)devid, (const int8_t *)api_key);
    if(send_pkg == NULL)
    {
        return;
    }
    /* send_pkg = PacketConnect2("433223", "{ \"SYS\" : \"0DEiuApATHgLurKNEl6vY4bLwbQ=\" }"); */
    /* send_pkg = PacketConnect2("433223", "{ \"13982031959\" : \"888888\" }"); */

    /* 向设备云发送连接请求 */
    printf("send connect to server, bytes: %d\n", send_pkg->_write_pos);
    DoSend(0, (const uint8_t *)send_pkg->_data, send_pkg->_write_pos);
    /* 必须释放这个内存，否则造成泄露 */
    DeleteBuffer(&send_pkg);
    mDelay(1000);
}
/*
 *  @brief  发送PING包维持心跳
 */
void Ping_Server(void)
{
    EdpPacket *send_pkg;
    printf("%s %d\n", __func__, __LINE__);
    /* 组装ping包 */
    send_pkg = PacketPing();

    DoSend(0, (const uint8_t *)send_pkg->_data,
           send_pkg->_write_pos);
    mDelay(500);
    /* 必须释放这个内存，否则造成泄露 */
    DeleteBuffer(&send_pkg);
    mDelay(100);
}
