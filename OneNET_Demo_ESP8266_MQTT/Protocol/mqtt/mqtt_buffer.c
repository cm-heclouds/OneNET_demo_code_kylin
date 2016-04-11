#include "mqtt_buffer.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "mqtt.h"

static const uint32_t MQTT_MIN_EXTENT_SIZE = 256;

void MqttBuffer_Init(struct MqttBuffer *buf)
{
    buf->first_ext = NULL;
    buf->last_ext = NULL;
    buf->available_bytes = 0;
    buf->allocations = NULL;
    buf->alloc_count = 0;
    buf->alloc_max_count = 0;
    buf->first_available = NULL;
    buf->buffered_bytes = 0;
}

void MqttBuffer_Destroy(struct MqttBuffer *buf)
{
    MqttBuffer_Reset(buf);
}

void MqttBuffer_Reset(struct MqttBuffer *buf)
{
    uint32_t i;
    //printf("%s %d\n",__func__,__LINE__);
    for(i = 0; i < buf->alloc_count; ++i)
    {
        free(buf->allocations[i]);
        buf->allocations[i] = NULL;
    }

    if(buf->allocations)
    {
        free(buf->allocations);//bug
        buf->allocations = NULL;
    }

    MqttBuffer_Init(buf);
}

struct MqttExtent *MqttBuffer_AllocExtent(struct MqttBuffer *buf, uint32_t bytes)
{
    struct MqttExtent *ext;

    bytes += sizeof(struct MqttExtent);

    if(buf->available_bytes < bytes)
    {
        uint32_t alloc_bytes;
        char *chunk;

        if(buf->alloc_count == buf->alloc_max_count)
        {
            uint32_t max_count = buf->alloc_max_count * 2 + 1;
            //  printf(" alloc %s %d 1th_size=%d,al_cnt=%d:al_max_cnt:%d\n",__func__,__LINE__,max_count * sizeof(char**),buf->available_bytes,buf->alloc_count,buf->alloc_max_count);
            char **tmp = (char**)malloc(max_count * sizeof(char**));
            if(NULL == tmp)
            {
                printf("%s %d avail bytes:%d\n", __func__, __LINE__, buf->available_bytes);
                return NULL;
            }
            //      printf("%s %d avail bytes:%d,allocations_addr=0x%x,tmp=0x%x\n",__func__,__LINE__,buf->available_bytes,buf->allocations,tmp);
            memset(tmp, 0, max_count * sizeof(char**));
            if(buf->allocations)
            {
                memcpy(tmp, buf->allocations, buf->alloc_max_count);// buf->allocations always null here
                free(buf->allocations);//bug
            }
            buf->alloc_max_count = max_count;
            buf->allocations = tmp;
        }

        alloc_bytes = bytes < MQTT_MIN_EXTENT_SIZE ? MQTT_MIN_EXTENT_SIZE : bytes;
        //  printf("alloc %s %d bytes bytes:%d alloc_bytes:%d,true or false %d\n",__func__,__LINE__,bytes,alloc_bytes,bytes < MQTT_MIN_EXTENT_SIZE);
        chunk = (char*)malloc(alloc_bytes);//too big
        if(NULL == chunk)
        {
            return NULL;
        }

        buf->alloc_count += 1;
        buf->allocations[buf->alloc_count - 1] = chunk;
        buf->available_bytes = alloc_bytes;
        buf->first_available = chunk;
        // printf("%s %d avail bytes:%d alloc_bytes=%d,chunk_addr=0x%x\n",__func__,__LINE__,buf->available_bytes,alloc_bytes,chunk);
    }

    //assert(buf->available_bytes >= bytes);
    //  assert(buf->alloc_count > 0);

    ext = (struct MqttExtent*)(buf->first_available);
    ext->len = bytes - sizeof(struct MqttExtent);
    ext->payload = buf->first_available + sizeof(struct MqttExtent);
    ext->next = NULL;

    buf->first_available += bytes;
    buf->available_bytes -= bytes;

    return ext;
}

int MqttBuffer_Append(struct MqttBuffer *buf, char *payload, uint32_t size, int own)
{
    const uint32_t bytes = own ? size : sizeof(struct MqttExtent);

    struct MqttExtent *ext = MqttBuffer_AllocExtent(buf, bytes);
    if(NULL == ext)
    {
        return MQTTERR_OUTOFMEMORY;
    }

    if(own)
    {
        ext->payload = ((char*)ext) + sizeof(struct MqttExtent);
        memcpy(ext->payload, payload, size);
    }
    else
    {
        ext->payload = payload;
        ext->len = size;
    }

    MqttBuffer_AppendExtent(buf, ext);
    return MQTTERR_NOERROR;
}

void MqttBuffer_AppendExtent(struct MqttBuffer *buf, struct MqttExtent *ext)
{
    ext->next = NULL;
    if(NULL != buf->last_ext)
    {
        buf->last_ext->next = ext;
        buf->last_ext = ext;
    }
    else
    {
        //assert(NULL == buf->first_ext);
        // assert(1 <= buf->alloc_count);

        buf->first_ext = ext;
        buf->last_ext = ext;
    }

    buf->buffered_bytes += ext->len;
}
