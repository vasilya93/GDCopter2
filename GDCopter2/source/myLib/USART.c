#include "USART.h"
#include "stm32l1xx.h"
#include "math.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdlib.h"
#include "ClockControl.h"

USART_Type USART;

void USART_SetupBasic(unsigned int speed);

void USART_Begin(unsigned int speed)
{
  USART_SetupBasic(speed);
  USART3->CR1 |= USART_CR1_RE | USART_CR1_TE;
}

void USART_BeginConfigured(uint16_t speed, uint8_t conf)
{
  USART_SetupBasic(speed);
  
  //configuring data bits, parity control and stop bits
  if(conf & USART_9BYTE)
  {
    USART3->CR1 |= USART_CR1_M;
  }
  else
  {
    USART3->CR1 &= ~USART_CR1_M;
  }
  
  if(conf & USART_PCENABLE)
  {
    USART3->CR1 |= USART_CR1_PCE;
    if(conf & USART_PARITY_ODD)
    {
      USART3->CR1 |= USART_CR1_PS;
    }
    else
    {
      USART3->CR1 &= ~USART_CR1_PS;
    }
  }
  else
  {
    USART3->CR1 &= ~USART_CR1_PCE;
  }
  
  switch(conf & USART_SB)
  {
  case USART_SB1:
    USART3->CR2 &= ~USART_CR2_STOP;
    break;
  case USART_SB05:
    USART3->CR2 |= USART_CR2_STOP_0;
    break;
  case USART_SB2:
    USART3->CR2 |= USART_CR2_STOP_1;
    break;
  case USART_SB15:
    USART3->CR2 |= USART_CR2_STOP;
    break;
  }
  
  USART3->CR1 |= USART_CR1_RE | USART_CR1_TE;
}

void USART_End()
{
  RCC->APB1RSTR |= RCC_APB1RSTR_USART3RST;
  RCC->APB1RSTR &= ~RCC_APB1RSTR_USART3RST;
  RCC->APB1ENR &= ~RCC_APB1ENR_USART3EN;
}

void USART_WriteByte(uint8_t writtenByte)
{
  USART3->DR = writtenByte;
}

bool USART_ByteReceived_Attach(void (*handler)(uint8_t receicedByte))
{
  if(handler == NULL)
  {
    return false;
  }
  
  USART.BRH_Amount++;
  void (**newPointer)(uint8_t receivedByte) = (void (**)(uint8_t))realloc(USART.ByteReceivedHandlers, USART.BRH_Amount*sizeof(void(*)(uint8_t)));
  
  if(newPointer == NULL)
  {
    USART.BRH_Amount--;
    return false;
  }
  
  USART.ByteReceivedHandlers = newPointer;
  USART.ByteReceivedHandlers[USART.BRH_Amount - 1] = handler;
  
  return true;
}

void USART_ByteReceived_Detach()
{
  free(USART.ByteReceivedHandlers);
  USART.ByteReceivedHandlers = NULL;
  USART.BRH_Amount = 0;
}

bool USART_TransRegEmpty_Attach(void (*handler)())
{
  if(handler == NULL)
  {
    return false;
  }
  
  USART.TREH_Amount++;
  void (**newPointer)() = (void (**)())realloc(USART.TransRegEmptyHandlers, USART.TREH_Amount*sizeof(void(*)()));
  
  if(newPointer == NULL)
  {
    USART.TREH_Amount--;
    return false;
  }

  USART.TransRegEmptyHandlers = newPointer;
  USART.TransRegEmptyHandlers[USART.TREH_Amount - 1] = handler;
  
  return true;
}

void USART_TransRegEmpty_Detach()
{
  free(USART.TransRegEmptyHandlers);
  USART.TransRegEmptyHandlers = NULL;
  USART.TREH_Amount = 0;
}

bool USART_TransComplete_Attach(void (*handler)())
{
  if(handler == NULL)
  {
    return false;
  }
  
  USART.TCH_Amount++;
  void (**newPointer)() = (void (**)())realloc(USART.TransCompleteHandlers, USART.TCH_Amount*sizeof(void(*)()));
  
  if(newPointer == NULL)
  {
    USART.TCH_Amount--;
    return false;
  }
  
  USART.TransCompleteHandlers = newPointer;
  USART.TransCompleteHandlers[USART.TCH_Amount - 1] = handler;
  
  return true;
}

void USART_TransComplete_Detach()
{
  free(USART.TransCompleteHandlers);
  USART.TransCompleteHandlers = NULL;
  USART.TCH_Amount = 0;
}

