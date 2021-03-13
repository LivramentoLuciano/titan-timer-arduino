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

  case TIMER_STATE_HEADER:
    handleRequestTimerState(_data);
    break;

  case REPLAY_HEADER:
    handleReplay(_data);
    break;

  case FORWARD_HEADER:
    handleForward(_data);
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

  routine.set_settings(_mode, _tWork, _tRest, _tRestSets, (char)_rounds, (char)_sets);
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
    display.resetIdleMsg();
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
  if (routine.enabled())
  {
    pauseTimer();
    sendOk(PAUSE_HEADER);
  }
}

void Bluetooth::handleResume(char *_data)
{
  if (routine.enabled())
  { // ver si hacer asi o responder confirmando el msj, pero avisando que se omitio la accion
    resumeTimer();
    sendOk(RESUME_HEADER);
  }
}

void Bluetooth::handleRoundUp(char *_data)
{
  if (routine.enabled() && !(routine.get_instance() == INIT)) // No lo meto en 'enabled' para que 'pause/resume' anden
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
        sendOk(ROUND_UP_HEADER); // Lo meto, evito que responda Ok en ultimo round del ultimo set, quizas puedo cambiar a que responda Ok, de recibido pero sin cambios
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
      sendOk(ROUND_UP_HEADER);
    }
  }
}

void Bluetooth::handleRoundDown(char *_data)
{
  if (routine.enabled() && !(routine.get_instance() == INIT))
  {
    if (routine.get_actualRound() > 1)
    {
      if (routine.get_t() == 0)
        routine.roundDown(); // Si es 0 -> resto 1 round, sino solo tiro a 0 el round actual

      routine.set_t(0);
      resetTimer();               // lo tira a 0 y pausa
      routine.set_instance(WORK); // Me asegura funcionamiento correcto tmb en 'rest / restSets'
      display.updateAll(routine.get_tLeft(), routine.get_actualRound(), routine.get_rounds(), routine.get_actualSet(), routine.get_sets(), routine.get_instanceString());
      sendOk(ROUND_DOWN_HEADER); // Lo meto para que no responda Ok en casos que omite (quizas puedo cambiarlo a que responda Ok de recibido, pero avisando que omitido)
    }
    else
    { // primer round
      if ((routine.get_actualSet() > 1) && (routine.get_t() == 0))
      {
        // Si es 0 -> paso a ultimo round del set anterior
        routine.set_actualRound(routine.get_rounds());
        routine.setDown();
      }
      // Primer round del primer set o de cualquier set pero con tiempo actual mayor a 0 -> tiro a 0 (sin cambiar el round)
      routine.set_t(0);
      resetTimer(); // lo tira a 0 y pausa
      routine.set_instance(WORK);
      display.updateAll(routine.get_tLeft(), routine.get_actualRound(), routine.get_rounds(), routine.get_actualSet(), routine.get_sets(), routine.get_instanceString());
      sendOk(ROUND_DOWN_HEADER);
    }
  }
}

void Bluetooth::handleRequestTimerState(char *data)
{
  sendTimerState();
}

void Bluetooth::handleReplay(char *_data)
{
  if (routine.enabled() && !(routine.get_instance() == INIT))
  {
    char *_strtokIndx = strtok(_data, ";");
    int _deltaS = atoi(_strtokIndx); // 'deltaSeconds' recibido

    // No puedo retroceder mas segundos que los actuales
    if (_deltaS > routine.get_t())
      _deltaS = routine.get_t();

    if (routine.get_instance() == WORK)
    { // En descansos, por ahora, omito secondsDown/Up o podria poner que sea siempre X
      secondsDownTimer(_deltaS);
      sendOk(REPLAY_HEADER);
    }
  }
}

void Bluetooth::handleForward(char *_data)
{
  if (routine.enabled() && !(routine.get_instance() == INIT))
  {
    char *_strtokIndx = strtok(_data, ";");
    int _deltaS = atoi(_strtokIndx); // 'deltaSeconds' recibido

    // No puedo avanzar mas segundos que los restantes
    if (_deltaS > routine.get_tLeft())
      _deltaS = routine.get_tLeft();

    if (routine.get_instance() == WORK)
    {
      if (!((routine.isLastRound() && routine.isLastSet()) && _deltaS == routine.get_tLeft()))
      {
        secondsUpTimer(_deltaS);
        sendOk(FORWARD_HEADER);
      }
      // Omito si estoy en el ultimo round del entrenamiento y el tiempo a avanzar es igual al restante 
      // -> No finaliza el entrenamiento 
      // Evito errores por apretar un "avanzar" de mas
    }
    // En descansos, por ahora, omito, podria back/forw un X cte por las dudas
  }
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

// Envia el estado del timer (paused, started, etc)
// Tanto ante un 'request' de la app, como al finalizar la rutina
// Soluciona el conflicto de app yendo a 2* plano
void Bluetooth::sendTimerState()
{
  String _trama = "";
  char _state = timerStateChar();
  _trama.concat(TRAMA_INI);
  _trama.concat(TIMER_STATE_HEADER);
  _trama.concat(TRAMA_SEPARATOR);
  _trama.concat(_state);
  _trama.concat(TRAMA_SEPARATOR);
  _trama.concat(TRAMA_END);
  Serial.print(_trama);
}