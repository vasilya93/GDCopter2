#include "stm32l1xx.h"

#ifndef USART_H
#define USART_H

//apb1 for usart1
//apb2 usart2 - usart5, i2c1, i2c2, tim2 - tim7 (usart3 for me)
//way apb2 prescaler <- ahb prescaler
//now i'll make easier - with default frequency; when i add functions to change frequency -
//i'll add write functions to to change baudrate for USART; 


void USART_Begin(uint16_t speed);

void USART_BeginConfigured(uint16_t speed, uint8_t conf);

void USART_End();

void USART_WriteByte(uint8_t writtenByte);

void USART_ByteReceived_Attach(void (*handler)(uint8_t receicedByte));

void USART_TransRegEmpty_Attach(void (*handler)());

void USART_TransComplete_Attach(void (*handler)());

typedef struct USART
{
  void (**ByteReceivedHandlers)(uint8_t receivedByte);
  uint8_t BRH_Amount;
  
  void (**TransRegEmptyHandlers)();
  uint8_t TREH_Amount;
  
  void (**TransCompleteHandlers)();
  uint8_t TCH_Amount;
}USART;

#endif