#include "miTimer.h"

volatile int seconds = 0;
volatile bool newSecond = false;
TimerState timerState = STOPPED;

ISR(TIMER1_COMPA_vect)
{
    seconds++;
    newSecond = true;
}

void configTimer()
{
    //  Timer1 Modo: CTC, 1Hz de frecuencia de interrupcion
    TCCR1A = 0;                                         //  El registro de control A queda todo en 0
    TCCR1B = 0;                                         //  Limpia el Registro de control B
    TCNT1 = 0;                                          //  Inicializa el contador del Timer en 0
    OCR1A = 0x3D09;                                     //  Registro de comparación, nos da interrupcion 1 seg (1Hz): 16MHz/1024/1Hz=15625=0x3D09
    TCCR1B |= (1 << WGM12) | (1 << CS10) | (1 << CS12); //  Establece modo CTC, prescaler 1024
    TIMSK1 &= ~(1 << OCIE1A);                           //  De arranque inhabilita interrupción por comparación
}

void resetAndEnableTimer()
{
    TCNT1 = 0;               // Inicializa el contador del Timer en 0
    TIFR1 = (1 << OCF1A);    // Limpio el flag para evitar que dispare interrupcion apenas habilito, no se xq es con '1' en vez de '0' que se limpia,porque en el manual dice otra cosa //TIFR1 &= ~(1<<OCF1A);
    TIMSK1 |= (1 << OCIE1A); // Habilita interrupción por comparación
    seconds = 0;
    newSecond = false;
    timerState = STARTED;
}

void resetTimer()
{
    // inhabilitar timer, seconds = 0, newSecond = false
    TCNT1 = 0;                //Inicializa el contador del Timer en 0
    TIMSK1 &= ~(1 << OCIE1A); //Inhabilita interrupción por comparación
    seconds = 0;
    newSecond = false;
    timerState = PAUSED;
}

void pauseTimer()
{
    TCNT1 = 0;                //Inicializa el contador del Timer en 0, para que no quede desfasado con el timer de la APP (el cual esperara 1 segundo entero al reanudar)
    TIMSK1 &= ~(1 << OCIE1A); //Inhabilita interrupción por comparación
    timerState = PAUSED;
}

void resumeTimer()
{
    TIFR1 = (1 << OCF1A);    //Limpio el flag para evitar que dispare interrupcion apenas habilito, no se xq es con '1' en vez de '0' que se limpia,porque en el manual dice otra cosa //TIFR1 &= ~(1<<OCF1A);TIFR1 = (1<<OCF1A); //TIFR1 &= ~(1<<OCF1A);      //Limpio el flag para evitar que dispare interrupcion apenas habilito
    TCNT1 = 0;               //Inicializa el contador del Timer en 0, para que no quede desfasado con el timer de la APP (el cual esperara 1 segundo entero al reanudar)
    TIMSK1 |= (1 << OCIE1A); //Habilita interrupción por comparación
    timerState = STARTED;
}