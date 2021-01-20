#include <Arduino.h>
#include "definiciones.h"

//int t = 0; // Para tratar de solucionar problema de interrupcionTimer, que apenas habilitas entra a la interrupción, sin esperar a que se haya llenado el registro

/*
 * Filtro Tramas
 * Detecta las tramas de los mensajes recibidos.
 * Guarda caracter a caracter en buffer y activa un flag al detectar finalmente una trama recibida
 */
void filtro_trama(char *cG_bufferRx, char *nuevaTrama )
{
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '{';
    char endMarker = '}';
    char maxDataLength = 100;
    char rc;

   
    //Serial.print("lee datos UART: ");
    rc = Serial.read();
    //Serial.write(rc);//Serial.println("");
    if (recvInProgress == true)
    {
      if (rc != endMarker)
      {
        cG_bufferRx[ndx] = rc;
        ndx++;
        if (ndx > maxDataLength) { ndx = maxDataLength; }
      }
      else
      {    
        //Serial.print("Encontro marca Fin.");
        cG_bufferRx[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        *nuevaTrama = true;//Serial.print(" Estado nuevaTrama: ");Serial.println(*nuevaTrama,DEC);
      }
    }
    else
    {
      if(rc == startMarker)
      {
        //Serial.println("Encontro marca Inicio"); 
        recvInProgress = true;
      }
    }
}


void procesar_trama(char *trama, char *nuevaTrama, int *ptr_duracionActividad, int *ptr_duracionDescanso, int *ptr_totalRounds, int *ptr_totalSets, int *ptr_duracionDescansoSets, int *ptr_instanciaRutina, volatile int *ptr_segundos, int *ptr_alarmaON)
{
  char *header;
  char *dato;
  static char copia_trama[20];
  static char copia_dato[20];//uso esto de copiar el "elemento datos" de la trama porque (no se porqué) fallaba al aplicarle el subStr, para quedarme con cada partecita de los datos recibidos

  strcpy(copia_trama,trama);
  header = subStr(copia_trama,";",1); //1 ->POS_COMANDO
  dato = subStr(trama,";",2); //2 ->POS_DATO (dentro de la trama, ponerlos en defines)// sacar esta linea, ya que segun el header 'datos' por ahi esta en otra posicion
  strcpy(copia_dato, dato);
  
  switch (header[0])
  {
    case HDR_START: //'S' Comienza rutina
    { 
      *ptr_instanciaRutina = INIT;
      *ptr_duracionActividad = String(subStr(copia_trama,";",3)).toInt();
      *ptr_duracionDescanso = String(subStr(copia_trama,";",4)).toInt();
      *ptr_duracionDescansoSets = String(subStr(copia_trama,";",5)).toInt();
      *ptr_totalRounds= String(subStr(copia_trama,";",6)).toInt();
      *ptr_totalSets = String(subStr(copia_trama,";",7)).toInt();

      //cuentaDisplay(*ptr_duracionActividad - segundos);
      cuentaDisplay(duracionInit);
      TCNT1  = 0;                //Inicializa el contador del Timer en 0
      TIFR1 = (1<<OCF1A);        //Limpio el flag para evitar que dispare interrupcion apenas habilito, no se xq es con '1' en vez de '0' que se limpia,porque en el manual dice otra cosa //TIFR1 &= ~(1<<OCF1A);
      TIMSK1 |= (1 << OCIE1A);   //Habilita interrupción por comparación  
      //Timer1.restart();
      //Timer1.start();
      /*Serial.println("");
      Serial.println("Iniciar rutina de: .");
      Serial.println("Act: " + String(*ptr_duracionActividad));
      Serial.println("Desc: " + String(*ptr_duracionDescanso));
      Serial.println("Rounds: " + String(*ptr_totalRounds));
      Serial.println("Sets: " + String(*ptr_totalSets));
      Serial.println("Descanso Sets: " + String(*ptr_duracionDescansoSets));*/
      
    }
    break; 
    
    case HDR_PAUSE:
    {
      TCNT1  = 0;                //Inicializa el contador del Timer en 0, para que no quede desfasado con el timer de la APP (el cual esperara 1 segundo entero al reanudar)
      TIMSK1 &= ~(1 << OCIE1A);   //Inhabilita interrupción por comparación  
      //Timer1.restart(); //Para que no quede desfasado con el timer de la APP (el cual esperara 1 segundo entero al reanudar)
      //Timer1.stop();
      *ptr_alarmaON = false; //desactivo alarma, caso de pausar justo en ese momento //ver si armar estados distintos (instanciaRutina = PAUSA)
      //*ptr_instanciaRutina = 
    }break;
    
    case HDR_RESUME:
    {
      TCNT1  = 0;
      TIFR1 = (1<<OCF1A);        //Limpio el flag para evitar que dispare interrupcion apenas habilito, no se xq es con '1' en vez de '0' que se limpia,porque en el manual dice otra cosa //TIFR1 &= ~(1<<OCF1A);TIFR1 = (1<<OCF1A); //TIFR1 &= ~(1<<OCF1A);      //Limpio el flag para evitar que dispare interrupcion apenas habilito
      TIMSK1 |= (1 << OCIE1A);   //Habilita interrupción por comparación
      //Timer1.start();
      *ptr_alarmaON = true;      
      //*ptr_instanciaRutina = 
    }break;
       
    case HDR_STOP:
    {
      //*ptr_segundos = 0;
      TCNT1  = 0;                //Inicializa el contador del Timer en 0
      TIMSK1 &= ~(1 << OCIE1A);   //Inhabilita interrupción por comparación       
      //Timer1.restart();
      //Timer1.stop();
      *ptr_instanciaRutina = FINALIZADA;
    }break;    
  }
  trama[0] = '\0';
  *nuevaTrama = false;
}

char* subStr (char* str, char *delim, int index) 
{
  char *act, *sub, *ptr;
  static char copy[20];
  int i;

  // Since strtok consumes the first arg, make a copy
  strcpy(copy, str);

  for (i = 1, act = copy; i <= index; i++, act = NULL) 
  {
    //Serial.print(".");
    sub = strtok_r(act, delim, &ptr);
    if (sub == NULL) break;
  }
  return sub;
}

