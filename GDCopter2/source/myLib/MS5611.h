#ifndef MS5611_h
#define MS5611_h

//31-35 message codes are for MS5611
#define MS5611_MSG_NODATA 31
#define MS5611_MSG_SENST1REQ 32
#define MS5611_MSG_SENST1 33
#define MS5611_MSG_OFFT1REQ 34
#define MS5611_MSG_OFFT1 35


#define MS5611_ADDRESS 0x77

#define MS5611_RESET 0x1E
#define MS5611_CONV_PRES_256 0x40
#define MS5611_CONV_PRES_512 0x42
#define MS5611_CONV_PRES_1024 0x44
#define MS5611_CONV_PRES_2048 0x46
#define MS5611_CONV_PRES_4096 0x48
#define MS5611_CONV_TEMP_256 0x50
#define MS5611_CONV_TEMP_512 0x52
#define MS5611_CONV_TEMP_1024 0x54
#define MS5611_CONV_TEMP_2048 0x56
#define MS5611_CONV_TEMP_4096 0x58
#define MS5611_ADC_READ 0x00
#define MS5611_ADC_READ_PRES 0x00
#define MS5611_ADC_READ_TEMP 0x00
#define MS5611_READ_SENS_T1 0xA2
#define MS5611_READ_OFF_T1 0xA4
#define MS5611_READ_TCS 0xA6
#define MS5611_READ_TCO 0xA8
#define MS5611_READ_TREF 0xAA
#define MS5611_READ_TEMPSENS 0xAC

#define MS5611_DD_PC256REQ 1
#define MS5611_DD_PC512REQ 2
#define MS5611_DD_PC1024REQ 3
#define MS5611_DD_PC2048REQ 4
#define MS5611_DD_PC4096REQ 5
#define MS5611_DD_TC256REQ 6
#define MS5611_DD_TC512REQ 7
#define MS5611_DD_TC1024REQ 8
#define MS5611_DD_TC2048REQ 9
#define MS5611_DD_TC4096REQ 10
#define MS5611_DD_TEMPREQ 11
#define MS5611_DD_PRESREQ 12
#define MS5611_DD_TEMP 13
#define MS5611_DD_PRES 14

#define MS5611_DD_SENST1REQ 15
#define MS5611_DD_OFFT1REQ 16
#define MS5611_DD_TCSREQ 17
#define MS5611_DD_TCOREQ 18
#define MS5611_DD_TREFREQ 19
#define MS5611_DD_TSREQ 20

#define MS5611_DD_SENST1 21
#define MS5611_DD_OFFT1 22
#define MS5611_DD_TCS 23
#define MS5611_DD_TCO 24
#define MS5611_DD_TREF 25
#define MS5611_DD_TS 26

#include "Wire.h"

extern Wire_Device_Type MS5611;

void MS5611_Initialize(Wire_Device_Type*);

char* MS5611_GetNextInitOperation(uint16_t currentDataDescript, I2C_OpDescript_Type* opDescript);
char* MS5611_GetNextRegOperation(uint16_t currentDataDescript, I2C_OpDescript_Type* opDescript);
void MS5611_ProcessOperationResult(I2C_Operation_Type* operation);

typedef struct MS5611_Data_Type
{
  uint16_t PressureSens;
  uint16_t PressureOff;
  uint16_t TempSensCoef;
  uint16_t TempOffCoef;
  uint16_t ReferenceTemp;
  uint16_t TempCoef;
  uint32_t RawPres;
  uint32_t RawTemp;
}MS5611_Data_Type;

#endif