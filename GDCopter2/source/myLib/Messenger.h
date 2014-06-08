#ifndef MESSENGER_H
#define MESSENGER_H

#include "Serial.h"

#define MSNR_MT_BYTE 1
#define MSNR_MT_WORD 2
#define MSNR_MT_DWRD 3

void Messenger_SendByte(uint8_t);

void Messenger_SendWord(uint16_t);

void Messenger_SendDWord(uint32_t);

#endif