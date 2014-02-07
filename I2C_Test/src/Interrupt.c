#include "Interrupt.h"
#include "stdlib.h"
#include "stdbool.h"
#include "Buffer.h"
#include "I2C.h"
#include "I2C_Operation.h"//пушить команды из таймера
#include "I2C_Buffer.h" //нужно вынести в итоге за и2ц
#include "MS5611.h"

uint16_t PeripheralStatus;

void EXTI15_10_IRQHandler(void)
{
  EXTI->PR |= EXTI_PR_PR13;
  if(~I2C1->SR2 & I2C_SR2_BUSY)
  {
    I2C1->CR1 |= I2C_CR1_START;
    PushSingleByte(&USART_Buffer, MESSAGE_I2C_START_SET);
  }
  else
  {
    PushSingleByte(&USART_Buffer, MESSAGE_I2C_BUSY);
  }
}

void EXTI0_IRQHandler(void)
{
  EXTI->PR |= EXTI_PR_PR0;
  if(~I2C1->SR2 & I2C_SR2_BUSY)
  {
    I2C1->CR1 |= I2C_CR1_START;
    PushSingleByte(&USART_Buffer, MESSAGE_I2C_START_SET);
    PeripheralStatus |= RIGHT_BUTTON_PRESSED;
    if(PeripheralStatus & RIGHT_BUTTON_FOR_WRITE)
    {
      PeripheralStatus &= ~RIGHT_BUTTON_FOR_WRITE;
    }
    else
    {      
      PeripheralStatus |= RIGHT_BUTTON_FOR_WRITE;
    }
  }
  else
  {
    PushSingleByte(&USART_Buffer, MESSAGE_I2C_BUSY);
  }
}

void TIM6_IRQHandler(void)
{  
  TIM6->SR &= ~TIM_SR_UIF;
  if(I2C1_Buffer.FreeElementsAmount > 2)
  {
    I2C_Operation newOperation;
    newOperation.DeviceAddress = MS5611_WRITE_ADDRESS;
    newOperation.BytesAmount = 1;
    newOperation.BytesCounter = 0;
    newOperation.DataBytes = calloc(newOperation.BytesAmount,sizeof(uint8_t));
    newOperation.DataBytes[0] = MS5611_ADC_READ;
    I2C_Push(&I2C1_Buffer, newOperation); //можно б было и тут проверку делать, полон ли буффер
    newOperation.DeviceAddress = MS5611_READ_ADDRESS;
    newOperation.BytesAmount = 3;
    newOperation.BytesCounter = 0;
    newOperation.DataBytes = calloc(newOperation.BytesAmount,sizeof(uint8_t));
    I2C_Push(&I2C1_Buffer, newOperation);
    newOperation.DeviceAddress = MS5611_WRITE_ADDRESS;
    newOperation.BytesAmount = 1;
    newOperation.BytesCounter = 0;
    newOperation.DataBytes = calloc(newOperation.BytesAmount,sizeof(uint8_t));
    newOperation.DataBytes[0] = MS5611_CONV_TEMP_512;
    I2C_Push(&I2C1_Buffer, newOperation);
  }
    I2C_SetStart(true);
}

//-----------------------------------------------------------------------------

void TIM7_IRQHandler(void)
{
   TIM7->SR &= ~TIM_SR_UIF;
   if((~PeripheralStatus & USART3_BUSY) && (~USART_Buffer.Status & BUFFER_STATUS_EMPTY))
   {
     PeripheralStatus |= USART3_BUSY;
     USART3->DR = Pop(&USART_Buffer);
     USART3->CR1 |= USART_CR1_TXEIE;
   }
}

//-----------------------------------------------------------------------------

void I2C1_EV_IRQHandler(void)
{
  if(I2C1->SR1 & I2C_SR1_SB)
  {
    I2C_SendAddress();
  }
  if(I2C1->SR1 & I2C_SR1_ADDR)
  {
    if(I2C1->SR2 & I2C_SR2_TRA)
    {
      I2C_OperateTransmitter();
    }
    else
    {
      I2C_OperateReceiver();
    }
  }
  if(I2C1->SR1 & I2C_SR1_TXE)
  {
    I2C_TransmitRegisterEmpty();
  }
  if(I2C1->SR1 & I2C_SR1_RXNE)
  {
    I2C_ReceiveRegisterNotEmpty();
  }
}

//-----------------------------------------------------------------------------

void I2C1_ER_IRQHandler(void)
{
  if(I2C1->SR1 & I2C_SR1_AF)
  {
    I2C1->SR1 &= ~I2C_SR1_AF;
    I2C1->CR1 |= I2C_CR1_STOP;//нет акноледжмента, во первых I2C_CurrentOperation пропадает
    PushSingleByte(&USART_Buffer, MESSAGE_I2C_NO_ACK);//во вторых все ост. операции ждут след. секунды
    I2C_SetStart(false);//вызов старта с параметром, нужно переписывать CurrentOperat или нет
  }
}

//-----------------------------------------------------------------------------

void USART3_IRQHandler(void)
{
  if(USART3->SR & USART_SR_TXE)
  {
    if(~USART_Buffer.Status & BUFFER_STATUS_EMPTY)
    {
      USART3->DR = Pop(&USART_Buffer);
    }
    else
    {
      USART3->CR1 &= ~USART_CR1_TXEIE;
    }
  }
  if(USART3->SR & USART_SR_TC)
  {
    if(~USART_Buffer.Status & BUFFER_STATUS_EMPTY)
    {
      USART3->CR1 |= USART_CR1_TXEIE;
      USART3->DR = Pop(&USART_Buffer);
    }
    else
    {
      USART3->SR &= ~USART_SR_TC;
      PeripheralStatus &= ~USART3_BUSY;
    }
  }
  if(USART3->SR & USART_SR_RXNE)
  {
    USART3->SR &= ~USART_SR_RXNE;
    uint8_t symbol = USART3->DR;
    if(symbol >= 0x41 && symbol < 0x5A)
    {
      symbol += 0x20;
    }
    else if(symbol >= 0x61 && symbol < 0x7A)
    {
      symbol -= 0x20;
    }
    USART3->DR = symbol;
    
    if (1)
    {
      //LedStatus &= ~(8);
      GPIOE->ODR &= ~(1<<11);
    }
    else
    {
      //LedStatus |= 8;
      GPIOE->ODR |= 1<<11;
    }
  }
}

//-----------------------------------------------------------------------------

void ADC1_IRQHandler(void)
{
  ADC1->SR |= ADC_SR_EOC;
  //uint16_t value = ADC1->DR;
  //FleshLeds(value);
}

//-----------------------------------------------------------------------------

void FleshLeds()
{
  if(PeripheralStatus&(STAT1_LED_ON))
  {
    GPIOA->ODR |= 1 << 4;
  }
  else
  {
    GPIOA->ODR &= ~(1 << 4);
  }
  if(PeripheralStatus&(STAT2_LED_ON))
  {
    GPIOA->ODR |= 1 << 5;
  }
  else
  {
    GPIOA->ODR &= ~(1 << 5);
  }
  if(PeripheralStatus&(STAT1_LED_ON))
  {
    GPIOE->ODR &= ~(1 << 10);
  }
  else
  {
    GPIOE->ODR |= 1 << 10;
  }
  if(PeripheralStatus&(STAT1_LED_ON))
  {
    GPIOE->ODR &= ~(1 << 11);
  }
  else
  {
    GPIOE->ODR |= 1 << 11;
  }
}

//-----------------------------------------------------------------------------