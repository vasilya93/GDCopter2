#include "stdint.h"
#include "stdlib.h"

#ifndef I2C_OPERATION_H  //типа из операции неплохо бы узнавать об устройстве
#define I2C_OPERATION_H  //есть операция чтения с данными для устройства
                         //нужно знать, что за утстройство, и в какой его регистр нужно положить значения
                         //свичевать адрес устройства и передавать соответствующему устройству

typedef struct I2C_Operation
{
  uint8_t DeviceAddress;
  uint8_t BytesAmount;
  uint8_t BytesCounter;
  uint8_t *DataBytes;
}I2C_Operation;

void I2C_InitOperation(I2C_Operation *operation);

extern I2C_Operation I2C1_CurrentOperation;

#endif