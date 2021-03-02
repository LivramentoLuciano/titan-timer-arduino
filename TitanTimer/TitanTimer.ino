#include "src/methods/methods.h"
#include "src/model/Routine.h"
#include "src/Display/display.h"
#include "src/miTimer/miTimer.h"
#include "src/Bluetooth/bluetooth.h"

Routine routine = Routine();
extern Display display;
extern volatile bool newSecond;
extern volatile int seconds;
extern Bluetooth bluetooth;

void setup() { initTitan(); }

void loop()
{
  bluetooth.check4data();
  
  if (bluetooth.newCommand == true)
    bluetooth.processCommand(bluetooth.buffer);
  
  switch (routine.get_instance())
  {
  case NOTHING:
    // ver si hacer algo aca
    break;

  case INIT:
    if (newSecond)
    {
      newSecond = false;
      routine.set_t(seconds);
      display.updateInitMsg(routine.get_tLeft());

      if (routine.get_tLeft() == 0)
      {
        seconds = 0; // Cuando resuelva el problema de librerias al poner routine general en "definiciones.h", este 'seconds' se va (y el timer modifica directo routine.seconds++)
        routine.nextInstance();
        display.showNewInstanceMsg("WORK!");
      }
      // display.updateTime(routine.get_tLeft()); // Ultimo, si pasa a nextInstance ya muestra el 't' y no '0'
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
      display.updateAll(routine.get_tLeft(), routine.get_actualRound(), routine.get_rounds(), routine.get_actualSet(), routine.get_sets(), routine.get_instanceString());
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
      display.updateAll(routine.get_tLeft(), routine.get_actualRound(), routine.get_rounds(), routine.get_actualSet(), routine.get_sets(), routine.get_instanceString());
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

      display.updateAll(routine.get_tLeft(), routine.get_actualRound(), routine.get_rounds(), routine.get_actualSet(), routine.get_sets(), routine.get_instanceString());
    }
    break;
  
  default:
    break;
  }
}
