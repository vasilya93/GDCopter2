#include "stm32l1xx.h"
#include "Interrupt.h"
#include "Buffer.h"
#include "I2C.h"
#include "I2C_Buffer.h"
#include "I2C_Operation.h"

//stat1 - pa4
//stat2 - pa5
//stat3 - pe10 - inverse
//stat4 - pe11 - inverse
//user button - pc13
//wkup button - pa0
//пока максимум - три байта

void Initialize();

void main()
{
  InitBuffer(&USART_Buffer);
  I2C_InitBuffer(&I2C1_Buffer);
  I2C_InitOperation(&I2C1_CurrentOperation);
  Initialize();
  
  while(1) {} //попробовать слать температуру каждый раз, чтоб убедитьс€, что она действительно не мен€етс€
  //убедитьс€ в том, что операции присваивани€ действительно выполн€ютс€
}

//pins uart2-rx pd6, uart2-tx pd5, rs232-en pd0, leds - pe10 pe11
//pins left button - pc13, right button - pa0;
//peripherals: tim6, uart2, gpiod, gpioe;
//-----------------------------------------------------------------------------

void Initialize()
{
  PeripheralStatus = 0;
  
  __enable_irq();
  NVIC_EnableIRQ(EXTI15_10_IRQn);
  NVIC_SetPriority(EXTI15_10_IRQn, 1);
  NVIC_EnableIRQ(EXTI0_IRQn);
  NVIC_SetPriority(EXTI0_IRQn, 1);
  NVIC_EnableIRQ(USART3_IRQn);
  //NVIC_SetPriority(USART3_IRQn, 13); //то, что тер€етс€, может и норм - быстро же сую
  NVIC_EnableIRQ(I2C1_EV_IRQn);
  NVIC_SetPriority(I2C1_EV_IRQn, 1);
  NVIC_EnableIRQ(I2C1_ER_IRQn);
  NVIC_SetPriority(I2C1_ER_IRQn, 1);
  NVIC_EnableIRQ(TIM6_IRQn);
  //NVIC_SetPriority(TIM6_IRQn, 13);
  NVIC_EnableIRQ(TIM7_IRQn);
  //NVIC_SetPriority(TIM7_IRQn, 13);
  
  RCC->CFGR &= ~(RCC_CFGR_SW);
  RCC->CFGR |= RCC_CFGR_SW_MSI;
  while (~RCC->CFGR & RCC_CFGR_SWS_MSI) {}  
  RCC->CR &= ~RCC_CR_PLLON;
  while(RCC->CR & RCC_CR_PLLRDY) {}
  RCC->CFGR |= RCC_CFGR_PLLMUL12 | RCC_CFGR_PLLSRC_HSE | RCC_CFGR_HPRE_DIV4;
  RCC->CR |= RCC_CR_PLLON;
  while(~RCC->CR & RCC_CR_PLLRDY) {}  
  
  RCC->CR |= RCC_CR_HSEON;
  RCC->CR |= RCC_CR_HSION;
  while (~RCC->CR & RCC_CR_HSERDY) {}
  while (~RCC->CR & RCC_CR_HSIRDY) {}
  RCC->CFGR &= ~(RCC_CFGR_SW);
  RCC->CFGR |= RCC_CFGR_SW_PLL;
  while (~RCC->CFGR & RCC_CFGR_SWS_PLL) {}
  
  RCC->AHBENR |= /*right button*/ RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN;
  RCC->APB1ENR |= RCC_APB1ENR_TIM6EN | RCC_APB1ENR_TIM7EN | RCC_APB1ENR_USART3EN;
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
  
  GPIOA->PUPDR |= /*right button*/ GPIO_PUPDR_PUPDR0_1;
  
  GPIOB->MODER |=  /*I2C1*/ GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1;
  GPIOB->OTYPER |= /*I2C1*/ GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7; //open drain
  GPIOB->OSPEEDR |= /*I2C1*/ GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7;
  GPIOB->PUPDR |= /*I2C1*/ GPIO_PUPDR_PUPDR6_0 | GPIO_PUPDR_PUPDR7_0; //pull up
  GPIOB->AFR[0] |= (0x4 << 24) | (0x4 << 28);
  
  RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
  
  GPIOC->MODER |=  /*USART3*/ GPIO_MODER_MODER10_1 | GPIO_MODER_MODER11_1;
  GPIOC->OSPEEDR |= /*USART3*/ GPIO_OSPEEDER_OSPEEDR10 | GPIO_OSPEEDER_OSPEEDR11;
  GPIOC->PUPDR |= /*button*/ GPIO_PUPDR_PUPDR13_0;
  GPIOC->AFR[1] |= (0x7 << 8) | (0x7 << 12);
  
  SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;
  EXTI->IMR |= EXTI_IMR_MR0 | EXTI_IMR_MR13;
  EXTI->EMR |= EXTI_EMR_MR0 | EXTI_EMR_MR13;
  EXTI->RTSR |= EXTI_FTSR_TR0;
  EXTI->FTSR |= EXTI_FTSR_TR13;
  
  USART3->SR &= ~USART_SR_TC;
  USART3->CR1 |= USART_CR1_UE | USART_CR1_RXNEIE | USART_CR1_TCIE; 
  USART3->BRR |= 0x8F;//0x8F for 56000; 0x341 for 9600
  //txe не включен, cleared by a write to DR
  //tc cleared by reading sr and writing dr, or by writin zero
  //загуглит про флаги?
  
  I2C1->CR2 |= I2C_CR2_ITBUFEN | I2C_CR2_ITEVTEN | I2C_CR2_ITERREN | I2C_CR2_FREQ_3;
  I2C1->CCR |= 40;
  I2C1->TRISE = 9;
  I2C1->CR1 |= I2C_CR1_PE | I2C_CR1_ACK;
  
  TIM6->PSC = 8000;
  TIM6->ARR = 1000;
  TIM6->DIER |= TIM_DIER_UIE;
  
  TIM7->PSC = 0;
  TIM7->ARR = 100;
  TIM7->DIER |= TIM_DIER_UIE;
  
  USART3->CR1 |= USART_CR1_RE | USART_CR1_TE;
  TIM7->CR1 |= TIM_CR1_CEN;
  
  I2C_InitializeSensors();
  
  I2C_SetStart(true);
  TIM6->CR1 |= TIM_CR1_CEN;  
}

//ошибки когда еще не пришло 256 символов; не пришло, но в буффере уже может есть


//-----------------------------------------------------------------------------