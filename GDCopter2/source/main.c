#include "stm32l1xx.h"
#include "ClockControl.h"
#include "Serial.h"

//pins uart2-rx pd6, uart2-tx pd5, rs232-en pd0, leds - pe10 pe11
//pins left button - pc13, right button - pa0;
//peripherals: tim6, uart2, gpiod, gpioe;

uint8_t ledState;
uint8_t symbol;

void Initialize();
void ToggleLed();

int main()
{
  Initialize();
  while(1);
}

void Initialize()
{
  ledState = 1;
  
  __enable_irq();
  NVIC_EnableIRQ(TIM7_IRQn);
  
  ClockControl_Initialize();
  
  Serial_Begin(9600);
  
  RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
  RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
  
  GPIOE->MODER |= GPIO_MODER_MODER10_0;
  GPIOE->ODR |= GPIO_ODR_ODR_10;
  
  TIM7->PSC = 2097;
  TIM7->ARR = 500;
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
   Serial_WriteLine("Hello world!\n\r");
   Serial_WriteInt32AsString(542);
   Serial_WriteLine("\n\r");
   ToggleLed();
}
