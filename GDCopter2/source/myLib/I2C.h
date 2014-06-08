#ifndef I2C_H
#define I2C_H

#define I2C_DATA_SIZE 10

#define I2C_DD_NODATA 1

#define I2C_ST_BUSY 1

#define I2C_MSG_STRTST 1

#include "stdbool.h"
#include "stdint.h"

typedef struct I2C_OpDescript_Type
{
  uint16_t DataDescript; 
  uint8_t DeviceAddress;
  bool IsRead;
  uint8_t BytesNum;  
}I2C_OpDescript_Type;

typedef struct I2C_Operation_Type
{
  I2C_OpDescript_Type Description;
  uint8_t State;
  uint8_t* Bytes;  
  
  void (**OperationCompleteHandlers)(void* operation);
  uint8_t OCH_Amount;
}I2C_Operation_Type;

void I2C_Begin();

bool I2C_OperationComplete_Attach(void (*handler)(void* operation));

bool I2C_StartOperation(I2C_OpDescript_Type);

#endif
