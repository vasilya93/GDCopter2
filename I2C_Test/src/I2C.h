#include "stdbool.h"
#include "stdint.h"

#ifndef I2C_H
#define I2C_H

#define I2C_SENSORS_INITIALIZE 0x01

bool I2C_SetStart(bool IsOperationChanged);
void I2C_SendAddress();
void I2C_OperateTransmitter();
void I2C_OperateReceiver();
void I2C_TransmitRegisterEmpty();
void I2C_ReceiveRegisterNotEmpty();
void I2C_WriteLastCommandToDevice();
void I2C_DecomposeCurrentOperation();
void I2C_InitializeSensors();

extern uint8_t I2C1_Status;

#endif