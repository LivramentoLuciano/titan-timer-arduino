#include "src/methods/methods.h"
#include "src/model/Routine.h"
#include "src/Display/display.h"
#include "src/miTimer/miTimer.h"

Routine routine = Routine();
extern Display display;
extern volatile bool newSecond;
extern volatile int seconds;

void setup() { initTitan(); }

void loop()
{
  switch (routine.get_instance())
  {
  case NOTHING:
    Serial.println("Prueba basica de 'routine' -> A la espera de comandos");
    delay(1000);
    // simulo llegada de rutina
    resetAndEnableTimer();
    routine.init(8, 4, 12, 2, 2);
    break;

  case INIT:
    if (newSecond)
    {
      newSecond = false;
      routine.set_t(seconds);

      if (routine.get_tLeft() == 0)
      {
        seconds = 0; // Cuando resuelva el problema de librerias al poner routine general en "definiciones.h", este 'seconds' se va (y el timer modifica directo routine.seconds++)
        routine.nextInstance();
      }
      // display.updateTime(routine.get_tLeft()); // Ultimo, si pasa a nextInstance ya muestra el 't' y no '0'
      display.updateAll(routine.get_tLeft(), routine.get_actualRound(), routine.get_actualSet());
    }
    break;

  case WORK:
    if (newSecond)
    {
      newSecond = false;
      routine.set_t(seconds);

      if (routine.get_tLeft() == 0)
      {
        seconds = 0;
        routine.nextInstance();
      }
      display.updateAll(routine.get_tLeft(), routine.get_actualRound(), routine.get_actualSet());
    }
    break;

  case REST:
    if (newSecond)
    {
      newSecond = false;
      routine.set_t(seconds);

      if (routine.get_tLeft() == 0)
      {
        seconds = 0;
        routine.nextInstance();
      }
      display.updateAll(routine.get_tLeft(), routine.get_actualRound(), routine.get_actualSet());
    }
    break;

  case REST_SET:
    if (newSecond)
    {
      newSecond = false;
      routine.set_t(seconds);

      if (routine.get_tLeft() == 0)
      {
        seconds = 0;
        routine.nextInstance();
      }

      display.updateAll(routine.get_tLeft(), routine.get_actualRound(), routine.get_actualSet());
    }
    break;

  default:
    break;
  }
}
