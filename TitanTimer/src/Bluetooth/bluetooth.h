#ifndef BLUETOOTH_H
#define BLUETOOTH_H
#include <Arduino.h>

// Ejemplo de comm: {S;tabata;5;3;14;2;1;}
const char TRAMA_INI = '{';
const char TRAMA_END = '}';
const char MAX_DATA_LENGTH = 100;
const char LOAD_ROUTINE_HEADER = 'L';
const char START_HEADER = 'S';
const char PAUSE_HEADER = 'P';
const char RESUME_HEADER = 's';
const char ROUND_UP_HEADER = 'R';
const char ROUND_DOWN_HEADER = 'r';
const char REPLAY_HEADER = 'b';
const char FORWARD_HEADER = 'f';

class Bluetooth
{
private:
public:
  char buffer[MAX_DATA_LENGTH] = "";
  bool newCommand = false;

  Bluetooth() {}

  void init() { Serial.begin(9600); }
  void check4data();
  void processCommand(char *cmd);

  // Estos deberian estar fuera de Bluetooth, metodos aparte, por ahora lo dejo aca
  void handleLoadRoutine(char *data);
  void handleStart(char *data);
  void handlePause(char *data);
  void handleResume(char *data);
  void handleRoundUp(char *data);
  void handleRoundDown(char *data);
};

#endif