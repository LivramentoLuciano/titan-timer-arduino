#include <Arduino.h>
#include "definiciones.h"

void initTrainingTimer (void){
  //configuro entradas y salidas
  for (int i = 0; i < 8; i++){
    pinMode(display7_1[i],OUTPUT);
  }
}



/*
 * Impresion de caracteres numericos en Display 7 segmentos
 * Parametros:
 * @numero: el numero a imprimir (de 0 a 9)
 * 
 * Devuelve:
 * @Nada
 */

void imprimirNum (int numero){
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

  if (a) digitalWrite(display7_1[0],HIGH);
  if (b) digitalWrite(display7_1[1],HIGH);
  if (c) digitalWrite(display7_1[2],HIGH);
  if (d) digitalWrite(display7_1[3],HIGH);  
  if (e) digitalWrite(display7_1[4],HIGH);
  if (f) digitalWrite(display7_1[5],HIGH);
  if (g) digitalWrite(display7_1[6],HIGH);
  if (p) digitalWrite(display7_1[7],HIGH); 
}

void cleanDisplay7 (int numDisplay7){
  for (int i = 0; i < 8; i++){
    digitalWrite(display7_1[i],LOW);
  }
}

void cuentaDisplay ( int cuenta){
  //voy a tener varios digitos, por ahora uso solo el de "unidades" (unid, decenas, centenas.. o  unidSeg,decSeg, unidMin)
  //Necesito dividir ej: 19 (int) en 2 caracteres '1' y '9'
  int unidSeg, decSeg;

  unidSeg = cuenta %10;
  decSeg = (cuenta %10) / 10;

  imprimirNum(unidSeg); 
}

