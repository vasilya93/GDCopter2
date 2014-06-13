#include "Wire.h"
#include "I2C.h"
#include "stdlib.h"

Wire_Type Wire;

void OperationCompleteHandler(void* operation);

bool Wire_AttachDevice(Wire_Device_Type* newDevice)
{
  if(newDevice == NULL)
  {
    return false;
  }
  
  Wire.DevicesNum++;
  Wire_Device_Type** newPointer = (Wire_Device_Type**)realloc(Wire.Devices, Wire.DevicesNum*sizeof(Wire_Device_Type*));
  
  if(newPointer == NULL)
  {
    Wire.DevicesNum--;
    return false;
  }
  
  Wire.Devices = newPointer;
  Wire.Devices[Wire.DevicesNum - 1] = newDevice;
  
  return true;
}

void Wire_Initialize()
{
  Wire.DevicesNum = 0;
  Wire.CurrentDevice = -1;
  I2C_Begin();
  I2C_OperationComplete_Attach(&OperationCompleteHandler);
}

void Wire_Begin()
{
  if(Wire.DevicesNum != 0)
  {
    Wire.CurrentDevice = 0;
    I2C_OpDescript_Type opDescript;
    char* data = Wire.Devices[Wire.CurrentDevice]->GetNextInitOperation(I2C_DD_NODATA, &opDescript);
    I2C_StartOperation(opDescript, data);
  }
}

void OperationCompleteHandler(void* operation)
{
  I2C_Operation_Type* operationCasted = (I2C_Operation_Type*)operation;
  Wire.Devices[Wire.CurrentDevice]->ProcessOperationResult(operationCasted);
  I2C_OpDescript_Type opDescript;
  char* data = Wire.Devices[Wire.CurrentDevice]->GetNextInitOperation(operationCasted->Description.DataDescript, &opDescript);
  if(opDescript.DataDescript != I2C_DD_NODATA)
  {
    I2C_StartOperation(opDescript, data);
  }
  else if(++Wire.CurrentDevice == Wire.DevicesNum)
  {
    Wire.CurrentDevice = -1;
  }
  else
  {
    if(data != NULL)
    {
      free(data);
    }
    data = Wire.Devices[Wire.CurrentDevice]->GetNextInitOperation(I2C_DD_NODATA, &opDescript); //поменять на рекурсивную операцию
    I2C_StartOperation(opDescript, data);
  }
}