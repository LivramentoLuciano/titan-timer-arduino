#include "bluetooth.h"
#include "../model/Routine.h"
#include "../Display/display.h"
#include "../miTimer/miTimer.h"

extern Routine routine; // Por los handlers (pero deberia hacerse en 'methods')
extern Display display;

void Bluetooth::check4data()
{
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char rc;

  while (Serial.available())
  {
    rc = Serial.read();
    //Serial.print("lee datos UART: "); Serial.write(rc); Serial.prinln("");
    if (recvInProgress == true)
    {
      if (rc != TRAMA_END)
      {
        buffer[ndx] = rc;
        ndx++;
        if (ndx > MAX_DATA_LENGTH)
          ndx = MAX_DATA_LENGTH;
      }
      else
      {
        buffer[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newCommand = true;
      }
    }
    else if (rc == TRAMA_INI)
      recvInProgress = true;
  }
}

void Bluetooth::processCommand(char *cmd)
{
  Serial.print("Recibio comando: " + String(cmd) + " - Procesando...");

  char *_header;
  char *_data;
  char *_strtokIndx; // indice para 'strtok'

  _header = strtok(cmd, ";"); // el primer valor es el 'header'
  _data = strtok(NULL, "\0"); // el resto es la 'data'

  switch (_header[0])
  {
  case LOAD_ROUTINE_HEADER:
    handleLoadRoutine(_data);
    break;
  case START_HEADER:
    handleStart(_data);
    break;
  case PAUSE_HEADER:
    handlePause(_data);
    break;
  case RESUME_HEADER:
    handleResume(_data);
    break;

  default:
    break;
  }

  newCommand = false;
  buffer[0] = '\0'; // limpio el buffer RX
}

void Bluetooth::handleLoadRoutine(char *_data)
{
  // Serial.print("Carga rutina: ");
  // Serial.println(_data);
  char *_strtokIndx;

  char *_mode = strtok(_data, ";");

  _strtokIndx = strtok(NULL, ";"); // this continues where the previous call left off
  int _tWork = atoi(_strtokIndx);

  _strtokIndx = strtok(NULL, ";");
  int _tRest = atoi(_strtokIndx);

  _strtokIndx = strtok(NULL, ";");
  int _tRestSets = atoi(_strtokIndx);

  int _rounds = atoi(strtok(NULL, ";")); // resolver que sea un char y no un char*
  int _sets = atoi(strtok(NULL, ";"));

  routine.set_settings(_tWork, _tRest, _tRestSets, (char)_rounds, (char)_sets);
  resetTimer();
}

void Bluetooth::handleStart(char *_data)
{
  if (routine.get_isLoaded())
  {
    // Serial.print("Start: ");
    // Serial.println(_data);

    routine.init();
    resetAndEnableTimer();
    display.clrscr();
    display.updateInitMsg(routine.get_tLeft());
  }
  else
  {
    // Aqui podria pedir a la app la rutina (En casos de reencendido del micro y celu estaba andando y quedaron desincronizados)
    Serial.println("Rutina no cargada");
  }
}

void Bluetooth::handlePause(char *_data)
{
  Serial.print("Pause: ");
  Serial.println(_data);
}

void Bluetooth::handleResume(char *_data)
{
  Serial.print("Resume: ");
  Serial.println(_data);
}

void Bluetooth::handleRoundUp(char *_data)
{
  Serial.print("RoundUp: ");
  Serial.println(_data);
}

void Bluetooth::handleRoundDown(char *_data)
{
  Serial.print("RoundDown: ");
  Serial.println(_data);
}