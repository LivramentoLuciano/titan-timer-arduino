#include "definiciones.h"
#include <TimerOne.h>

char instanciaRutina = FINALIZADA;
volatile int segundos = 0;
volatile int pasoSegundo = 0;
long inicioAlarma = 0;
char rounds = 1;
char duracionActividad = 10;
char duracionDescanso = 8;
char nRounds = 2;

int reproducirAlarma = 0;
char alarmaON = 1;

void setup() {
  initTrainingTimer(); //inicializa entradas, salidas, timer
  Serial.begin(9600); 
}


void loop() {
  String datoSerie;
  
  if (Serial.available()){
    datoSerie = Serial.readString();
  }
  
  if (datoSerie == "iniciar"){
    instanciaRutina = 1;
    //Serial.println("Inicia Actividad fisica"); 
    cuentaDisplay(duracionActividad - segundos);
    Timer1.start();
    alarmaON = 1;
  }else if (datoSerie == "pausa"){
    Timer1.stop();
    alarmaON = 0; //desactivo alarma, caso de pausar justo en ese momento //ver si armar estados distintos (instanciaRutina = PAUSA)
    }else if (datoSerie == "reset"){
      segundos = 0;
      Timer1.restart();
      Timer1.stop();
    }else if (datoSerie == "reanudar"){
      Timer1.start();
      alarmaON = 1;
    }
 
  datoSerie = "";

  if (instanciaRutina == TRABAJO){
    if (pasoSegundo){
      pasoSegundo = 0;
      Serial.println("Conteo regresivo - Tiempo Actividad: " + String(duracionActividad - segundos));  
      cleanDisplay7(0);
      cuentaDisplay(duracionActividad - segundos);   
      if (segundos == duracionActividad){
        instanciaRutina = DESCANSO;
        //Serial.println("Comienza descanso");
        segundos = 0;
      }  
    }
    if ((segundos == duracionActividad - T_ALARMA_TRABAJO) && alarmaON){  
      if ( millis()> inicioAlarma + 2*INTERVALO_BEEP){
        inicioAlarma = millis();
        tone(pinBuzzer, 1000, INTERVALO_BEEP);
      }   
    }else if (segundos == duracionActividad - 2 && alarmaON){
      tone(pinBuzzer, 1000, 1000);
    }         
  }

  if (instanciaRutina == DESCANSO){
    if (pasoSegundo){
      pasoSegundo = 0;
      Serial.println("Conteo regresivo - Tiempo Descanso: " + String(duracionDescanso - segundos)); 
      cleanDisplay7(0);
      cuentaDisplay(duracionDescanso - segundos);            
    }    
    
    if ( segundos == duracionDescanso){
      segundos = 0;
      if (rounds == nRounds){ //termina por completo la actividad
        instanciaRutina = FINALIZADA;        
      }else{ //repetir hasta alcanzar la cantidad de Rounds establecidos        
        rounds++;
        instanciaRutina = TRABAJO;
      }
    }else if ((segundos == duracionDescanso - T_ALARMA_DESCANSO) && alarmaON){
      if ( millis()> inicioAlarma + 2*INTERVALO_BEEP){
        inicioAlarma = millis();
        tone(pinBuzzer, 1000, INTERVALO_BEEP);
      }   
    }    
  }

  if (instanciaRutina == FINALIZADA){
    Serial.println("Entro Finalizada");
    cleanDisplay7(0);
    rounds = 1;   
    Timer1.restart();
    Timer1.stop();    
  }
}
