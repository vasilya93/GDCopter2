#include "USART.h"

void USART_Begin(uint16_t speed)
{
  //enableing GPIOC clocking, USART clocking
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 
  
  //configuring output pins
  GPIOC->MODER |= GPIO_MODER_MODER10_1 | GPIO_MODER_MODER11_1;
  GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR10 | GPIO_OSPEEDER_OSPEEDR11;
  GPIOC->AFR[1] |= (0x7 << 8) | (0x7 << 12);
  
  RCC->APB1ENR |= RCC_APB1ENR_USART3EN; 
  
  //configuring USART
  USART3->SR &= ~USART_SR_TC;
  USART3->CR1 |= USART_CR1_UE | USART_CR1_RXNEIE | USART_CR1_TCIE; 
  USART3->BRR |= 0x8F;
}

void USART_BeginConfigured(uint16_t speed, uint8_t conf)
{
  int a = 1;
}

void USART_End()
{
  int a = 1;
}

void USART_WriteByte(uint8_t writtenByte)
{
  int a = 1;
}

void USART_ByteReceived_Attach(void (*handler)(uint8_t receicedByte))
{
  int a = 1;
}

void USART_TransRegEmpty_Attach(void (*handler)())
{
  int a = 1;
}

void USART_TransComplete_Attach(void (*handler)())
{
  int a = 1;
}