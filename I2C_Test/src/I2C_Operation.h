#include "stdint.h"
#include "stdlib.h"

#ifndef I2C_OPERATION_H  //���� �� �������� ������� �� �������� �� ����������
#define I2C_OPERATION_H  //���� �������� ������ � ������� ��� ����������
                         //����� �����, ��� �� �����������, � � ����� ��� ������� ����� �������� ��������
                         //��������� ����� ���������� � ���������� ���������������� ����������

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