#include <Arduino.h>
#include "methods.h"
#include "../definiciones.h"

void initTitan()
{
    for (int i = 0; i < 4; i++)
        pinMode(TEST_LEDS[i], OUTPUT);
    
    Serial.begin(9600);
}

void testLeds()
{
    for (int i=0; i < 4; i++){
        digitalWrite(TEST_LEDS[i], HIGH);
        delay(1000);
        digitalWrite(TEST_LEDS[i], LOW);
        delay(1000);
    }
}