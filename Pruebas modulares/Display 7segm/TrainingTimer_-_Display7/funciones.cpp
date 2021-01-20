#include <Arduino.h>
#include "definiciones.h"

void initTrainingTimer (void){
  //configuro entradas y salidas
  for (int i = 0; i < 8; i++){
    pinMode(display7_1[i],OUTPUT);
  }
}

