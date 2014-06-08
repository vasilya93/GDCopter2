#include "Timer.h"
#include "ClockControl.h"

Timer Timer6, Timer7;

bool Timer_Start(const char* timName, uint32_t microseconds)
{
  int a = 1;
  switch(a)
  {
  case 1://TIM6_NAME:
    {
    }
    break;
  case 2://TIM7_NAME:
    {
    }
    break;
  }
  return false;
}

//-----------------------------Handlers-----------------------------------------

/*void TIM6_IRQHandler(void)
{  
  //TIM6->SR &= ~TIM_SR_UIF;
}

void TIM7_IRQHandler(void)
{  
  //TIM7->SR &= ~TIM_SR_UIF;
}*/