#include "ClockControl.h"
#include "stm32l1xx.h"

ClockControlType ClockControl;

void ClockControl_Initialize()
{
  RCC->CR |= RCC_CR_MSION;
  while(~RCC->CR & RCC_CR_MSIRDY);
  RCC->CFGR &= ~(RCC_CFGR_SW);
  RCC->CFGR |= RCC_CFGR_SW_MSI;
  //while(~RCC->CFGR & RCC_CFGR_SWS_MSI); - looks reasonable but won't survive
  while(RCC->CFGR & RCC_CFGR_SWS); //will be locked in the cycle intil both digits are zeroes
  
  RCC->CFGR &= ~RCC_CFGR_HPRE;
  RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
  
  RCC->CFGR &= ~RCC_CFGR_PPRE1;
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV1;
  
  RCC->CFGR &= ~RCC_CFGR_PPRE2;
  RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;
  
  ClockControl.APB1_Frequency = 2097000;
  ClockControl.APB2_Frequency = 2097000;
  ClockControl.AHB_Frequency = 2097000;
  ClockControl.SYSCLK_Frequency = 2097000;
}