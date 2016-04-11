#ifndef __UTILS_HEADER__
#define __UTILS_HEADER__
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

/*
 * buf按十六进制输出
 */
void hexdump(const uint8_t *buf, uint32_t num);
/*
*16进制数据转字符
*/
uint8_t prvucMBBIN2CHAR(uint8_t ucByte);

void uDelay(uint32_t i);

void mDelay(uint32_t i);
/*
 *  @brief MODBU协议要求的CRC16校验
 */
uint16_t usMBCRC16(uint8_t * pucFrame, uint16_t usLen);
#endif
