#ifndef miTimer_h
#define miTimer_h

#include <Arduino.h>
#include "TimerOne.h"


ISR(TIMER1_COMPA_vect);

void configTimer();
void resetAndEnableTimer();

#endif