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
  // Serial.print("Recibio comando: " + String(cmd) + " - Procesando...");

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
  case ROUND_UP_HEADER:
    handleRoundUp(_data);
    break;
  case ROUND_DOWN_HEADER:
    handleRoundDown(_data);
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

  sendOk(LOAD_ROUTINE_HEADER);
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
    sendOk(START_HEADER);
  }
  else
  {
    // Aqui podria pedir a la app la rutina (En casos de reencendido del micro y celu estaba andando y quedaron desincronizados)
    // Serial.println("Rutina no cargada");
  }
}

void Bluetooth::handlePause(char *_data)
{
  pauseTimer();
  sendOk(PAUSE_HEADER);
}

void Bluetooth::handleResume(char *_data)
{
  resumeTimer();
  sendOk(RESUME_HEADER);
}

void Bluetooth::handleRoundUp(char *_data)
{
  if (routine.isLastRound())
  {
    if (!routine.isLastSet()) // en ultimo set omito
    {
      routine.set_actualRound(1);
      routine.setUp();
      routine.set_instance(WORK);
      routine.set_t(0);
      resetTimer(); // lo tira a 0 y pausa
      display.updateAll(routine.get_tLeft(), routine.get_actualRound(), routine.get_rounds(), routine.get_actualSet(), routine.get_sets(), routine.get_instanceString());
    }
  }
  else
  {
    routine.roundUp();
    routine.set_instance(WORK);
    routine.set_t(0);
    resetTimer();
    // display.clrscr();
    // resolver esto del updateAll q esta horrible
    display.updateAll(routine.get_tLeft(), routine.get_actualRound(), routine.get_rounds(), routine.get_actualSet(), routine.get_sets(), routine.get_instanceString());
  }
  sendOk(ROUND_UP_HEADER);
}

void Bluetooth::handleRoundDown(char *_data)
{
  if (routine.get_actualRound() > 1)
  {
    routine.roundDown();
    routine.set_instance(WORK);
    routine.set_t(0);
    resetTimer(); // lo tira a 0 y pausa
    display.updateAll(routine.get_tLeft(), routine.get_actualRound(), routine.get_rounds(), routine.get_actualSet(), routine.get_sets(), routine.get_instanceString());
  }
  else
  {
    if (routine.get_actualSet() > 1)
    {
      routine.set_actualRound(routine.get_rounds());
      routine.setDown();
      routine.set_instance(WORK);
      routine.set_t(0);
      resetTimer(); // lo tira a 0 y pausa
      display.updateAll(routine.get_tLeft(), routine.get_actualRound(), routine.get_rounds(), routine.get_actualSet(), routine.get_sets(), routine.get_instanceString());
    }
  }
  sendOk(ROUND_DOWN_HEADER);
}

// Responde Recibido Ok: {OK_HEADER; type}
// type: es el tipo de comando por el que esta respondiendo el Ok (sera el ..._HEADER)
void Bluetooth::sendOk(char type)
{
  String _trama = "";
  _trama.concat(TRAMA_INI);
  _trama.concat(RESPONSE_OK_HEADER);
  _trama.concat(TRAMA_SEPARATOR);
  _trama.concat(type);
  _trama.concat(TRAMA_SEPARATOR);
  _trama.concat(TRAMA_END);

  // Lo dejo asi para probar, porque el siguiente modo es mas corto pero no puedo castear a String los char correctamente
  //  _trama = "{K;" + ... +";}";
  // Igual tampoco podria castear el type, asi q toy en la misma, no podria usarlo ese

  Serial.println(_trama);
}

// Por tema de 'state' en App. Resuelve que al finalizar se ponga en 'stopped'
// Igual voy a ver si saco los 'state' x el problema de background process
// que no se actualizarian si estoy con la app en 2* plano
// sacar 'state' tendria que separar boton play y pause y el problema q tenia de detectar el 'state' para saber si mando la rutina o no, se me acaba de ocurrir q lo puedo resolver en el micro, que le diga 'che, me mandaste play pero no tengo la rutina, pasamela0
void Bluetooth:: sendFinished(){
  String _trama = "";
  _trama.concat(TRAMA_INI);
  _trama.concat(FINISHED_HEADER);
  _trama.concat(TRAMA_SEPARATOR);
  _trama.concat(TRAMA_END);
  Serial.print(_trama);
}