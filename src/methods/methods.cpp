#include <Arduino.h>
#include "methods.h"
#include "../definiciones.h"
#include "../miTimer/miTimer.h"

extern Display display;

void initTitan()
{
  for (int i = 0; i < 4; i++)
    pinMode(TEST_LEDS[i], OUTPUT);

  configTimer();
  display.init();
  Serial.begin(9600);
}

void testLeds()
{
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(TEST_LEDS[i], HIGH);
    delay(1000);
    digitalWrite(TEST_LEDS[i], LOW);
    delay(1000);
  }
}

// Convierte segundos a mm:ss
String mmss(int secs)
{
  int m = secs / 60;
  int s = secs % 60;

  String mm = "0";
  String ss = "0";

  if (m < 10)
    mm += String(m);
  else
    mm = String(m);
  if (s < 10)
    ss += String(s);
  else
    ss = String(s);

  return mm + ":" + ss;
}