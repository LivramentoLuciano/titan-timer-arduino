#include <Arduino.h>
#include "definiciones.h"
#include <TimerOne.h>

extern volatile int segundos;
extern volatile int pasoSegundo;

//int t = 0;

void initTrainingTimer (void){
  //configuro entradas y salidas
  for (int i = 0; i < 8; i++){
    pinMode(display7[0][i],OUTPUT);
  }
  pinMode(pinBuzzer, OUTPUT);

  Serial.begin(9600);

  //Configuracion Timer
  /*Timer1.initialize(1000000);
  Timer1.attachInterrupt(interrupcion_1seg); 
  Timer1.restart();
  Timer1.stop();*/  
  //Configuro Timer1, para operar en Modo: CTC, con 1Hz de frecuencia de interrupcion
  noInterrupts();
  TCCR1A = 0;                //El registro de control A queda todo en 0
  TCCR1B = 0;                //Limpia el Registro de control B
  TCNT1  = 0;                //Inicializa el contador del Timer en 0
  OCR1A = 0x3D09;            //Carga el registro de comparación, de manera de obtener una interrupcion cada 1 seg (1Hz): 16MHz/1024/1Hz=15625=0x3D09
  TCCR1B |= (1 << WGM12)|(1<<CS10)|(1 << CS12);  //Establece modo CTC, prescaler 1024 
  
  //TIFR1 &= ~(1<<OCF1A);      //Limpio el flag para evitar que dispare interrupcion apenas habilito
  //t = millis();
  //TIMSK1 |= (1 << OCIE1A);
  TIMSK1 &= ~(1 << OCIE1A);   //De arranque inhabilita interrupción por comparación  
  interrupts();
}

//void timer

ISR(TIMER1_COMPA_vect)          // interrupción por igualdad de comparación en TIMER1
{
  //Serial.print((millis() - t) / 1000);
  segundos++;
  pasoSegundo = 1;  
}

void interrupcion_1seg()
{
  Serial.println("Timer");
  segundos++;
  pasoSegundo = 1;
}

