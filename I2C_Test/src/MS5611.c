#include "MS5611.h"
#include "stdint.h"
#include "stdlib.h"
#include "I2C_Device.h"

I2C_Device *MS5611_Device;
uint16_t PressureSens = 0;
uint16_t PressureOff = 0;
uint16_t TempSensCoef = 0;
uint16_t TempOffCoef = 0;
uint16_t ReferenceTemp = 0;
uint16_t TempCoef = 0;
uint32_t RawPres = 0;
uint32_t RawTemp = 0;

I2C_Device *Make_MS5611_Device(void)
{
  I2C_Device *device = malloc(sizeof(I2C_Device));
  if(device != NULL)
  {
    device->GetInitializeCommands = &MS5611_GetInitializeCommands;
    device->DecomposeOperation = &MS5611_DecomposeOperation;
  }
  device->Address = 0xEE;
  device->LastCommand = 0;
  return device;
}

I2C_Operation* MS5611_GetInitializeCommands(void *device, uint8_t *operationsAmount)
{
  *operationsAmount = MS5611_INIT_OPERATIONS_AMOUNT;
  I2C_Operation *operations = calloc(*operationsAmount, sizeof(I2C_Operation));
  for(int i=0;i<*operationsAmount;i+=2)
  {
    operations[i].BytesAmount = 1;
    operations[i].BytesCounter = 0;
    operations[i].DeviceAddress = 0xEE;
    operations[i].DataBytes = malloc(sizeof(uint8_t));
  }
  for(int i=1;i<*operationsAmount;i+=2)
  {
    operations[i].BytesAmount = 2;
    operations[i].BytesCounter = 0;
    operations[i].DeviceAddress = 0xEF;
    operations[i].DataBytes = calloc(2, sizeof(uint8_t));
  }
  operations[0].DataBytes[0] = MS5611_READ_SENS_T1;
  operations[2].DataBytes[0] = MS5611_READ_OFF_T1;
  operations[4].DataBytes[0] = MS5611_READ_TCS;
  operations[6].DataBytes[0] = MS5611_READ_TCO;
  operations[8].DataBytes[0] = MS5611_READ_TREF;
  operations[10].DataBytes[0] = MS5611_READ_TEMPSENS;
  return operations;
}

void MS5611_DecomposeOperation(void *device, I2C_Operation operation)
{
  switch(((I2C_Device*)device)->LastCommand)
  {
  case MS5611_READ_SENS_T1:
    PressureSens |= operation.DataBytes[0] << 8;
    PressureSens |= operation.DataBytes[1];
    break;
  case MS5611_READ_OFF_T1:
    PressureOff |= operation.DataBytes[0] << 8;
    PressureOff |= operation.DataBytes[1];
    break;
  case MS5611_READ_TCS:
    TempSensCoef |= operation.DataBytes[0] << 8;
    TempSensCoef |= operation.DataBytes[1];
    break;
  case MS5611_READ_TCO:
    TempOffCoef |= operation.DataBytes[0] << 8;
    TempOffCoef |= operation.DataBytes[1];
    break;
  case MS5611_READ_TREF:
    ReferenceTemp |= operation.DataBytes[0] << 8;
    ReferenceTemp |= operation.DataBytes[1];
    break;
  case MS5611_READ_TEMPSENS:
    TempCoef |= operation.DataBytes[0] << 8;
    TempCoef |= operation.DataBytes[1];
    break;
  case MS5611_ADC_READ_TEMP:
    RawTemp |= operation.DataBytes[0] << 16;
    RawTemp |= operation.DataBytes[1] << 8;
    RawTemp |= operation.DataBytes[2];
    break;
  /*case MS5611_ADC_READ_PRES:
    RawPres |= operation.DataBytes[0] << 16;
    RawPres |= operation.DataBytes[1] << 8;
    RawPres |= operation.DataBytes[2];
    break;*/
  }
}