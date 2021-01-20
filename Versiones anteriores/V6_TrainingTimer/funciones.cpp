#include <Arduino.h>
#include "definiciones.h"
#include <TimerOne.h>

extern volatile int segundos;
extern volatile int pasoSegundo;

void initTrainingTimer (void){
  //configuro entradas y salidas
  for (int i = 0; i < 8; i++){
    pinMode(display7[0][i],OUTPUT);
  }
  pinMode(pinBuzzer, OUTPUT);

  //Configuracion Timer
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(interrupcion_1seg); 
  Timer1.stop();  
}

void interrupcion_1seg()
{
  //Serial.println("Entro Timer");
  segundos++;
  pasoSegundo = 1;
}

/*
 * Impresion de caracteres numericos en Display 7 segmentos
 * Parametros:
 * @numero: el numero a imprimir (de 0 a 9)
 * @id_display: indica en que display se imprimira el numero
 * 
 * Devuelve:
 * @Nada
 */

void imprimirNum (int numero, int id_display){
  //dibuja el caracter pasado (numerico: 0-9)
  byte mask = caracterNumerico[numero];
  bool a,b,c,d,e,f,g,p;
  char aux = 0b00000001;

  a = mask & aux;  mask = mask >> 1;
  b = mask & aux;  mask = mask >> 1;
  c = mask & aux;  mask = mask >> 1;
  d = mask & aux;  mask = mask >> 1; 
  e = mask & aux;  mask = mask >> 1;
  f = mask & aux;  mask = mask >> 1;
  g = mask & aux;  mask = mask >> 1;
  p = mask & aux;

  if (a) digitalWrite(display7[id_display][0],HIGH);
  if (b) digitalWrite(display7[id_display][1],HIGH);
  if (c) digitalWrite(display7[id_display][2],HIGH);
  if (d) digitalWrite(display7[id_display][3],HIGH);  
  if (e) digitalWrite(display7[id_display][4],HIGH);
  if (f) digitalWrite(display7[id_display][5],HIGH);
  if (g) digitalWrite(display7[id_display][6],HIGH);
  if (p) digitalWrite(display7[id_display][7],HIGH); 
}

void cleanDisplay7 (int numDisplay7){
  for (int i = 0; i < 8; i++){
    digitalWrite(display7[numDisplay7][i],LOW);
  }
}

