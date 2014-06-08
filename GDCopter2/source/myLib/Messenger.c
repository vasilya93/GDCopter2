#include "Messenger.h"
#include "Serial.h"

void Messenger_SendByte(uint8_t message)
{
  Serial_WriteByte(MSNR_MT_BYTE);
  Serial_WriteByte(0);
  Serial_WriteByte(0);
  Serial_WriteByte(0);
  Serial_WriteByte(message);
}

void Messenger_SendWord(uint16_t message)
{
  Serial_WriteByte(MSNR_MT_WORD);
  Serial_WriteByte(0);
  Serial_WriteByte(0);
  Serial_WriteInt16(message);
}

void Messenger_SendDWord(uint32_t message)
{
  Serial_WriteByte(MSNR_MT_DWRD);
  Serial_WriteInt32(message);
}