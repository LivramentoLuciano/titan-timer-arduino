#include "src/methods/methods.h"
#include "src/model/Routine.h"
#include "src/Display/display.h"
#include "src/miTimer/miTimer.h"
#include "src/Bluetooth/bluetooth.h"
#include "src/Alarm/alarm.h"

Routine routine = Routine();
extern Display display;
extern volatile bool newSecond;
extern volatile int seconds;
extern Bluetooth bluetooth;
extern TimerState timerState;
extern unsigned long t_doneIdleMsg;
extern bool doneSliderMsg;
Alarma alarmaLastSeconds = Alarma();

void setup() { initTitan(); }

void loop()
{
  bluetooth.check4data();

  if (bluetooth.newCommand == true)
    bluetooth.processCommand(bluetooth.buffer);

  switch (routine.get_instance())
  {
  case NOTHING:
    if (!doneSliderMsg) // muestra msj idle
      display.showIdleMsg();
    else if (millis() - t_doneIdleMsg > 10000) // al acabar, luego de 10s, vuelve a mostrar
      display.resetIdleMsg();

    break;

  case INIT:
    if (routine.lastSeconds() && timerState == STARTED)
      alarmaLastSeconds.on(ALARM_NORMAL);
    else
      alarmaLastSeconds.off();

    if (newSecond)
    {
      newSecond = false;
      routine.set_t(seconds);
      display.updateInitMsg(routine.get_tLeft());

      if (routine.get_tLeft() == 0)
      {
        seconds = 0; // Cuando resuelva el problema de librerias al poner routine general en "definiciones.h", este 'seconds' se va (y el timer modifica directo routine.seconds++)
        routine.nextInstance();
        display.showNewInstanceMsg(routine.get_instance(), routine.get_mode()); // msj segun instancia y modo
      }
    }
    break;

  case FINISHED:
    resetTimer();
    timerState = STOPPED;
    bluetooth.sendTimerState(); // finished
    display.showNewInstanceMsg(routine.get_instance(), routine.get_mode());
    delay(2000);
    display.clrscr();
    routine.set_instance(NOTHING);
    break;

  default: // WORK/REST/REST_SETS
    if (routine.lastSeconds() && timerState == STARTED)         // No se enciende si esta en pausa
      if (routine.get_tLeft() == 1) alarmaLastSeconds.onCont(); // ultimo segundo, beep continuo
      else alarmaLastSeconds.on(routine.get_alarmMode());       // beeps intermitentes
    else
      alarmaLastSeconds.off();                                  // Por si justo pauso cuando sonaba -> La apago

    if (newSecond)
    {
      newSecond = false;
      routine.set_t(seconds);
      // elimine 'updateAll' -> Evito parpadeo constante (hacia clrscr())

      if (routine.get_t() == 1) // En el primer segundo (parche)
      {
        display.clrscr(); // borro NewInstanceMsg
        display.updateInstance(routine.get_instanceString());
        display.updateRound(routine.get_actualRound(), routine.get_rounds());
        display.updateSet(routine.get_actualSet(), routine.get_sets()); // Parche filtrado COMB y AMRAP direct en updateSet
      }

      display.updateTime(routine.get_tLeft()); // va dsp por el 'clrscr' del 1er seg

      if (routine.get_tLeft() == 0) // Instancia finalizada
      {
        seconds = 0;
        routine.nextInstance();
        display.showNewInstanceMsg(routine.get_instance(), routine.get_mode());
      }
    }
    break;
  }
}