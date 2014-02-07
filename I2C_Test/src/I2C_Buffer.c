#include "I2C_Buffer.h"
#include "stdint.h"
#include "I2C_Operation.h"
#include "MS5611.h"

I2C_Buffer I2C1_Buffer;

//а можно ли унифицировать - тут память нужно освобождать;

void I2C_Push(I2C_Buffer *buffer, I2C_Operation operation)
{
  if(buffer->Status & I2C_BUFFER_STATUS_FULL) //если мы до этого знали, что буффер полон - увелчичли индекс первого места
  {
    I2C_IncrementIndex(&buffer->IndexFirst);
    free(buffer->Array[buffer->IndexLast].DataBytes); //освободили память, выделямую под байты данных замещаемой операции
  }
  else
  {
    buffer->FreeElementsAmount--;
  }
  buffer->Array[buffer->IndexLast] = operation;//записали новый элемент на индексированное посл место
  buffer->Status &= ~I2C_BUFFER_STATUS_EMPTY;//буффер точно не пуст
  I2C_IncrementIndex(&buffer->IndexLast);//увеличили индекс последнего места
  if(buffer->IndexFirst == buffer->IndexLast)
  {
    buffer->Status &= I2C_BUFFER_STATUS_FULL;
  }
}

I2C_Operation I2C_Pop(I2C_Buffer *buffer)
{
  buffer->Status &= ~I2C_BUFFER_STATUS_FULL;
  if(buffer->Status & I2C_BUFFER_STATUS_EMPTY)
  {
    I2C_Operation emptyOperation;
    I2C_InitOperation(&emptyOperation);
    return emptyOperation;
  }
  else
  {
    buffer->FreeElementsAmount++;
    I2C_Operation operation = buffer->Array[buffer->IndexFirst];
    I2C_IncrementIndex(&buffer->IndexFirst);
    if(buffer->IndexFirst == buffer->IndexLast)
    {
      buffer->Status |= I2C_BUFFER_STATUS_EMPTY;
    }
    return operation;
  }
}

//выше - переписать, вставить где-нить инициализацию буффера

void I2C_IncrementIndex(uint16_t *index) //снести в одно с обычным буффером
{
  *index += 1;
  if(*index == I2C_BUFFER_SIZE)
  {
    *index = 0;
  }
}

void I2C_InitBuffer(I2C_Buffer *buffer)
{
  buffer->IndexFirst = 0;
  buffer->IndexLast = 0;
  buffer->FreeElementsAmount = I2C_BUFFER_SIZE;
  buffer->Status = I2C_BUFFER_STATUS_EMPTY;
}

void I2C_PushFirstCom(I2C_Buffer *buffer)
{
  I2C_Operation newOperation;
  newOperation.DeviceAddress = MS5611_WRITE_ADDRESS;
  newOperation.BytesAmount = 1;
  newOperation.BytesCounter = 0;
  newOperation.DataBytes = calloc(newOperation.BytesAmount,sizeof(uint8_t));
  newOperation.DataBytes[0] = MS5611_CONV_TEMP_512;
  I2C_Push(buffer, newOperation);
}