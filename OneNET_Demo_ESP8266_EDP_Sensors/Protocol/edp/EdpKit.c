#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "EdpKit.h"
#include "stdint.h"
void my_assert(int a)
{
    if(a == 0)
    {
        //printf("assert false\n");
    }
}
#define MKFUN_PACKET_SAVE_DATA(TYPE, NAME)              \
    EdpPacket* PacketSavedata##NAME(SaveDataType type,          \
                    const char* dst_devid,      \
                    const char* ds_id,          \
                    TYPE value,             \
                    time_t at,              \
                    const char* token){         \
    EdpPacket* pkg = NULL;                      \
    switch (type){                          \
    case kTypeFullJson:                     \
        pkg = PacketSavedataType1##NAME(dst_devid, ds_id,       \
                        value, at, token);      \
        break;                          \
    case kTypeSimpleJsonWithoutTime:                \
        pkg = PacketSavedataType2##NAME(dst_devid, ds_id,       \
                        value, at, token);      \
        break;                          \
    case kTypeSimpleJsonWithTime:                   \
        pkg = PacketSavedataType3##NAME(dst_devid, ds_id,       \
                        value, at, token);      \
        break;                          \
    default:                            \
        break;                          \
    }                               \
                                    \
    return pkg;                             \
}

#define MKFUN_PACKET_SAVE_DATA_TYPE1(TYPE, NAME, MED)           \
    static EdpPacket* PacketSavedataType1##NAME(const char* dst_devid,  \
                        const char * ds_id,  \
                        TYPE value,     \
                        time_t at,      \
                        const char* token){ \
    cJSON* json_obj = NULL;                     \
    cJSON* ds_array = NULL;                     \
    cJSON* json_obj_ds = NULL;                  \
    cJSON* dp_array = NULL;                     \
    cJSON* dp_item = NULL;                      \
    char ss[100] = {0};                     \
    EdpPacket* pkg = NULL;                      \
                                    \
    json_obj = cJSON_CreateObject();                \
                                    \
    if(token) {                         \
        cJSON_AddStringToObject(json_obj, "token", token);      \
    }                               \
                                    \
    ds_array = cJSON_CreateArray();                 \
    json_obj_ds = cJSON_CreateObject();             \
                                    \
    cJSON_AddStringToObject(json_obj_ds, "id", ds_id);      \
                                    \
    dp_array = cJSON_CreateArray();                 \
    dp_item = cJSON_CreateObject();                 \
                                    \
    FormatAt(ss, 100, at);                      \
    cJSON_AddStringToObject(dp_item, "at", ss);         \
    cJSON_Add##MED##ToObject(dp_item, "value", value);      \
    cJSON_AddItemToArray(dp_array, dp_item);            \
                                    \
    cJSON_AddItemToObject(json_obj_ds, "datapoints", dp_array); \
    cJSON_AddItemToArray(ds_array, json_obj_ds);            \
    cJSON_AddItemToObject(json_obj, "datastreams", ds_array);   \
                                    \
    pkg = PacketSavedataJson((const int8_t*)dst_devid, json_obj,           \
                        kTypeFullJson);     \
    cJSON_Delete(json_obj);                     \
                                    \
    return pkg;                         \
}

#define MKFUN_PACKET_SAVE_DATA_TYPE2(TYPE, NAME, MED)           \
    static EdpPacket* PacketSavedataType2##NAME(const char* dst_devid,  \
                        const char* ds_id,  \
                        TYPE value,     \
                        time_t at,      \
                        const char* token)  \
    {                                   \
    cJSON* json_obj = NULL;                     \
    EdpPacket* pkg = NULL;                      \
                                    \
    json_obj = cJSON_CreateObject();                \
    cJSON_Add##MED##ToObject(json_obj, ds_id, value);       \
                                    \
    pkg = PacketSavedataJson((const int8_t*)dst_devid, json_obj,           \
                        kTypeSimpleJsonWithoutTime); \
    cJSON_Delete(json_obj);                     \
                                    \
    return pkg;                         \
}


#define MKFUN_PACKET_SAVE_DATA_TYPE3(TYPE, NAME, MED)           \
    static EdpPacket* PacketSavedataType3##NAME(const char* dst_devid,  \
                        const char* ds_id,  \
                        TYPE value,     \
                        time_t at,      \
                        const char* token){ \
    cJSON* json_obj = NULL;                     \
    cJSON* json_obj_ds = NULL;                  \
    char ss[100] = {0};                     \
    EdpPacket* pkg = NULL;                      \
                                    \
    json_obj = cJSON_CreateObject();                \
    FormatAt(ss, 100, at);                      \
    json_obj_ds = cJSON_CreateObject();             \
    cJSON_Add##MED##ToObject(json_obj_ds, ss, value);       \
    cJSON_AddItemToObject(json_obj, ds_id, json_obj_ds);        \
                                    \
    pkg = PacketSavedataJson((const int8_t*)dst_devid, json_obj,           \
                        kTypeSimpleJsonWithTime);   \
    cJSON_Delete(json_obj);                     \
                                    \
    return pkg;                         \
}

