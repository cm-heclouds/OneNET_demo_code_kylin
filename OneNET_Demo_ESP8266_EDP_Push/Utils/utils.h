#ifndef __UTILS_HEADER__
#define __UTILS_HEADER__
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void hexdump(const unsigned char *buf, uint32_t);
unsigned char prvucMBBIN2CHAR(unsigned char ucByte);
void Delay(unsigned int i);
unsigned short usMBCRC16( unsigned char * pucFrame, unsigned short  usLen );
void mDelay(uint32_t i);
void uDelay(uint32_t i);
#endif

