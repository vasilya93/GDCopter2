#include "MS5611.h"
#include "Wire.h"
#include "I2C.h"
#include "stdlib.h"
#include "Messenger.h"

MS5611_Data_Type MS5611_Data;
Wire_Device_Type MS5611;

void MS5611_Initialize(Wire_Device_Type* device)
{
  device->GetNextInitOperation = &MS5611_GetNextInitOperation;
  device->GetNextRegOperation = &MS5611_GetNextRegOperation;
  device->ProcessOperationResult = &MS5611_ProcessOperationResult;
}

char* MS5611_GetNextInitOperation(uint16_t currentDataDescript, I2C_OpDescript_Type* opDescript)
{
  switch(currentDataDescript)
  {
  case WIRE_DD_NODATA:
    {
      I2C_SetOpDescription(opDescript, MS5611_DD_SENST1REQ, MS5611_ADDRESS, false, 1);
      char* data = malloc(1);
      data[0] = MS5611_READ_SENS_T1;
      return data;
    }
    break;
  case MS5611_DD_SENST1REQ:
    {
      I2C_SetOpDescription(opDescript, MS5611_DD_SENST1, MS5611_ADDRESS, true, 2);
      return NULL;
    }
    break;
  case MS5611_DD_SENST1:
    {
      I2C_SetOpDescription(opDescript, MS5611_DD_OFFT1REQ, MS5611_ADDRESS, false, 1);
      char* data = malloc(1);
      data[0] = MS5611_READ_OFF_T1;
      return data;    
    }
    break;
  case MS5611_DD_OFFT1REQ:
    {
      I2C_SetOpDescription(opDescript, MS5611_DD_OFFT1, MS5611_ADDRESS, true, 2);
      return NULL;
    }
    break;
  case MS5611_DD_OFFT1:
    {
      I2C_SetOpDescription(opDescript, MS5611_DD_TCSREQ, MS5611_ADDRESS, false, 1);
      char* data = malloc(1);
      data[0] = MS5611_READ_TCS;
      return data;
    }
    break;
  case MS5611_DD_TCSREQ:
    {
      I2C_SetOpDescription(opDescript, MS5611_DD_TCS, MS5611_ADDRESS, true, 2);
      return NULL;
    }
    break;
  case MS5611_DD_TCS:
    {
      I2C_SetOpDescription(opDescript, MS5611_DD_TCOREQ, MS5611_ADDRESS, false, 1);
      char* data = malloc(1);
      data[0] = MS5611_READ_TCO;
      return data;
    }
    break;
  case MS5611_DD_TCOREQ:
    {
      I2C_SetOpDescription(opDescript, MS5611_DD_TCO, MS5611_ADDRESS, true, 2);
      return NULL;
    }
    break;
  case MS5611_DD_TCO:
    {
      I2C_SetOpDescription(opDescript, MS5611_DD_TREFREQ, MS5611_ADDRESS, false, 1);
      char* data = malloc(1);
      data[0] = MS5611_READ_TREF;
      return data;
    }
    break;
  case MS5611_DD_TREFREQ:
    {
      I2C_SetOpDescription(opDescript, MS5611_DD_TREF, MS5611_ADDRESS, true, 2);
      return NULL;
    }
    break;
  case MS5611_DD_TREF:
    {
      I2C_SetOpDescription(opDescript, MS5611_DD_TSREQ, MS5611_ADDRESS, false, 1);
      char* data = malloc(1);
      data[0] = MS5611_READ_TEMPSENS;
      return data;
    }
    break;
  case MS5611_DD_TSREQ:
    {
      I2C_SetOpDescription(opDescript, MS5611_DD_TS, MS5611_ADDRESS, true, 2);
      return NULL;
    }
    break;
  case MS5611_DD_TS:
    {
      I2C_SetOpDescription(opDescript, 0, 0, false, 0);
      return NULL;
    }
    break;
  default:
    {
      I2C_SetOpDescription(opDescript, 0, 0, false, 0);
      return NULL;
    }
    break;
  }
}

char* MS5611_GetNextRegOperation(uint16_t currentDataDescript, I2C_OpDescript_Type* opDescript)
{
  return "";
}

void MS5611_ProcessOperationResult(I2C_Operation_Type* operation)
{
  switch(operation->Description.DataDescript)
  {
  case MS5611_DD_SENST1:
    MS5611_Data.PressureSens = (operation->Bytes[0] << 8) | operation->Bytes[1];
    Messenger_SendWord(MS5611_Data.PressureSens);
    break;
  case MS5611_DD_OFFT1:
    MS5611_Data.PressureOff = (operation->Bytes[0] << 8) | operation->Bytes[1];
    Messenger_SendWord(MS5611_Data.PressureOff);
    break;
  case MS5611_DD_TCS:
    MS5611_Data.TempSensCoef = (operation->Bytes[0] << 8) | operation->Bytes[1];
    Messenger_SendWord(MS5611_Data.TempSensCoef);
    break;
  case MS5611_DD_TCO:
    MS5611_Data.TempOffCoef = (operation->Bytes[0] << 8) | operation->Bytes[1];
    Messenger_SendWord(MS5611_Data.TempOffCoef);
    break;
  case MS5611_DD_TREF:
    MS5611_Data.ReferenceTemp = (operation->Bytes[0] << 8) | operation->Bytes[1];
    Messenger_SendWord(MS5611_Data.ReferenceTemp);
    break;
  case MS5611_DD_TS:
    MS5611_Data.TempCoef = (operation->Bytes[0] << 8) | operation->Bytes[1];
    Messenger_SendWord(MS5611_Data.TempCoef);
    break;
  default:
    break;
  }
}