#define MKFUN_UNPACK_SAVE_DATA(TYPE, NAME)              \
    int32_t UnpackSavedata##NAME(SaveDataType type,         \
                 EdpPacket* pkg,            \
                 char** ds_id,              \
                 TYPE* value)               \
    {                                   \
    int32_t ret = -1;                           \
    switch (type){                          \
    case kTypeFullJson:                         \
    ret = UnpackSavedataType1##NAME(pkg, ds_id, value);     \
    break;                              \
    case kTypeSimpleJsonWithoutTime:                    \
    ret = UnpackSavedataType2##NAME(pkg, ds_id, value);     \
    break;                              \
    case kTypeSimpleJsonWithTime:                   \
    ret = UnpackSavedataType3##NAME(pkg, ds_id, value);     \
    break;                              \
    default:                                \
    break;                              \
    }                                   \
                                    \
    return ret;                             \
}

#define MKFUN_UNPACK_SAVE_DATA_TYPE1(TYPE, NAME, MED)           \
    static int32_t UnpackSavedataType1##NAME(EdpPacket* pkg,        \
                         char** ds_id,      \
                         TYPE* value)       \
    {                                   \
    cJSON* json_obj = NULL;                     \
    cJSON* ds_array = NULL;                     \
    cJSON* ds_item = NULL;                      \
    char* id = NULL;                            \
    cJSON* dp_array = NULL;                     \
    cJSON* dp_item = NULL;                      \
    char* valuestring = NULL;                       \
                                    \
    if(0 != UnpackSavedataJson(pkg, &json_obj))             \
    return ERR_UNPACK_SAVED_JSON;                   \
                                    \
    ds_array = cJSON_GetObjectItem(json_obj, "datastreams");        \
    if((0 == ds_array) || (1 != cJSON_GetArraySize(ds_array)))      \
    return ERR_UNPACK_SAVED_JSON;                   \
                                    \
    ds_item = cJSON_GetArrayItem(ds_array, 0);              \
    if(0 == ds_item)                            \
    return ERR_UNPACK_SAVED_JSON;                   \
                                    \
    id = cJSON_GetObjectItem(ds_item, "id")->valuestring;       \
    *ds_id = (char*)malloc(strlen(id)+1);               \
    memcpy((char*)(*ds_id), id, strlen(id)+1);              \
                                        \
    dp_array = cJSON_GetObjectItem(ds_item, "datapoints");      \
    if((0 == dp_array) || (1 != cJSON_GetArraySize(dp_array)))      \
    return ERR_UNPACK_SAVED_JSON;                   \
                                    \
    dp_item = cJSON_GetArrayItem(dp_array, 0);              \


#define UNPACK_SAVE_DATA_TYPE1_END_STRING               \
    valuestring = cJSON_GetObjectItem(dp_item, "value")->valuestring;   \
    *value = (char*)malloc(strlen(valuestring)+1);          \
    memcpy((char*)(*value), valuestring, strlen(valuestring)+1);    \
    cJSON_Delete(json_obj);                     \
    return 0;                               \
}

#define UNPACK_SAVE_DATA_TYPE1_END_NUMBER(TYPE)         \
    *value = cJSON_GetObjectItem(dp_item, "value")->value##TYPE;    \
    cJSON_Delete(json_obj);                     \
    return 0;                               \
}

#define MKFUN_UNPACK_SAVE_DATA_TYPE2(TYPE, NAME, MED)           \
    static int32_t UnpackSavedataType2##NAME(EdpPacket* pkg,        \
                          char** ds_id,     \
                          TYPE* value)      \
    {                                   \
        cJSON* json_obj = NULL;                     \
    cJSON* json_child = NULL;                   \
    size_t len = 0;                         \
    char* valuestring = NULL;                   \
                                    \
    if(0 != UnpackSavedataJson(pkg, &json_obj))         \
        return ERR_UNPACK_SAVED_JSON;               \
    if (!json_obj->child){                      \
        cJSON_Delete(json_obj);                 \
        return ERR_UNPACK_SAVED_JSON;               \
    }                               \
    json_child = json_obj->child;                   \
    len = strlen(json_child->string) + 1;               \
    *ds_id = (char*)malloc(len);                    \
    memcpy((char*)(*ds_id), json_child->string, len);       \

#define UNPACK_SAVE_DATA_TYPE23_END_NUMBER(TYPE)            \
    *value = json_child->value##TYPE;                   \
    cJSON_Delete(json_obj);                     \
    return 0;                               \
}

#define UNPACK_SAVE_DATA_TYPE23_END_STRING()                \
    valuestring = json_child->valuestring;              \
    *value = (char*)malloc(strlen(valuestring)+1);          \
    memcpy((char*)(*value), valuestring, strlen(valuestring)+1);    \
    cJSON_Delete(json_obj);                     \
    return 0;                               \
}

