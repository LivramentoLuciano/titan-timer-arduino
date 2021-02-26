#include <Arduino.h>
#include "display.h"

void Display:: updateTime(int t){
    Serial.print("simulo display -> ");
    Serial.println(t);
}