void USART_InitStructure()
{
  free(USART.ByteReceivedHandlers);
  USART.ByteReceivedHandlers = NULL;
  USART.BRH_Amount = 0;
  
  free(USART.TransRegEmptyHandlers);
  USART.TransRegEmptyHandlers = NULL;
  USART.TREH_Amount = 0;
  
  free(USART.TransCompleteHandlers);
  USART.TransCompleteHandlers = NULL;
  USART.TCH_Amount = 0;
  
  USART.IsStructureInitialized = true;
}

void USART_EnableInterrupts(uint8_t interrupts)
{
  if(interrupts & USART_IT_TXE)
  {
    USART3->CR1 |= USART_CR1_TXEIE;
  }  
  if(interrupts & USART_IT_TC)
  {
    USART3->CR1 |= USART_CR1_TCIE;
  }  
  if(interrupts & USART_IT_RXNE)
  {
    USART3->CR1 |= USART_CR1_RXNEIE;
  }  
  if(interrupts & USART_IT_PE)
  {
    USART3->CR1 |= USART_CR1_PEIE;
  }
}

void USART_DisableInterrupts(uint8_t interrupts)
{
  if(interrupts & USART_IT_TXE)
  {
    USART3->CR1 &= ~USART_CR1_TXEIE;
  }  
  if(interrupts & USART_IT_TC)
  {
    USART3->CR1 &= ~USART_CR1_TCIE;
  }  
  if(interrupts & USART_IT_RXNE)
  {
    USART3->CR1 &= ~USART_CR1_RXNEIE;
  }  
  if(interrupts & USART_IT_PE)
  {
    USART3->CR1 &= ~USART_CR1_PEIE;
  }
}

//-----------------------------Handlers-----------------------------------------

void USART3_IRQHandler(void)
{
  if((USART3->SR & USART_SR_TXE) && (USART3->CR1 & USART_CR1_TXEIE))
  {
    //cleared by a write to DR
    for(int i = 0; i < USART.TREH_Amount; i++)
    {
      (*USART.TransRegEmptyHandlers[i])();
    }
  }
  if((USART3->SR & USART_SR_TC) && (USART3->CR1 & USART_CR1_TCIE))
  {
    //cleared by a read from SR followed by a write to DR
    //can be cleared by writing '0' to it (recommended for multibuffer communication)
    USART3->SR &= ~USART_SR_TC;
    for(int i = 0; i < USART.TCH_Amount; i++)
    {
      (*USART.TransCompleteHandlers[i])();
    }
  }
  if((USART3->SR & USART_SR_RXNE) && (USART3->CR1 & USART_CR1_RXNEIE))
  {
    //cleared by a read to DR
    //can be cleared by writing '0' to it (recommended for multibuffer communication)
    uint8_t receivedByte = USART3->DR;
    for(int i = 0; i < USART.BRH_Amount; i++)
    {
      (*USART.ByteReceivedHandlers[i])(receivedByte);
    }
  }
  
  //PE cleared by a sequence of SR read and read/write access to DR
  //software must wait for RXNE before clearing PE
}

//-------------------------Hidden funtions--------------------------------------

void USART_SetupBasic(unsigned int speed)
{
  NVIC_EnableIRQ(USART3_IRQn);
  
  //enabling GPIOC clocking, USART clocking
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 
  
  //configuring output pins
  GPIOC->MODER &= ~(GPIO_MODER_MODER10 | GPIO_MODER_MODER11);
  GPIOC->MODER |= GPIO_MODER_MODER10_1 | GPIO_MODER_MODER11_1;
  GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR10 | GPIO_OSPEEDER_OSPEEDR11;
  GPIOC->AFR[1] &= ~((0xF << 8) | (0xF << 12));
  GPIOC->AFR[1] |= (0x7 << 8) | (0x7 << 12);
  
  //enabling GPIOC clocking, USART clocking
  RCC->APB1ENR |= RCC_APB1ENR_USART3EN; 
  
  //configuring USART
  USART3->SR &= ~USART_SR_TC;
  USART3->CR1 |= USART_CR1_UE | USART_CR1_RXNEIE | USART_CR1_TCIE;
  
  //calculating Baudrate Register value
  float USARTDIV = (float)ClockControl.APB1_Frequency / (float)(16 * speed);
  unsigned int DivMantissa = (int)USARTDIV;
  while(DivMantissa < 1); //caution in case fck is too low or baudrate is too big
  
  float Fraction = (USARTDIV - DivMantissa);
  unsigned int DivFraction;
  if(Fraction >= (0.0625 * 15.5))
  {
    DivFraction = 0;
    DivMantissa++;
  }
  else
  {
    DivFraction = (int)floor((Fraction / 0.0625) + 0.5);
  }  
  
  USART3->BRR = DivFraction | (DivMantissa << 4);
}