#ifndef miTimer_H
#define miTimer_H

#include <Arduino.h>
#include "TimerOne.h"


enum TimerState {
    STARTED,
    PAUSED,
    STOPPED,
};

ISR(TIMER1_COMPA_vect);

void configTimer();
void resetAndEnableTimer();
void resetTimer();
void pauseTimer();
void resumeTimer();
void secondsDownTimer(int dS);
void secondsUpTimer(int dS);
char timerStateChar();
#endif