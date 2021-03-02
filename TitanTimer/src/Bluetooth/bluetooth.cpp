#include "bluetooth.h"

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

void Bluetooth::processCommand(String cmd)
{
  Serial.println("recibio un comando. Procesando...");

  newCommand = false;
  buffer[0] = '\0'; // limpio el buffer RX
}