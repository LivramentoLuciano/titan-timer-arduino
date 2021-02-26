#include "src/methods/methods.h"
#include "src/model/Routine.h"
#include "src/Display/display.h"
#include "src/miTimer/miTimer.h"

Routine routine = Routine();
Display display = Display();
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
    routine.init(50, 10, 60, 6, 4);
    break;

  case INIT:
    if (newSecond)
    {
      newSecond = false;
      routine.set_t(seconds);
      int _tLeft = routine.get_tLeft();
      Serial.print("En instancia INIT: ");
      display.updateTime(_tLeft);

      if (_tLeft == 0)
        routine.set_instance(WORK);
    }

    break;

  case WORK:
    if (newSecond)
    {
      newSecond = false;
      Serial.print("En instancia WORK -> Duracion: ");
      Serial.println(routine.get_tWork());
    }
    break;

  case REST:
    if (newSecond)
    {
      newSecond = false;
      Serial.print("En instancia REST -> Duracin: ");
      Serial.println(routine.get_tRest());
    }
    break;

  case REST_SET:
    if (newSecond)
    {
      newSecond = false;
      Serial.print("En instancia REST_SET -> Duracion: ");
      Serial.println(routine.get_tRestSets());
    }
    break;

  default:
    break;
  }
}