#define MKFUN_UNPACK_SAVE_DATA_TYPE3(TYPE, NAME, MED)           \
    static int32_t UnpackSavedataType3##NAME(EdpPacket* pkg,        \
                          char** ds_id,     \
                          TYPE* value)      \
{                                   \
        cJSON* json_obj = NULL;                     \
    cJSON* json_child = NULL;                   \
    char* valuestring = NULL;                   \
    size_t len = 0;                         \
                                    \
    if(0 != UnpackSavedataJson(pkg, &json_obj))         \
        return ERR_UNPACK_SAVED_JSON;               \
                                    \
    json_child = json_obj->child;                   \
    if (!json_child || !json_child->string){            \
        cJSON_Delete(json_obj);                 \
        return ERR_UNPACK_SAVED_JSON;               \
    }                               \
                                    \
    len = strlen(json_child->string) + 1;               \
    *ds_id = (char*)malloc(len);                    \
    memcpy(*ds_id, json_child->string, len);            \
                                    \
    json_child = json_child->child;                 \
    if (!json_child){                       \
        cJSON_Delete(json_obj);                 \
        return ERR_UNPACK_SAVED_JSON;               \
    }                               \

static void FormatAt(char* buffer, int32_t len, time_t now)
{
#if 0
    struct tm timinfo;

    if (now == 0)
    {
        now = time(0);
    }
#endif

#if defined(linux) || defined(__linux) || defined(__linux__)
    strftime(buffer, len, "%F %T", localtime_r(&now, &timinfo));
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    localtime_s(&timinfo, &now);
    strftime(buffer, len, "%F %T", &timinfo);
#endif
}

/*---------------------------------------------------------------------------*/
Buffer* NewBuffer()
{
    Buffer* buf = (Buffer*)malloc(sizeof(Buffer));
    if(buf == NULL)
    {
        return NULL;
    }
    buf->_data = (uint8_t*)malloc(sizeof(uint8_t) * BUFFER_SIZE);
    if(buf->_data == NULL)
    {
        printf("%s %d NewBuffer failed\n", __func__, __LINE__);
        free(buf);
        buf = NULL;
        return NULL;
    }
    buf->_write_pos = 0;
    buf->_read_pos = 0;
    buf->_capacity = BUFFER_SIZE;
    return buf;
}
void DeleteBuffer(Buffer** buf)
{
    uint8_t* pdata = (*buf)->_data;
    free(pdata);
    free(*buf);
    *buf = 0;
}
int32_t CheckCapacity(Buffer* buf, uint32 len)
{
    uint32 cap_len = buf->_capacity;
    int32_t flag = 0;
    uint8_t* pdata = NULL;

    while (cap_len - buf->_write_pos < len) /* remain len < len */
    {
        cap_len = cap_len << 1;
        if (++flag > 32)
            break;  /* overflow */
    }
    if (flag > 32)
        return -1;
    if (cap_len > buf->_capacity)
    {
        pdata = (uint8_t*)malloc(sizeof(uint8_t) * cap_len);
        memcpy(pdata, buf->_data, buf->_write_pos);
        free(buf->_data);
        buf->_data = pdata;
        buf->_capacity = cap_len;
    }
    return 0;
}
/*---------------------------------------------------------------------------*/
int32_t ReadByte(EdpPacket* pkg, uint8_t* val)
{
    if (pkg->_read_pos + 1 > pkg->_write_pos)
        return -1;
    *val = pkg->_data[pkg->_read_pos];
    pkg->_read_pos += 1;
    return 0;
}
int32_t ReadBytes(EdpPacket* pkg, uint8_t** val, uint32 count)
{
    if (pkg->_read_pos + count > pkg->_write_pos)
        return -1;
    *val = (uint8_t*)malloc(sizeof(uint8_t) * count);
    memcpy(*val, pkg->_data + pkg->_read_pos, count);
    pkg->_read_pos += count;
    return 0;
}
int32_t ReadUint16(EdpPacket* pkg, uint16_t* val)
{
    uint8_t msb, lsb;
    if (pkg->_read_pos + 2 > pkg->_write_pos)
        return -1;
    msb = pkg->_data[pkg->_read_pos];
    pkg->_read_pos++;
    lsb = pkg->_data[pkg->_read_pos];
    pkg->_read_pos++;
    *val = (msb << 8) + lsb;
    return 0;
}
int32_t ReadUint32(EdpPacket* pkg, uint32* val)
{
    int32_t i = 0;
    uint32 tmpval = 0;
    if (pkg->_read_pos + 4 > pkg->_write_pos)
        return -1;
    while (i++ < 4)
    {
        tmpval = (tmpval << 8) | (pkg->_data[pkg->_read_pos]);
        pkg->_read_pos++;
    }
    *val = tmpval;
    return 0;
}
int32_t ReadStr(EdpPacket* pkg, int8_t** val)
{
    uint16_t len = 0;
    int32_t rc = 0;
    /* read str len */
    rc = ReadUint16(pkg, &len);
    if (rc)
        return rc;
    if (pkg->_read_pos + len > pkg->_write_pos)
        return -1;
    /* copy str val */
    *val = malloc(sizeof(char) * (len + 1));
    memset(*val, 0, len + 1);
    strncpy((char *)*val, (const char *)(pkg->_data + pkg->_read_pos), len);
    pkg->_read_pos += len;
    return 0;
}
int32_t ReadRemainlen(EdpPacket* pkg, uint32* len_val)
{
    uint32 multiplier = 1;
    uint32 len_len = 0;
    uint8_t onebyte = 0;
    int32_t rc;
    *len_val = 0;
    do
    {
        rc = ReadByte(pkg, &onebyte);
        if (rc)
            return rc;

        *len_val += (onebyte & 0x7f) * multiplier;
        multiplier *= 0x80;

        len_len++;
        if (len_len > 4)
        {
            return -1;/*len of len more than 4;*/
        }
    }
    while((onebyte & 0x80) != 0);
    return 0;
}
/*---------------------------------------------------------------------------*/
int32_t WriteByte(Buffer* buf, uint8_t byte)
{
    my_assert(buf->_read_pos == 0);
    if (CheckCapacity(buf, 1))
        return -1;
    buf->_data[buf->_write_pos] = byte;
    buf->_write_pos++;
    return 0;
}
int32_t WriteBytes(Buffer* buf, const void* bytes, uint32_t count)
{
    my_assert(buf->_read_pos == 0);
    if (CheckCapacity(buf, count))
        return -1;
    memcpy(buf->_data + buf->_write_pos, bytes, count);
    buf->_write_pos += count;
    return 0;
}
int32_t WriteUint16(Buffer* buf, uint16_t val)
{
    my_assert(buf->_read_pos == 0);
    return WriteByte(buf, MOSQ_MSB(val))
           || WriteByte(buf, MOSQ_LSB(val));
}
int32_t WriteUint32(Buffer* buf, uint32_t val)
{
    my_assert(buf->_read_pos == 0);
    return WriteByte(buf, (val >> 24) & 0x00FF)
           || WriteByte(buf, (val >> 16) & 0x00FF)
           || WriteByte(buf, (val >> 8) & 0x00FF)
           || WriteByte(buf, (val) & 0x00FF);
}
int32_t WriteStr(Buffer* buf, const int8_t *str)
{
    uint16_t length = 0;
    my_assert(buf->_read_pos == 0);
    length = strlen((const char *)str);
    return WriteUint16(buf, length)
           || WriteBytes(buf, str, length);
}
int32_t WriteRemainlen(Buffer* buf, uint32_t len_val)
{
    uint32_t remaining_length = len_val;
    int32_t remaining_count = 0;
    uint8_t byte = 0;

    my_assert(buf->_read_pos == 0);

    do
    {
        byte = remaining_length % 128;
        remaining_length = remaining_length / 128;
        /* If there are more digits to encode, set the top bit of this digit */
        if (remaining_length > 0)
        {
            byte = byte | 0x80;
        }
        buf->_data[buf->_write_pos++] = byte;
        remaining_count++;
    }
    while(remaining_length > 0 && remaining_count < 5);
    my_assert(remaining_count != 5);
    return 0;
}
/*---------------------------------------------------------------------------*/
/* connect1 (C->S): devid + apikey */
EdpPacket* PacketConnect1(const int8_t* devid, const int8_t* auth_key)
{
    uint32_t remainlen;
    EdpPacket* pkg = NewBuffer();
    if(pkg == NULL)
    {
        return NULL;
    }

    /* msg type */
    WriteByte(pkg, CONNREQ);
    /* remain len */
    remainlen = (2 + 3) + 1 + 1 + 2 + (2 + strlen((const char *)devid)) + (2 + strlen((const char *)auth_key));
    WriteRemainlen(pkg, remainlen);
    /* protocol desc */
    WriteStr(pkg, PROTOCOL_NAME);
    /* protocol version */
    WriteByte(pkg, PROTOCOL_VERSION);
    /* connect flag */
    WriteByte(pkg, 0x40);
    /* keep time */
    WriteUint16(pkg, 0x0080);
    /* DEVID */
    WriteStr(pkg, devid);
    /* auth key */
    WriteStr(pkg, auth_key);
    return pkg;
}
/* connect2 (C->S): userid + auth_info */
EdpPacket* PacketConnect2(const int8_t* userid, const int8_t* auth_info)
{
    EdpPacket* pkg = NULL;
    uint32_t remainlen;

    pkg = NewBuffer();
    /* msg type */
    WriteByte(pkg, CONNREQ);
    /* remain len */
    remainlen = (2 + 3) + 1 + 1 + 2 + 2 + (2 + strlen((const char *)userid)) + (2 + strlen((const char *)auth_info));
    WriteRemainlen(pkg, remainlen);
    /* protocol desc */
    WriteStr(pkg, PROTOCOL_NAME);
    /* protocol version */
    WriteByte(pkg, PROTOCOL_VERSION);
    /* connect flag */
    WriteByte(pkg, 0xC0);
    /* keep time */
    WriteUint16(pkg, 0x0080);
    /* devid */
    WriteByte(pkg, 0x00);
    WriteByte(pkg, 0x00);
    /* USERID */
    WriteStr(pkg, userid);
    /* auth info */
    WriteStr(pkg, auth_info);
    return pkg;
}
/* push_data (C->S) */
EdpPacket* PacketPushdata(const int8_t* dst_devid, const int8_t* data, uint32_t data_len)
{
    EdpPacket* pkg = NULL;
    uint32_t remainlen;

    pkg = NewBuffer();
    /* msg type */
    WriteByte(pkg, PUSHDATA);
    /* remain len */
    remainlen = (2 + strlen((const char *)dst_devid)) + data_len;
    WriteRemainlen(pkg, remainlen);
    /* dst devid */
    WriteStr(pkg, dst_devid);
    /* data */
    WriteBytes(pkg, data, data_len);
    return pkg;
}
/* sava_data (C->S) */
EdpPacket* PacketSavedataJson(const int8_t* dst_devid, cJSON* json_obj, int32_t type)
{
    EdpPacket* pkg = NULL;
    uint32_t remainlen = 0;
    int8_t* json_out = NULL;
    uint32_t json_len = 0;

    pkg = NewBuffer();
    json_out =(int8_t *) cJSON_Print(json_obj);
    json_len = strlen((const char *)json_out);

    /* msg type */
    WriteByte(pkg, SAVEDATA);
    if (dst_devid)
    {
        /* remain len */
        remainlen = 1 + (2 + strlen((const char *)dst_devid)) + 1 + (2 + json_len);
        WriteRemainlen(pkg, remainlen);
        /* translate address flag */
        WriteByte(pkg, 0x80);
        /* dst devid */
        WriteStr(pkg, dst_devid);
    }
    else
    {
        /* remain len */
        remainlen = 1 + 1 + (2 + json_len);
        WriteRemainlen(pkg, remainlen);
        /* translate address flag */
        WriteByte(pkg, 0x00);
    }
    /* json flag */
    WriteByte(pkg, type);
    /* json */
    WriteStr(pkg, json_out);
    printf("%s \n", json_out);
    free(json_out);
    return pkg;
}

/*
 * 定义3种JSON格式的打包和解包函数，
 * 每种格式都支持int,double和char*三种类型
 */
MKFUN_PACKET_SAVE_DATA_TYPE1(int, Int, Number)
MKFUN_PACKET_SAVE_DATA_TYPE1(double, Double, Number)
MKFUN_PACKET_SAVE_DATA_TYPE1(const char*, String, String)

MKFUN_PACKET_SAVE_DATA_TYPE2(int, Int, Number)
MKFUN_PACKET_SAVE_DATA_TYPE2(double, Double, Number)
MKFUN_PACKET_SAVE_DATA_TYPE2(const char*, String, String)

MKFUN_PACKET_SAVE_DATA_TYPE3(int, Int, Number)
MKFUN_PACKET_SAVE_DATA_TYPE3(double, Double, Number)
MKFUN_PACKET_SAVE_DATA_TYPE3(const char*, String, String)

MKFUN_PACKET_SAVE_DATA(int, Int)
MKFUN_PACKET_SAVE_DATA(double, Double)
MKFUN_PACKET_SAVE_DATA(const char*, String)

/* 解包函数 */
MKFUN_UNPACK_SAVE_DATA_TYPE1(int, Int, Number)
UNPACK_SAVE_DATA_TYPE1_END_NUMBER(int)
MKFUN_UNPACK_SAVE_DATA_TYPE1(double, Double, Number)
UNPACK_SAVE_DATA_TYPE1_END_NUMBER(double)
MKFUN_UNPACK_SAVE_DATA_TYPE1(char*, String, String)
UNPACK_SAVE_DATA_TYPE1_END_STRING

MKFUN_UNPACK_SAVE_DATA_TYPE2(int, Int, Number)
UNPACK_SAVE_DATA_TYPE23_END_NUMBER(int)
MKFUN_UNPACK_SAVE_DATA_TYPE2(double, Double, Number)
UNPACK_SAVE_DATA_TYPE23_END_NUMBER(double)
MKFUN_UNPACK_SAVE_DATA_TYPE2(char*, String, String)
UNPACK_SAVE_DATA_TYPE23_END_STRING()

MKFUN_UNPACK_SAVE_DATA_TYPE3(int, Int, Number)
UNPACK_SAVE_DATA_TYPE23_END_NUMBER(int)
MKFUN_UNPACK_SAVE_DATA_TYPE3(double, Double, Number)
UNPACK_SAVE_DATA_TYPE23_END_NUMBER(double)
MKFUN_UNPACK_SAVE_DATA_TYPE3(char*, String, String)
UNPACK_SAVE_DATA_TYPE23_END_STRING()

MKFUN_UNPACK_SAVE_DATA(int, Int)
MKFUN_UNPACK_SAVE_DATA(double, Double)
MKFUN_UNPACK_SAVE_DATA(char*, String)

int32_t UnpackSavedataAck(EdpPacket* pkg, int8_t** json_ack)
{
    uint32_t remainlen = 0;
    uint8_t flag = 0;
    uint16_t json_len = 0;

    if (ReadRemainlen(pkg, &remainlen))
        return ERR_UNPACK_SAVEDATA_ACK;

    if (ReadByte(pkg, &flag))
        return ERR_UNPACK_SAVEDATA_ACK;

    if (ReadUint16(pkg, &json_len))
        return ERR_UNPACK_SAVEDATA_ACK;

    if (ReadBytes(pkg, (uint8_t**)(json_ack), json_len))
        return ERR_UNPACK_SAVEDATA_ACK;

    return 0;
}

/* sava_data bin (C->S) */
EdpPacket* PacketSavedataBin(const int8_t* dst_devid,
                             cJSON* desc_obj, const uint8_t* bin_data, uint32_t bin_len)
{
    EdpPacket* pkg = NULL;
    uint32_t remainlen = 0;
    int8_t* desc_out = NULL;
    uint32_t desc_len = 0;

    /* check arguments */
    desc_out =( int8_t*)cJSON_Print(desc_obj);
    desc_len = strlen((const char *)desc_out);
    if (desc_len > (0x01 << 16) || bin_len > (3 * (0x01 << 20))
        /* desc < 2^16 && bin_len < 3M*/
        || cJSON_GetObjectItem(desc_obj, "ds_id") == 0)
        /* desc_obj MUST has ds_id */
    {
        free(desc_out);
        return 0;
    }
    pkg = NewBuffer();
    /* msg type */
    WriteByte(pkg, SAVEDATA);
    if (dst_devid)
    {
        /* remain len */
        remainlen = 1 + (2 + strlen((const char *)dst_devid)) + 1 + (2 + desc_len) + (4 + bin_len);
        WriteRemainlen(pkg, remainlen);
        /* translate address flag */
        WriteByte(pkg, 0x80);
        /* dst devid */
        WriteStr(pkg, dst_devid);
    }
    else
    {
        /* remain len */
        remainlen = 1 + 1 + (2 + desc_len) + (4 + bin_len);
        WriteRemainlen(pkg, remainlen);
        /* translate address flag */
        WriteByte(pkg, 0x00);
    }
    /* bin flag */
    WriteByte(pkg, 0x02);
    /* desc */
    WriteStr(pkg, desc_out);
    free(desc_out);
    /* bin data */
    WriteUint32(pkg, bin_len);
    WriteBytes(pkg, bin_data, bin_len);
    return pkg;
}

EdpPacket* PacketSavedataBinStr(const int8_t* dst_devid,
                                const int8_t* desc_str, const uint8_t* bin_data, uint32_t bin_len)
{
    EdpPacket* pkg = NULL;
    uint32_t remainlen = 0;
    uint32_t desc_len = 0;

    /* check arguments */
    desc_len = strlen((const char *)desc_str);
    if (desc_len > (0x01 << 16) || bin_len > (3 * (0x01 << 20)))
    {
        /* desc < 2^16 && bin_len < 3M*/
        return 0;
    }
    pkg = NewBuffer();
    /* msg type */
    WriteByte(pkg, SAVEDATA);
    if (dst_devid)
    {
        /* remain len */
        remainlen = 1 + (2 + strlen((const char *)dst_devid)) + 1 + (2 + desc_len) + (4 + bin_len);
        WriteRemainlen(pkg, remainlen);
        /* translate address flag */
        WriteByte(pkg, 0x80);
        /* dst devid */
        WriteStr(pkg, dst_devid);
    }
    else
    {
        /* remain len */
        remainlen = 1 + 1 + (2 + desc_len) + (4 + bin_len);
        WriteRemainlen(pkg, remainlen);
        /* translate address flag */
        WriteByte(pkg, 0x00);
    }
    /* bin flag */
    WriteByte(pkg, 0x02);
    /* desc */
    WriteStr(pkg, desc_str);
    /* bin data */
    WriteUint32(pkg, bin_len);
    WriteBytes(pkg, bin_data, bin_len);
    return pkg;
}

EdpPacket* PacketCmdResp(const int8_t* cmdid, uint16_t cmdid_len,
                         const int8_t* resp, uint32_t resp_len)
{
    EdpPacket* send_pkg = NULL;
    unsigned remainlen = 0;

    send_pkg = NewBuffer();
    /* 6 = 2 + 4 = len(cmdid_len) + len(resp_len) */
    remainlen = cmdid_len + resp_len + (resp_len ? 6 : 2);
    WriteByte(send_pkg, CMDRESP);
    WriteRemainlen(send_pkg, remainlen);
    WriteUint16(send_pkg, cmdid_len);
    WriteBytes(send_pkg, cmdid, cmdid_len);
    if (resp_len)
    {
        WriteUint32(send_pkg, resp_len);
        WriteBytes(send_pkg, resp, resp_len);
    }
    return send_pkg;
}
/* ping (C->S) */
EdpPacket* PacketPing(void)
{
    EdpPacket* pkg = NULL;

    pkg = NewBuffer();
    /* msg type */
    WriteByte(pkg, PINGREQ);
    /* remain len */
    WriteRemainlen(pkg, 0);
    return pkg;
}
/*---------------------------------------------------------------------------*/
/* recv stream to a edp packet (S->C) */
EdpPacket* GetEdpPacket(RecvBuffer* buf)
{
    EdpPacket* pkg = NULL;
    int32_t flag = 0;

    flag = IsPkgComplete(buf);
    if (flag <= 0)
    {
        printf("%s flag<=0\n", __func__);
        return pkg;
    }
    pkg = NewBuffer();
    WriteBytes(pkg, buf->_data, flag);
    /* shrink buffer */
    memmove(buf->_data, buf->_data + flag, buf->_write_pos - flag);
    buf->_write_pos -= flag;
    return pkg;
}
/* is the recv buffer has a complete edp packet? */
int32_t IsPkgComplete(RecvBuffer* buf)
{
    uint8_t* data = NULL;
    uint32_t data_len = 0;
    uint32_t multiplier = 1;
    uint32_t len_val = 0;
    uint32_t len_len = 1;
    uint8_t* pdigit = NULL;
    uint32_t pkg_total_len = 0;

    data = buf->_data;
    data_len = buf->_write_pos;

    if (data_len <= 1)
    {
        return 0;   /* continue receive */
    }
    /* recevie remaining len */
    pdigit = data;

    do
    {
        if (len_len > 4)
        {
            return -1;  /* protocol error; */
        }
        if (len_len > data_len - 1)
        {
            return 0;   /* continue receive */
        }
        len_len++;
        pdigit++;
        len_val += ((*pdigit) & 0x7f) * multiplier;
        multiplier *= 0x80;
    }
    while(((*pdigit) & 0x80) != 0);

    pkg_total_len = len_len + len_val;
    /* receive payload */
    if (pkg_total_len <= (uint32_t)data_len)
    {
        //printf("a complete packet len:%d\n", pkg_total_len);
        return pkg_total_len;   /* all data for this pkg is read */
    }
    else
    {
        return 0;   /* continue receive */
    }
}
/* get edp packet type, client should use this type to invoke Unpack??? function */
uint8_t EdpPacketType(EdpPacket* pkg)
{
    uint8_t mtype = 0x00;
    ReadByte(pkg, &mtype);
    return mtype;
}
/* connect_resp (S->C)*/
int32_t UnpackConnectResp(EdpPacket* pkg)
{
    uint8_t flag, rtn;
    uint32_t remainlen;
    if (ReadRemainlen(pkg, &remainlen))
        return ERR_UNPACK_CONNRESP_REMAIN;

    if (ReadByte(pkg, &flag))
        return ERR_UNPACK_CONNRESP_FLAG;
    if (ReadByte(pkg, &rtn))
        return ERR_UNPACK_CONNRESP_RTN;

    my_assert(pkg->_read_pos == pkg->_write_pos);
    return (int32_t)rtn;
}
/* push_data (S->C) */
int32_t UnpackPushdata(EdpPacket* pkg, int8_t** src_devid, int8_t** data, uint32_t* data_len)
{
    uint32_t remainlen;
    if (ReadRemainlen(pkg, &remainlen))
        return ERR_UNPACK_PUSHD_REMAIN;
    if (ReadStr(pkg, src_devid))
        return ERR_UNPACK_PUSHD_DEVID;
    remainlen -= (2 + strlen((const char *)(*src_devid)));
    if (ReadBytes(pkg, (uint8_t**)data, remainlen))
        return ERR_UNPACK_PUSHD_DATA;
    *data_len = remainlen;
    my_assert(pkg->_read_pos == pkg->_write_pos);
    return 0;
}
/* save_data (S->C) */
int32_t UnpackSavedata(EdpPacket* pkg, int8_t** src_devid, uint8_t* jb_flag)
{
    uint32_t remainlen;
    uint8_t ta_flag;
    if (ReadRemainlen(pkg, &remainlen))
        return ERR_UNPACK_SAVED_REMAIN;
    /* translate address flag */
    if (ReadByte(pkg, &ta_flag))
        return ERR_UNPACK_SAVED_TANSFLAG;
    if (ta_flag == 0x80)
    {
        if (ReadStr(pkg, src_devid))
            return ERR_UNPACK_SAVED_DEVID;
    }
    else
    {
        *src_devid = 0;
    }
    /* json or bin */
    if (ReadByte(pkg, jb_flag))
        return ERR_UNPACK_SAVED_DATAFLAG;
    return 0;
}
int32_t UnpackSavedataJson(EdpPacket* pkg, cJSON** json_obj)
{
    int8_t* json_str;
    if (ReadStr(pkg, &json_str))
        return ERR_UNPACK_SAVED_JSON;
    *json_obj = cJSON_Parse((const char *)json_str);
    free(json_str);
    if (*json_obj == 0)
        return ERR_UNPACK_SAVED_PARSEJSON;
    my_assert(pkg->_read_pos == pkg->_write_pos);
    return 0;
}

int32_t UnpackSavedataBin(EdpPacket* pkg, cJSON** desc_obj,
                          uint8_t** bin_data, uint32_t* bin_len)
{
    int8_t* desc_str;
    if (ReadStr(pkg, &desc_str))
        return ERR_UNPACK_SAVED_BIN_DESC;
    *desc_obj = cJSON_Parse((const char *)desc_str);
    free(desc_str);
    if (*desc_obj == 0)
        return ERR_UNPACK_SAVED_PARSEDESC;
    if (ReadUint32(pkg, bin_len))
        return ERR_UNPACK_SAVED_BINLEN;
    if (ReadBytes(pkg, bin_data, *bin_len))
        return ERR_UNPACK_SAVED_BINDATA;
    my_assert(pkg->_read_pos == pkg->_write_pos);
    return 0;
}

int32_t UnpackSavedataBinStr(EdpPacket* pkg, int8_t** desc_str,
                             uint8_t** bin_data, uint32_t* bin_len)
{
    if (ReadStr(pkg, desc_str))
        return ERR_UNPACK_SAVED_BIN_DESC;
    if (ReadUint32(pkg, bin_len))
        return ERR_UNPACK_SAVED_BINLEN;
    if (ReadBytes(pkg, bin_data, *bin_len))
        return ERR_UNPACK_SAVED_BINDATA;
    my_assert(pkg->_read_pos == pkg->_write_pos);
    return 0;
}
int32_t UnpackCmdReq(EdpPacket* pkg, int8_t** cmdid, uint16_t* cmdid_len,
                     int8_t** req, uint32_t* req_len)
{
    uint32 remainlen;
    int32_t rc;
    if (ReadRemainlen(pkg, &remainlen))
        return ERR_UNPACK_CMDREQ;

    rc = ReadUint16(pkg, cmdid_len);
    if (rc)
        return rc;
    if (ReadBytes(pkg, (uint8_t**)cmdid, *cmdid_len))
        return ERR_UNPACK_CMDREQ;

    rc = ReadUint32(pkg, req_len);
    if (rc)
        return rc;
    if (ReadBytes(pkg, (uint8_t**)req, *req_len))
        return ERR_UNPACK_CMDREQ;

    my_assert(pkg->_read_pos == pkg->_write_pos);
    return 0;
}

/* ping_resp (S->C) */
int32_t UnpackPingResp(EdpPacket* pkg)
{
    uint32 remainlen;
    if (ReadRemainlen(pkg, &remainlen))
        return ERR_UNPACK_PING_REMAIN;
    my_assert(pkg->_read_pos == pkg->_write_pos);
    return 0;
}

/* sava_data (C->S) */
EdpPacket* PacketSavedataSimpleString(const int8_t* dst_devid, const int8_t* input)
{
    EdpPacket* pkg = NULL;
    uint32 remainlen = 0;
    uint32 input_len = 0;

    pkg = NewBuffer();
    input_len = strlen((const char *)input);
    /* msg type */
    WriteByte(pkg, SAVEDATA);
    if (dst_devid)
    {
        /* remain len */
        remainlen = 1 + (2 + strlen((const char *)dst_devid)) + 1 + (2 + input_len);
        WriteRemainlen(pkg, remainlen);
        /* translate address flag */
        WriteByte(pkg, 0x80);
        /* dst devid */
        WriteStr(pkg, dst_devid);
    }
    else
    {
        /* remain len */
        remainlen = 1 + 1 + (2 + input_len);
        WriteRemainlen(pkg, remainlen);
        /* translate address flag */
        WriteByte(pkg, 0x00);
    }
    /* json flag */
    WriteByte(pkg, kTypeString);
    /* json */
    WriteStr(pkg, input);

    return pkg;
}

int32_t UnpackSavedataSimpleString(EdpPacket* pkg, int8_t** output)
{
    if (ReadStr(pkg, output))
        return ERR_UNPACK_SAVED_JSON;
    return 0;
}
