#include "stdint.h"
#include "I2C_Operation.h"

#ifndef I2C_BUFFER_H
#define I2C_BUFFER_H

#define I2C_BUFFER_STATUS_FULL 0x01
#define I2C_BUFFER_STATUS_EMPTY 0x02
#define I2C_BUFFER_SIZE 12

typedef struct I2C_Buffer
{
  I2C_Operation Array[I2C_BUFFER_SIZE];
  uint16_t IndexFirst; //index of the first element added into buffer
  uint16_t IndexLast; //index of the last element into element
  uint16_t FreeElementsAmount;
  uint8_t Status;
}I2C_Buffer;

void I2C_Push(I2C_Buffer *buffer, I2C_Operation operation);

I2C_Operation I2C_Pop(I2C_Buffer *buffer);

void I2C_IncrementIndex(uint16_t *index);

void I2C_InitBuffer(I2C_Buffer *buffer);

void I2C_PushFirstCom(I2C_Buffer *buffer);

extern I2C_Buffer I2C1_Buffer;

#endif