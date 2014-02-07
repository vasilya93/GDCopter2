#include "stdint.h"
#include "I2C_Operation.h"

#ifndef I2C_DEVICE_H
#define I2C_DEVICE_H

typedef struct I2C_Device
{
  uint8_t Address;
  uint8_t LastCommand;
  I2C_Operation* (*GetInitializeCommands)(void *self, uint8_t *operationsAmount);
  void (*DecomposeOperation)(void *self, I2C_Operation operation);
}I2C_Device;
//можно создать динамический массив с именами всех команд и при инициализации запихивать их туда
//по идее в последствии доступ быстрее будет
#endif