#include "Buffer.h"
#include "math.h"

void IncrementIndex(uint16_t *index);

void Buffer_Push(Buffer_Type *buffer, uint8_t symbol)
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

uint8_t Buffer_Pop(Buffer_Type *buffer)
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


void Buffer_Init(Buffer_Type *buffer)
{
  buffer->IndexFirst = 0;
  buffer->IndexLast = 0;
  buffer->FreeElements = BUFFER_SIZE;
  buffer->Status = BUFFER_STATUS_EMPTY;
}

//-------------------------Hidden funtions--------------------------------------

void IncrementIndex(uint16_t *index)
{
  *index += 1;
  if(*index == BUFFER_SIZE)
  {
    *index = 0;
  }
}