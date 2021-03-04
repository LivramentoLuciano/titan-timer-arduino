#ifndef miTimer_H
#define miTimer_H

#include <Arduino.h>
#include "TimerOne.h"


ISR(TIMER1_COMPA_vect);

void configTimer();
void resetAndEnableTimer();
void resetTimer();
void pauseTimer();
void resumeTimer();
#endif