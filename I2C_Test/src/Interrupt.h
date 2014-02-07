#include "stm32l1xx.h"

#ifndef INTERRUPT_H
#define INTERRUPT_H

#define USART3_BUSY 0x01
#define I2C1_COMMAND_SENT 0x02
#define STAT1_LED_ON 0x04
#define STAT2_LED_ON 0x08
#define STAT3_LED_ON 0x10
#define STAT4_LED_ON 0x20
#define I2C1_COMMAND_PHASE 0x40
#define COUNTING_OVERSAMPLING 0x800
#define RIGHT_BUTTON_PRESSED 0x100
#define RIGHT_BUTTON_FOR_WRITE 0x200
#define I2C1_SENSORS_INITIALIZE 0x400

extern uint16_t PeripheralStatus;

void TIM6_IRQHandler(void);
void TIM7_IRQHandler(void);
void I2C1_EV_IRQHandler(void);
void I2C1_ER_IRQHandler(void);
void USART3_IRQHandler(void);
void ADC1_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
void FleshLeds();

#endif