#include "Arduino.h"
#include "alarm.h"

unsigned long tToogleBeep = 0; // Beep intermitente (toogle on/off)

Alarma:: Alarma(){
  pinMode(PIN_BUZZER, OUTPUT);
  buzzerOff();
}

// distinto beep (y lastSeconds) segun modo
void Alarma::on(AlarmMode _mode)
{
  int _beepIntv = BEEP_INTERVAL_NORMAL;
  if (_mode == ALARM_SHORT) _beepIntv = BEEP_INTERVAL_SHORT;

  if (millis() > tToogleBeep + _beepIntv)
  { // toogle 'Beep'
    if (isBuzzerOn) buzzerOff();
    else buzzerOn();
    tToogleBeep = millis();
  }
}

void Alarma:: onCont(){
  buzzerOn();
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