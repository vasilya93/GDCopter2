#include "stdint.h"
#include "I2C_Device.h"
#include "I2C_Operation.h"

#ifndef MS5611_h
#define MS5611_h

#define MS5611_INIT_OPERATIONS_AMOUNT 12

#define MS5611_WRITE_ADDRESS 0xEE
#define MS5611_READ_ADDRESS 0xEF
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

I2C_Device *Make_MS5611_Device(void);
I2C_Operation* MS5611_GetInitializeCommands(void *device, uint8_t *operationsAmount);
void MS5611_DecomposeOperation(void *device, I2C_Operation operation); 

extern I2C_Device *MS5611_Device;
extern uint16_t PressureSens;
extern uint16_t PressureOff;
extern uint16_t TempSensCoef;
extern uint16_t TempOffCoef;
extern uint16_t ReferenceTemp;
extern uint16_t TempCoef;
extern uint32_t RawPres;
extern uint32_t RawTemp;

#endif