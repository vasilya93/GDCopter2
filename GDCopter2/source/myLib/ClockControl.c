#include "ClockControl.h"
#include "stm32l1xx.h"

ClockControlType ClockControl;

void ClockControl_Initialize()
{
  //enable HSE
  RCC->CR |= RCC_CR_HSEON;
  while(~RCC->CR & RCC_CR_HSERDY);
  
  //enable MSI
  RCC->CR |= RCC_CR_MSION;
  while(~RCC->CR & RCC_CR_MSIRDY);
  
  //run MSY as SYSCLK
  RCC->CFGR &= ~(RCC_CFGR_SW);
  while(RCC->CFGR & RCC_CFGR_SWS);
  
  //disable PLL to configure it
  RCC->CR &= ~RCC_CR_PLLON;
  while(RCC->CR & RCC_CR_PLLRDY);
  
  //configure PLL
  RCC->CFGR &= ~RCC_CFGR_PLLSRC;
  RCC->CFGR |= RCC_CFGR_PLLSRC_HSE;
  RCC->CFGR &= ~RCC_CFGR_PLLMUL;
  RCC->CFGR |= RCC_CFGR_PLLMUL8;
  RCC->CFGR |= RCC_CFGR_PLLDIV; //can't be two zeros, so unusual way
  RCC->CFGR &= (RCC_CFGR_PLLDIV2 | ~RCC_CFGR_PLLDIV);  
  
  //enable PLL
  RCC->CR |= RCC_CR_PLLON;
  while(~RCC->CR & RCC_CR_PLLRDY);
  
  //run PLL as SYSCLK
  RCC->CFGR &= ~(RCC_CFGR_SW);
  RCC->CFGR |= RCC_CFGR_SW_PLL;
  while(~RCC->CFGR & RCC_CFGR_SWS_PLL);
  
  
  RCC->CFGR &= ~RCC_CFGR_HPRE;
  RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
  
  RCC->CFGR &= ~RCC_CFGR_PPRE1;
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
  
  RCC->CFGR &= ~RCC_CFGR_PPRE2;
  RCC->CFGR |= RCC_CFGR_PPRE2_DIV4;
  
  ClockControl.APB1_Frequency = 8000000; // if changes, clock tuning in I2C should be changed 
  ClockControl.APB2_Frequency = 8000000;
  ClockControl.AHB_Frequency = 32000000;
  ClockControl.SYSCLK_Frequency = 32000000;
}