#include "I2C.h"
#include "stm32l1xx.h"
#include "I2C_Buffer.h"
#include "I2C_Operation.h"
#include "Buffer.h"
#include "I2C_Device.h"
#include "MS5611.h"

uint8_t I2C1_Status;

bool I2C_SetStart(bool isOperationChanged)
{
  if((~I2C1->SR2 & I2C_SR2_BUSY) && (~I2C1_Buffer.Status & I2C_BUFFER_STATUS_EMPTY))
  {
    I2C1->CR1 |= I2C_CR1_START;
    if(isOperationChanged)
    {
      free(I2C1_CurrentOperation.DataBytes);
      I2C1_CurrentOperation = I2C_Pop(&I2C1_Buffer);
    }
    return true;
  }
  else if(I2C1_Buffer.Status & I2C_BUFFER_STATUS_EMPTY)
  {
    I2C1_Status &= ~I2C_SENSORS_INITIALIZE;
    return false;
  }
  else 
  {
    PushSingleByte(&USART_Buffer, MESSAGE_I2C_BUSY);
    return false;
  }
}

//-----------------------------------------------------------------------------
void I2C_SendAddress()
{
  I2C1->DR = I2C1_CurrentOperation.DeviceAddress;
  PushSingleByte(&USART_Buffer, MESSAGE_I2C_START_SUCC);
}
//-----------------------------------------------------------------------------
void I2C_OperateTransmitter()
{
  PushSingleByte(&USART_Buffer, MESSAGE_I2C_TRANSMITTER);
  I2C1->DR = I2C1_CurrentOperation.DataBytes[0];
}
//-----------------------------------------------------------------------------
void I2C_OperateReceiver()
{
  I2C1->CR1 |= I2C_CR1_ACK;
  PushSingleByte(&USART_Buffer, MESSAGE_I2C_RECEIVER);
}
//-----------------------------------------------------------------------------
void I2C_TransmitRegisterEmpty()
{
  //записать переданную команду в LastCommand устройства
  I2C_WriteLastCommandToDevice();
  if(++I2C1_CurrentOperation.BytesCounter < I2C1_CurrentOperation.BytesAmount)
  {
    I2C1->DR = I2C1_CurrentOperation.DataBytes[I2C1_CurrentOperation.BytesCounter];
  }
  else if(I2C1->SR1 & I2C_SR1_BTF)
  {
    I2C1->CR1 |= I2C_CR1_STOP;//буффер в i2c сложнее - нельзя терять данные, портится логика работы, либо выделять логические блоки и их буфферизировать
    PushSingleByte(&USART_Buffer, MESSAGE_I2C_COMMAND_TRANS); //не ставить старт, если переданная команда - преобразования 
    if(I2C1_CurrentOperation.DataBytes[I2C1_CurrentOperation.BytesAmount-1] != MS5611_CONV_TEMP_512)
    {
      I2C_SetStart(true);
    }
  }
}
//-----------------------------------------------------------------------------
void I2C_ReceiveRegisterNotEmpty()
{
  PushSingleByte(&USART_Buffer, MESSAGE_I2C_BYTE_REC);
  I2C1_CurrentOperation.BytesCounter += 1;
  I2C1_CurrentOperation.DataBytes[I2C1_CurrentOperation.BytesCounter - 1] = I2C1->DR;
  uint8_t bytesLeft = I2C1_CurrentOperation.BytesAmount - I2C1_CurrentOperation.BytesCounter;
  switch(bytesLeft)
  {
  case 1:
    I2C1->CR1 &= ~I2C_CR1_ACK;
    I2C1->CR1 |= I2C_CR1_STOP;
    break;
  case 0:
    I2C_DecomposeCurrentOperation();
    I2C_SetStart(true);
    break;
  }
}
//-----------------------------------------------------------------------------
void I2C_DecomposeCurrentOperation()
{
  switch(I2C1_CurrentOperation.DeviceAddress)
  {
  case MS5611_READ_ADDRESS:
    MS5611_Device->DecomposeOperation(MS5611_Device, I2C1_CurrentOperation);
    break;
  }
}
//-----------------------------------------------------------------------------
void I2C_WriteLastCommandToDevice()
{
  switch(I2C1_CurrentOperation.DeviceAddress)
  {
  case MS5611_READ_ADDRESS:
    MS5611_Device->LastCommand = I2C1_CurrentOperation.DataBytes[I2C1_CurrentOperation.BytesCounter];
    break;
  }
}
//-----------------------------------------------------------------------------
void I2C_InitializeSensors()
{
  MS5611_Device = Make_MS5611_Device();
  uint8_t operationsAmount;
  I2C_Operation *initOperations = (MS5611_Device->GetInitializeCommands)(MS5611_Device, &operationsAmount);
  
  for(int i = 0; i<operationsAmount; i++)
  {
    I2C_Push(&I2C1_Buffer, initOperations[i]);
  }
  I2C_SetStart(true);
  
  I2C1_Status |= I2C_SENSORS_INITIALIZE;
  while(I2C1_Status & I2C_SENSORS_INITIALIZE);
  
  I2C_PushFirstCom(&I2C1_Buffer);
}
//-----------------------------------------------------------------------------