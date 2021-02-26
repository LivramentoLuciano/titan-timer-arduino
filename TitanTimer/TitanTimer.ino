#include "src/methods/methods.h"
#include "src/model/Routine.h"

Routine routine = Routine();

void setup()
{
  initTitan();
}

void loop()
{
  switch (routine.get_instance())
  {
  case NOTHING:
    Serial.println("Prueba basica de 'routine' -> A la espera de comandos");
    delay(1000);
    routine.init(50, 10, 60, 6, 4);
    break;

  case INIT:
    Serial.println("En instancia INIT -> Comienza en 3, 2, 1...");
    delay(3000);
    routine.set_instance(WORK);
    break;

  case WORK:
    Serial.print("En instancia WORK -> Duración: ");
    Serial.println(routine.get_tWork());
    break;

  case REST:
    Serial.print("En instancia REST -> Duración: ");
    Serial.println(routine.get_tRest());
    break;

  case REST_SET:
    Serial.print("En instancia REST_SET -> Duración: ");
    Serial.println(routine.get_tRestSets());
    break;

  default:
    break;
  }
}
