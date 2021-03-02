#ifndef BLUETOOTH_H
#define BLUETOOTH_H
#include <Arduino.h>

const char TRAMA_INI = '{';
const char TRAMA_END = '}';
const char MAX_DATA_LENGTH = 100;

class Bluetooth
{
private:

public:
  char buffer[MAX_DATA_LENGTH] = "";
  bool newCommand = false;

  Bluetooth() {}

  void init() { Serial.begin(9600); }
  void check4data();
  void processCommand(String cmd);
};

#endif