void cuentaDisplay ( int cuenta){
  //voy a tener varios digitos, por ahora uso solo el de "unidades" (unid, decenas, centenas.. o  unidSeg,decSeg, unidMin)
  //Necesito dividir ej: 19 (int) en 2 caracteres '1' y '9'
  //Cuenta "segundos", pero "display" en minutos, por tanto debo hacer el pasaje
  int unidSeg, decSeg, unidMin, decMin;
  int cuentaSegs, cuentaMins;
  
  //Division 'int' se queda con el numero entero, "minutos" -- '%' se queda con el resto de la division, "Segundos"
  cuentaMins = cuenta / 60; 
  cuentaSegs = cuenta % 60; 
  
  //Separo en digitos los minutos y segundos
  unidSeg = cuentaSegs %10;
  decSeg = (cuentaSegs %10) / 10;
  unidMin = cuentaMins%10;
  decMin = (cuentaMins %10) / 10;  

  //imprimo cada digito de los minutos y segundos en el display correspondiente
  imprimirNum(unidSeg, 0); 
  //imprimirNum(decSeg, 1);
  //imprimirNum(unidMin, 2);
  //imprimirNum(decMin, 3);
}


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
    case HDR_TABATA_TxC: //'T' Header para rutina de Tabata-TxC de Entrenamiento Funcional
    { 
      *ptr_instanciaRutina = TRABAJO;
      *ptr_duracionActividad = String(subStr(copia_trama,";",2)).toInt();
      *ptr_duracionDescanso = String(subStr(copia_trama,";",3)).toInt();
      *ptr_totalRounds= String(subStr(copia_trama,";",4)).toInt();
      *ptr_totalSets = String(subStr(copia_trama,";",5)).toInt();
      *ptr_duracionDescansoSets = String(subStr(copia_trama,";",6)).toInt();
      //Serial.print("{R;T;OK}");
      cuentaDisplay(*ptr_duracionActividad - segundos);
      Timer1.restart();
      Timer1.start();
      Serial.println("");
      Serial.println("Iniciar tabata.");
      Serial.println("Act: " + String(*ptr_duracionActividad));
      Serial.println("Desc: " + String(*ptr_duracionDescanso));
      Serial.println("Rounds: " + String(*ptr_totalRounds));
      Serial.println("Sets: " + String(*ptr_totalSets));
      Serial.println("Descanso Sets: " + String(*ptr_duracionDescansoSets));
    }
    break;

    case HDR_AMRAP: //'A' recibe desde el celu, si el buzzer tiene que sonar o no, a la hora de tomar un medicamento (ver si tiene sentido, por dos cosas: 1-Ver compatibilidad con la rutina de leds, para que ambos funcionen bien al mismo tiempo 2-Ver si tiene sentido, por tema de buscador que tmb podria sonar al mismo tiempo (ver si cambiar el sonido, y que si esta habilitado, que el buscador no funcione)
    { 
      *ptr_instanciaRutina = TRABAJO;
      //AMRAP -> No tiene 'sets', serian X rounds de Y minutos con un descanso entre rounds de Z minutos
      /*
      char * strtokIndx; // this is used by strtok() as an index
      *ptr_duracionActividad = atoi(strtok(copia_dato,";"));    // get the first part - the string   
      *ptr_totalRounds = atoi(strtok(NULL, ";"));
      *ptr_duracionDescanso = atoi(strtok(NULL, ";")); 
      *ptr_totalSets = 1;
      */
      
      *ptr_duracionActividad = String(subStr(copia_trama,";",2)).toInt();
      *ptr_duracionDescanso = String(subStr(copia_trama,";",3)).toInt();
      *ptr_totalRounds= String(subStr(copia_trama,";",4)).toInt();
      *ptr_totalSets = 1;
            
      cuentaDisplay(*ptr_duracionActividad - segundos);
      Timer1.restart();
      Timer1.start();
      *ptr_alarmaON = true; 
       
      /*Serial.println("");
      Serial.println("Iniciar AMRAP.");
      Serial.println("Act: " + String(*ptr_duracionActividad));
      Serial.println("Desc: " + String(*ptr_duracionDescanso));
      Serial.println("Rounds: " + String(*ptr_totalRounds));
      Serial.println("Sets: " + String(*ptr_totalSets)); 
      */
    }
    break;

    case HDR_COMBATE: //'C' Inicia Rutina de "Combate"
    { //similar AMRAP
      *ptr_instanciaRutina = TRABAJO;
      /*
      char * strtokIndx; // this is used by strtok() as an index
      *ptr_duracionActividad = atoi(strtok(copia_dato,";"));    // get the first part - the string   
      *ptr_totalRounds = atoi(strtok(NULL, ";"));
      *ptr_duracionDescanso= atoi(strtok(NULL, ";")); 
      *ptr_totalSets = 1;
      */
      
      *ptr_duracionActividad = String(subStr(copia_trama,";",2)).toInt();
      *ptr_duracionDescanso = String(subStr(copia_trama,";",3)).toInt();
      *ptr_totalRounds= String(subStr(copia_trama,";",4)).toInt();
      *ptr_totalSets = 1;  

      cuentaDisplay(*ptr_duracionActividad - segundos);
      Timer1.restart();
      Timer1.start();
      *ptr_alarmaON = true;  
      
      /*Serial.println("");
      Serial.println("Iniciar Combate.");
      Serial.println("Act: " + String(*ptr_duracionActividad));
      Serial.println("Desc: " + String(*ptr_duracionDescanso));
      Serial.println("Rounds: " + String(*ptr_totalRounds));
      Serial.println("Sets: " + String(*ptr_totalSets)); 
      */           
    }
    break;   

    case HDR_PAUSA:
    {
      Timer1.stop();
      *ptr_alarmaON = false; //desactivo alarma, caso de pausar justo en ese momento //ver si armar estados distintos (instanciaRutina = PAUSA)
      //*ptr_instanciaRutina = 
    }break;
    
    case HDR_REANUDAR:
    {
      Timer1.start();
      *ptr_alarmaON = true;      
      //*ptr_instanciaRutina = 
    }break;
       
    case HDR_RESET:
    {
      //*ptr_segundos = 0;
      Timer1.restart();
      Timer1.stop();
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

