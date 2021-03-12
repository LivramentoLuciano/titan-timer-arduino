#include "Arduino.h"
#include "alarm.h"

unsigned long tToogleBeep = 0; // Beep intermitente (toogle on/off)

Alarma:: Alarma(){
  pinMode(PIN_BUZZER, OUTPUT);
  buzzerOff();
}

void Alarma::on()
{
  if (millis() > tToogleBeep + BEEP_INTERVAL)
  { // toogle 'Beep'
    if (isBuzzerOn) buzzerOff();
    else buzzerOn();
    tToogleBeep = millis();
  }
}

void Alarma::off()
{
  buzzerOff();
  tToogleBeep = 0;
}

void Alarma::buzzerOn()
{
  digitalWrite(PIN_BUZZER, ENCENDER_BUZZER);
  isBuzzerOn = true;
}

void Alarma::buzzerOff()
{
  digitalWrite(PIN_BUZZER, APAGAR_BUZZER);
  isBuzzerOn = false;
}