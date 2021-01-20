#include "definiciones.h"

void setup() {
  initTrainingTimer();
  Serial.begin(9600);
}

void loop() {
  funcionA();
  delay(2000);
}


void funcionA (void){
  for ( int j= 2; j<11 ; j++)   // Va encendiendo los segmentos
  {
    digitalWrite(j, HIGH);
    delay(400);
  }
  for ( int j= 2; j<11 ; j++)   //Apaga todos los segmentos
  {
    digitalWrite(j, LOW);
    delay(400) ;  
  }
}
