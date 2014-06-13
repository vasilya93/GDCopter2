#include "stm32l1xx.h"
#include "ClockControl.h"
#include "Serial.h"
#include "I2C.h"
#include "Messenger.h"
#include "MS5611.h"
#include "Wire.h"
#include "stdlib.h"

//pins uart2-rx pd6, uart2-tx pd5, rs232-en pd0, leds - pe10 pe11
//pins left button - pc13 - user, right button - pa0 - wkup;
//peripherals: tim6, uart2, gpiod, gpioe;

uint8_t ledState;
uint8_t symbol;
uint32_t counter;
char RecArray[50];

void Initialize();
void ToggleLed();
void HandlerCounter(uint8_t recByte);
void I2C_Handler(void* operation);

int main()
{
  //почему пропускает? e
  //пропускает второй символ в строке
  Initialize();
  while(1);
}

void Initialize()
{
  ledState = 1;
  counter = 0;
  
  __enable_irq();
  NVIC_EnableIRQ(TIM7_IRQn);
  //NVIC_EnableIRQ(EXTI15_10_IRQn);
  NVIC_EnableIRQ(EXTI0_IRQn);
  
  ClockControl_Initialize();
  
  MS5611_Initialize(&MS5611);
  Wire_Initialize();
  Wire_AttachDevice(&MS5611);
  //I2C_Begin();
  //I2C_OperationComplete_Attach(&I2C_Handler);
  
  Serial_Begin(115200);
  Serial_ByteReceived_Attach(&HandlerCounter);
  
  RCC->AHBENR |= /*right button*/ RCC_AHBENR_GPIOAEN | /*left button RCC_AHBENR_GPIOCEN |*/ RCC_AHBENR_GPIOEEN;
  RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
  
  GPIOA->PUPDR |= /*right button*/ GPIO_PUPDR_PUPDR0_1;
  //GPIOC->PUPDR |= /*left button*/ GPIO_PUPDR_PUPDR13_0;
  
  /*SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;*/
  EXTI->IMR |= EXTI_IMR_MR0 /*| EXTI_IMR_MR13*/;
  EXTI->EMR |= EXTI_EMR_MR0 /*| EXTI_EMR_MR13*/;
  EXTI->RTSR |= EXTI_FTSR_TR0;
  //EXTI->FTSR |= EXTI_FTSR_TR13;
  
  GPIOE->MODER |= GPIO_MODER_MODER10_0;
  GPIOE->ODR |= GPIO_ODR_ODR_10;
  
  TIM7->PSC = 8000;
  TIM7->ARR = 1000;
  TIM7->DIER |= TIM_DIER_UIE;
  TIM7->CR1 |= TIM_CR1_CEN;
}

void ToggleLed()
{
  if(ledState)
  {
    GPIOE->ODR &= ~GPIO_ODR_ODR_10;
    ledState  = 0;
  }
  else
  {
    GPIOE->ODR |= GPIO_ODR_ODR_10;
    ledState = 1;
  }
}

void TIM7_IRQHandler(void)
{
   TIM7->SR &= ~TIM_SR_UIF;
   ToggleLed();
}

void EXTI0_IRQHandler(void)
{  
  EXTI->PR |= EXTI_PR_PR0;
  Wire_Begin();
//  I2C_OpDescript_Type opDescript;
//  opDescript.BytesNum = 1;
//  opDescript.DataDescript = 1;
//  opDescript.DeviceAddress = 0x77;
//  opDescript.IsRead = false;
//  uint8_t* bytes = malloc(1);
//  bytes[0] = MS5611_READ_SENS_T1;
//  I2C_StartOperation(opDescript, bytes);
}

/*void EXTI15_10_IRQHandler(void)
{
  EXTI->PR |= EXTI_PR_PR13;
  ToggleLed();
}*/
  
void HandlerCounter(uint8_t recByte)
{
  RecArray[counter] = recByte;
  if(counter < 48)
  {
    counter++;
  }
  ToggleLed();
}

void I2C_Handler(void* operation)
{
  switch(((I2C_Operation_Type*)operation)->Description.DataDescript)
  {
  case 1:
    {
      I2C_OpDescript_Type opDescript;
      opDescript.BytesNum = 2;
      opDescript.DataDescript = 2;
      opDescript.DeviceAddress = 0x77;
      opDescript.IsRead = true;
      I2C_StartOperation(opDescript, NULL);
    }
    break;
  case 2:
    {
      uint16_t data = (((I2C_Operation_Type*)operation)->Bytes[0]) << 8 | ((I2C_Operation_Type*)operation)->Bytes[1];
      Messenger_SendDWord(data);
    }
    break;
  }
}
