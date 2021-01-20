#include <Arduino.h>
#include "definiciones.h"

/*
 * Filtro Tramas
 * Detecta las tramas de los mensajes recibidos.
 * Guarda caracter a caracter en buffer y activa un flag al detectar finalmente una trama recibida
 */
//void filtro_trama(char *cG_bufferRx, char *nuevaTrama )
void filtro_trama(char *cG_bufferRx)
{
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '{';
    char endMarker = '}';
    char maxDataLength = 100;
    char rc;

    //Serial.print("lee datos UART: ");
    rc = Serial.read();
    //Serial.write(rc);Serial.println("");
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
        //*nuevaTrama = true;//Serial.print(" Estado nuevaTrama: ");Serial.println(*nuevaTrama,DEC);
        cG_nuevaTrama = true; //Serial.print(" Estado nuevaTrama: ");Serial.println(cG_nuevaTrama,DEC);
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


void procesar_trama()
{
  char *header;
  char * strtokIndx; // this is used by strtok() as an index
  
  Serial.println(cG_bufferRx);
  header = strtok(cG_bufferRx,";"); // get the first part - the string
    
  switch (header[0])
  {
    case HDR_START: //'S' Comienza rutina
    { 
      instanciaRutina = INIT;
      mode = strtok(NULL,";"); // this continues where the previous call left off
      
      strtokIndx  = strtok(NULL,";");
      duracionActividad = atoi(strtokIndx);
      
      strtokIndx = strtok(NULL, ";"); 
      duracionDescanso = atoi(strtokIndx);
      
      strtokIndx = strtok(NULL, ";"); 
      duracionDescansoSets = atoi(strtokIndx);
      
      strtokIndx = strtok(NULL, ";");
      totalRounds = atoi(strtokIndx);
      
      strtokIndx = strtok(NULL, ";");
      totalSets = atoi(strtokIndx);

      actualRound = 1;
      actualSet = 1;
      segundos = 0;
      pasoSegundo = 0;
      
      /*
      // Esto es si cambio a uso de class Routine
      int _workTime = 50, _restTime =10, _restBtwnSetsTime=15, _totalRounds=2, _totalSets=2, _timeLeft=3;
      String _mode, _state;
      _state = "init";*/
      /*_mode = String(subStr(copia_trama,";",2));
      _workTime = String(subStr(copia_trama,";",3)).toInt();
      _restTime = String(subStr(copia_trama,";",4)).toInt();
      _restBtwnSetsTime = String(subStr(copia_trama,";",5)).toInt();
      _totalRounds = String(subStr(copia_trama,";",6)).toInt();
      _totalSets = String(subStr(copia_trama,";",7)).toInt();
      _timeLeft = duracionInit
      //Serial.println("Routine Default:" + String(routine.mode) + ", " + String(routine.workTime)+ ", " +  String(routine.restTime) + "...");
      //routine.setRoutine(_workTime, _restTime, _restBtwnSetsTime, _totalRounds, _totalSets, _timeLeft, "tabata", _state);
      //Serial.println("Routine recibida del celu: " + String(routine.mode) + ", " + String(routine.workTime)+ ", " +  String(routine.restTime) + "...");
      */

      cleanDisplay7(0);
      cuentaDisplay(duracionInit);
      
      TCNT1  = 0;                //Inicializa el contador del Timer en 0
      TIFR1 = (1<<OCF1A);        //Limpio el flag para evitar que dispare interrupcion apenas habilito, no se xq es con '1' en vez de '0' que se limpia,porque en el manual dice otra cosa //TIFR1 &= ~(1<<OCF1A);
      TIMSK1 |= (1 << OCIE1A);   //Habilita interrupción por comparación
      
      Serial.print("Iniciar rutina de: ");Serial.println(mode);
      /*Serial.println("Act: " + String(duracionActividad));
      Serial.println("Desc: " + String(duracionDescanso));
      Serial.println("Rounds: " + String(totalRounds));
      Serial.println("Sets: " + String(totalSets));
      Serial.println("Descanso Sets: " + String(duracionDescansoSets));*/
    }
    break; 
    
    case HDR_PAUSE:
    {
      TCNT1  = 0;                //Inicializa el contador del Timer en 0, para que no quede desfasado con el timer de la APP (el cual esperara 1 segundo entero al reanudar)
      TIMSK1 &= ~(1 << OCIE1A);   //Inhabilita interrupción por comparación  
      alarmaON = false; //desactivo alarma, caso de pausar justo en ese momento //ver si armar estados distintos (instanciaRutina = PAUSA)
    }break;
    
    case HDR_RESUME:
    {
      TCNT1  = 0;
      TIFR1 = (1<<OCF1A);        //Limpio el flag para evitar que dispare interrupcion apenas habilito, no se xq es con '1' en vez de '0' que se limpia,porque en el manual dice otra cosa //TIFR1 &= ~(1<<OCF1A);TIFR1 = (1<<OCF1A); //TIFR1 &= ~(1<<OCF1A);      //Limpio el flag para evitar que dispare interrupcion apenas habilito
      TIMSK1 |= (1 << OCIE1A);   //Habilita interrupción por comparación
      alarmaON = true;
    }break;
       
    case HDR_STOP:
    {
      //segundos = 0;
      TCNT1  = 0;                //Inicializa el contador del Timer en 0
      TIMSK1 &= ~(1 << OCIE1A);   //Inhabilita interrupción por comparación
      instanciaRutina = FINALIZADA;
    }break; 

    case HDR_R_INCREMENT:
    {
      //routine.roundIncrement();
      //El uso de la clase Routine me esta fallando, por ahora lo hago literal en el lugar
      roundIncrement();      
    }break;

    case HDR_R_DECREMENT:
    {
      //routine.roundIncrement();
      //El uso de la clase Routine me esta fallando, por ahora lo hago literal en el lugar
      roundDecrement();      
    }break;

    case HDR_S_INCREMENT:
    {
      //routine.roundIncrement();
      //El uso de la clase Routine me esta fallando, por ahora lo hago literal en el lugar
      setIncrement();      
    }break;

    case HDR_S_DECREMENT:
    {
      //routine.roundIncrement();
      //El uso de la clase Routine me esta fallando, por ahora lo hago literal en el lugar
      setDecrement();      
    }break;

    case HDR_RESINCRONIZE:
    {
      Serial.println("RESINCRONIZAR");
      
      // Antes que nada, paro el reloj (dsp ver como se va a resolver bien, si la app primero manda un pause, o en el sincronize, detiene el timer
      // y si luego de resincronize, hay que presionar si o si un "continue"
      // Limpio el flag para evitar que dispare interrupcion apenas habilito
      TIFR1 = (1<<OCF1A);      
      TCNT1  = 0;                //Inicializa el contador del Timer en 0, (esperara 1 segundo entero al reanudar)
      // Voy a forzar un 'pause' por el cambio realizado en la App para que en una resincronizacion se pause el programa
      TIMSK1 &= ~(1 << OCIE1A);   //Inhabilita interrupción por comparación  
      
      mode = strtok(NULL,";"); // this continues where the previous call left off

      String _appInstanciaRutina;
      _appInstanciaRutina = strtok(NULL,";");
      Serial.println(_appInstanciaRutina);
      if (_appInstanciaRutina == "work"){
        instanciaRutina = TRABAJO;
      } else if (_appInstanciaRutina == "rest"){
        instanciaRutina = DESCANSO;
        Serial.println("Entro Rest");
      } else if (_appInstanciaRutina == "restBtwnSets"){
        instanciaRutina = DESCANSO_SETS;
      } else if (_appInstanciaRutina == "init"){
        instanciaRutina = INIT;
      } else if (_appInstanciaRutina == "NONE"){
        instanciaRutina = NONE;
      } else if (_appInstanciaRutina == "finished" || _appInstanciaRutina == "restarted"){
        instanciaRutina = FINALIZADA;
      }
      Serial.print("Modo: ");Serial.print(mode); Serial.print(" - Instancia: ");Serial.println(instanciaRutina);

      strtokIndx  = strtok(NULL,";");
      duracionActividad = atoi(strtokIndx);
      
      strtokIndx = strtok(NULL, ";"); 
      duracionDescanso = atoi(strtokIndx);
      
      strtokIndx = strtok(NULL, ";"); 
      duracionDescansoSets = atoi(strtokIndx);
      
      strtokIndx = strtok(NULL, ";");
      totalRounds = atoi(strtokIndx);
      
      strtokIndx = strtok(NULL, ";");
      totalSets = atoi(strtokIndx);

      strtokIndx = strtok(NULL, ";");
      actualRound = atoi(strtokIndx);

      strtokIndx = strtok(NULL, ";");
      actualSet = atoi(strtokIndx);

      strtokIndx = strtok(NULL, ";");
      int _timeLeft = atoi(strtokIndx); //en realidad recibo 'timeLeft'
      cleanDisplay7(0);
      if (instanciaRutina == TRABAJO){
        segundos = duracionActividad - _timeLeft;
        cuentaDisplay(duracionActividad - segundos);         
      } else if (instanciaRutina == DESCANSO){
        segundos = duracionDescanso - _timeLeft;
        cuentaDisplay(duracionDescanso - segundos);  
      } else if (instanciaRutina == DESCANSO_SETS){
        segundos = duracionDescansoSets - _timeLeft;
        cuentaDisplay(duracionDescansoSets - segundos);  
      } else if (instanciaRutina == INIT){
        segundos = duracionInit - _timeLeft;
        cuentaDisplay(duracionInit - segundos); //cuentaDisplay(_timeLeft); //Equiv, pero lo dejo asi porque aca lo tenia hecho con 'segundos', para no cambiarlo  
      } else{
        segundos = 0;
      }
    }break;
  }
  cG_bufferRx[0] = '\0';
  cG_nuevaTrama = false;
}

