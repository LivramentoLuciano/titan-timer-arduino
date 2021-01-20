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
  int unidSeg, decSeg;

  unidSeg = cuenta %10;
  decSeg = (cuenta %10) / 10;

  imprimirNum(unidSeg, 0); 
}

