#include "stdint.h" //define custom types like uint8_t, uint16_t 

#ifndef BUFFER_H
#define BUFFER_H

#define BUFFER_STATUS_FULL 0x01
#define BUFFER_STATUS_EMPTY 0x02
#define BUFFER_SIZE 256
#define PACKET_SIZE 4

#define MESSAGE_I2C_START_SET 0x01
#define MESSAGE_I2C_BUSY 0x02
#define MESSAGE_I2C_START_SUCC 0x03
#define MESSAGE_I2C_NO_ACK 0x04
#define MESSAGE_I2C_TRANSMITTER 0x05
#define MESSAGE_I2C_RECEIVER 0x06
#define MESSAGE_I2C_COMMAND_TRANS 0x07
#define MESSAGE_I2C_BYTE_REC 0x08
//�����������, ��� �������
//������ ��������� ������, �� ������ ����� ������ � ��������, ��� ���������
//5 ���� ������; 1�� ���� - ��������� ����������, ��������� ������ - ������
//���� - ��� ������: ������ ���. ����� (���������� � ��������)
//                   ��� ��� ����� (������������� ������ ���������)
//                   ��� ����� ������ - 32 ��� ��������
//������ ������ ���0, ���1 - �������� 0x01 - ������ �����, 0x02 - ��� �����, 0x03 - �����
//��� ����� �������?
//���� �������, ��� ���� ����������� ������������ ���(� ������ �� �� ������-�� ����������)
//��������� ����� ��������� ���������� � �������� ����������; ����� �������� � ������ ������� ���� ������
//� ������ ������ ������� ����� ��������� ��� � ������ ��� ����� ������
//�� ���� ������ I2C ���-�� ����

typedef struct WordsToBytes
{
  union
  {
    uint16_t Words[PACKET_SIZE/2];
    uint8_t Bytes[PACKET_SIZE];
  };
  uint8_t WordsCounter;
}WordsToBytes;

typedef struct _32ToBytes
{
  union
  {
    uint32_t Integer;
    uint8_t Bytes[PACKET_SIZE];
  };
}_32ToBytes;

typedef struct Buffer
{
  uint8_t Array[BUFFER_SIZE];
  uint16_t IndexFirst; //index of the first element added into buffer
  uint16_t IndexLast; //index of the last element into buffer
  uint16_t FreeElements;
  uint8_t Status;
  
  WordsToBytes WordsStorage;
  uint8_t SingleBytesStorage[PACKET_SIZE];
  uint8_t SingleBytesCounter;
}Buffer;

void PushSymbol(Buffer *buffer, uint8_t symbol);

void PushSingleByte(Buffer *buffer, uint8_t byte);

void PushInteger16(Buffer *buffer, uint16_t word);

void PushInteger32(Buffer *buffer, uint32_t number);

void PushString(Buffer *buffer, const uint8_t string[]);

/*void PushNumber8AsString(Buffer *buffer, uint8_t number);*/

uint8_t Pop(Buffer *buffer);

void IncrementIndex(uint16_t *index);

void InitBuffer(Buffer *buffer);

extern Buffer USART_Buffer;

#endif