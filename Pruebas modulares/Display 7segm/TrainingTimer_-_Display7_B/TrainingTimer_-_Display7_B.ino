#include "definiciones.h"

void setup() {
  initTrainingTimer();
  Serial.begin(9600);
}

void loop() {
  //funcionA();
  for ( int i = 0; i<10 ; i++){
    imprimirNum(i);
    delay(500);
    cleanDisplay7(1);
  }
}
