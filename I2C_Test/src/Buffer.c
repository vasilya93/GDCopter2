#include "Buffer.h"
#include "math.h"

Buffer USART_Buffer;

void PushSymbol(Buffer *buffer, uint8_t symbol)
{
  buffer->Array[buffer->IndexLast] = symbol;
  buffer->Status &= ~BUFFER_STATUS_EMPTY;
  IncrementIndex(&buffer->IndexLast);
  if(buffer->Status & BUFFER_STATUS_FULL)
  {
    IncrementIndex(&buffer->IndexFirst);
  }
  else
  {
    buffer->FreeElements--;
  }
  if(buffer->IndexFirst == buffer->IndexLast)
  {
    buffer->Status &= BUFFER_STATUS_FULL;
  }
}

void PushSingleByte(Buffer *buffer, uint8_t byte)
{
  buffer->SingleBytesStorage[buffer->SingleBytesCounter] = byte;
  if(++buffer->SingleBytesCounter == PACKET_SIZE)
  {
    while(buffer->FreeElements < (PACKET_SIZE + 1));
    PushSymbol(buffer, 0x01);
    for(int i = 0; i < PACKET_SIZE; i++)
    {
      PushSymbol(buffer, buffer->SingleBytesStorage[i]);
    }
    buffer->SingleBytesCounter = 0;
  }
}

void PushString(Buffer *buffer, const uint8_t string[])
{
  uint16_t i = 0;
  while(string[i] != '\0')
  {
    PushSymbol(buffer, string[i]);
    i++;
  }
  PushSymbol(buffer, 0x0D);
  PushSymbol(buffer, 0x0A);
}


void PushInteger32(Buffer *buffer, uint32_t number)
{
  _32ToBytes _32toBytes;
  _32toBytes.Integer = number;
  while(buffer->FreeElements < (PACKET_SIZE+1));
  PushSymbol(buffer, 0x03);
  for(int i = 0; i < PACKET_SIZE; i++)
  {
    PushSymbol(buffer, _32toBytes.Bytes[i]);
  }
}

void PushInteger16(Buffer *buffer, uint16_t word)
{
  WordsToBytes wordsToBytes;
  wordsToBytes.Words[wordsToBytes.WordsCounter] = word;
  switch(wordsToBytes.WordsCounter)
  {
  case 1:
    while(buffer->FreeElements < (PACKET_SIZE+1));
    PushSymbol(buffer, 0x02);
    for(int i = 0; i < PACKET_SIZE; i++)
    {
      PushSymbol(buffer, wordsToBytes.Bytes[i]);
    }
    wordsToBytes.WordsCounter = 0;
    break;
  case 0:
    wordsToBytes.WordsCounter = 1;
    break;
  }
}

/*void PushNumber8AsString(Buffer *buffer, uint8_t number)
{
  //создает массив чаров (от двух до четырех элементов) и передает его функции push string
  int n = log10(number) + 1;
  char* array = calloc(n, sizeof(char));
  
}*/

uint8_t Pop(Buffer *buffer)
{
  buffer->Status &= ~BUFFER_STATUS_FULL;
  if(buffer->Status & BUFFER_STATUS_EMPTY)
  {
    return '\0';
  }
  else
  {
    buffer->FreeElements++;
    uint8_t symbol = buffer->Array[buffer->IndexFirst];
    IncrementIndex(&buffer->IndexFirst);
    if(buffer->IndexFirst == buffer->IndexLast)
    {
      buffer->Status |= BUFFER_STATUS_EMPTY;
    }
    return symbol;
  }
}

void IncrementIndex(uint16_t *index)
{
  *index += 1;
  if(*index == BUFFER_SIZE)
  {
    *index = 0;
  }
}

void InitBuffer(Buffer *buffer)
{
  buffer->IndexFirst = 0;
  buffer->IndexLast = 0;
  buffer->FreeElements = BUFFER_SIZE;
  buffer->SingleBytesCounter = 0;
  buffer->WordsStorage.WordsCounter = 0;
  buffer->Status = BUFFER_STATUS_EMPTY;
}