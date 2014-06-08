#ifndef TIMER_H
#define TIMER_H

#define TIM6_NAME "Timer6"
#define TIM7_NAME "Timer7"

#include "stdint.h"
#include "stdbool.h"

//timer6 and timer7 are on apb1 bus

typedef struct Timer
{
  void(**Handlers)();
  uint8_t HandlersAmount;
}Timer;

//returns true if frequency is available, false - if unavailable
bool Timer_Start(const char* timName, uint32_t microseconds);

void Timer_Stop(const char* timName);

bool Timer_Attach(const char* timName, void(*)());

#endif