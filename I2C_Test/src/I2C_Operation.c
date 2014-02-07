#include "I2C_Operation.h"

I2C_Operation I2C1_CurrentOperation;

void I2C_InitOperation(I2C_Operation *operation)
{
  operation->DeviceAddress = 0;
  operation->BytesAmount = 0;
  operation->BytesCounter = 0;
  operation->DataBytes = malloc(0);
}