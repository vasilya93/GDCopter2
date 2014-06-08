#include "I2C.h"
#include "stm32l1xx.h"
#include "stdlib.h"
#include "Messenger.h"

I2C_Operation_Type I2C_Operation;

void I2C_Begin()
{
  I2C_Operation.State = 0;
  I2C_Operation.Bytes = calloc(I2C_DATA_SIZE, sizeof(uint8_t));
  I2C_Operation.Description.DataDescript = I2C_DD_NODATA;
  
  NVIC_EnableIRQ(I2C1_EV_IRQn);
  NVIC_EnableIRQ(I2C1_ER_IRQn);
  
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
  
  GPIOB->MODER |=  /*I2C1*/ GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1;
  GPIOB->OTYPER |= /*I2C1*/ GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7; //open drain
  GPIOB->OSPEEDR |= /*I2C1*/ GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7;
  GPIOB->PUPDR |= /*I2C1*/ GPIO_PUPDR_PUPDR6_0 | GPIO_PUPDR_PUPDR7_0; //pull up
  GPIOB->AFR[0] |= (0x4 << 24) | (0x4 << 28);
  
  RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
  
  I2C1->CR2 |= I2C_CR2_ITBUFEN | I2C_CR2_ITEVTEN | I2C_CR2_ITERREN | I2C_CR2_FREQ_3 /*changes if PCLK1 changes*/;
  I2C1->CCR |= 40; /*changes if PCLK1 changes*/
  I2C1->TRISE = 9; /*changes if PCLK1 changes*/
  I2C1->CR1 |= I2C_CR1_PE | I2C_CR1_ACK;
}

bool I2C_OperationComplete_Attach(void (*handler)(void* operation))
{
  if(handler == NULL)
  {
    return false;
  }
  
  I2C_Operation.OCH_Amount++;
  void (**newPointer)(void* operation) = (void (**)(void*))realloc(I2C_Operation.OperationCompleteHandlers, I2C_Operation.OCH_Amount*sizeof(void(*)(void*)));
  
  if(newPointer == NULL)
  {
    I2C_Operation.OCH_Amount--;
    return false;
  }
  
  I2C_Operation.OperationCompleteHandlers = newPointer;
  I2C_Operation.OperationCompleteHandlers[I2C_Operation.OCH_Amount - 1] = handler;
  
  return true;
}

bool I2C_StartOperation(I2C_OpDescript_Type description)
{
  if((description.BytesNum > I2C_DATA_SIZE) || (I2C_Operation.State & I2C_ST_BUSY))
  {
    return false;
  }
  I2C_Operation.State |= I2C_ST_BUSY;
  I2C_Operation.Description = description;
  
  while(I2C1->SR2 & I2C_SR2_BUSY);
  
  I2C1->CR1 |= I2C_CR1_START;
  Messenger_SendByte(I2C_MSG_STRTST); 
  
  return true;
}

void I2C1_EV_IRQHandler(void)
{
  if(I2C1->SR1 & I2C_SR1_SB)
  {
    //I2C_SendAddress();
  }
  if(I2C1->SR1 & I2C_SR1_ADDR)
  {
    if(I2C1->SR2 & I2C_SR2_TRA)
    {
      //I2C_OperateTransmitter();
    }
    else
    {
      //I2C_OperateReceiver();
    }
  }
  if(I2C1->SR1 & I2C_SR1_TXE)
  {
    //I2C_TransmitRegisterEmpty();
  }
  if(I2C1->SR1 & I2C_SR1_RXNE)
  {
    //I2C_ReceiveRegisterNotEmpty();
  }
}

void I2C1_ER_IRQHandler(void)
{
  if(I2C1->SR1 & I2C_SR1_AF)
  {
    //I2C1->SR1 &= ~I2C_SR1_AF;
    //I2C1->CR1 |= I2C_CR1_STOP;//нет акноледжмента, во первых I2C_CurrentOperation пропадает
    //PushSingleByte(&USART_Buffer, MESSAGE_I2C_NO_ACK);//во вторых все ост. операции ждут след. секунды
    //I2C_SetStart(false);//вызов старта с параметром, нужно переписывать CurrentOperat или нет
  }
}