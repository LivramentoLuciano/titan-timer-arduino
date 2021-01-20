#include "definiciones.h"
#include <TimerOne.h>

char instanciaRutina = FINALIZADA;
volatile int segundos = 0;
volatile int pasoSegundo = 0;
long ultimoTiempo = 0;
long inicioAlarma = 0;
char rounds = 1;
char duracionActividad = 20;
char duracionDescanso = 5;
char nRounds = 1;

int reproducirAlarma = 0;

void setup() {
  initTrainingTimer(); //inicializa entradas, salidas, timer
  Serial.begin(9600); 
}


void loop() {
  String dato;
  
  if (Serial.available()){
    dato = Serial.readString();
  }
  
  if (dato == "iniciar"){
    dato = "";
    instanciaRutina = 1;
    Serial.println("Inicia Actividad fisica"); 
    //ultimoTiempo = millis(); 
    cuentaDisplay(duracionActividad - segundos);
    Timer1.start();
  }

  if (instanciaRutina == TRABAJO){
    Serial.println("Conteo regresivo - Tiempo Actividad: " + String(duracionActividad - segundos));  
    //if ( millis() - ultimoTiempo > 1000 ){
    if (pasoSegundo){
      //segundos++;
      //ultimoTiempo = millis();
      pasoSegundo = 0;
      cleanDisplay7(0);
      cuentaDisplay(duracionActividad - segundos);   
      if (segundos == duracionActividad){
        instanciaRutina = DESCANSO;
        Serial.println("Comienza descanso");
        segundos = 0;
      }  
    }
    if (segundos == duracionActividad - T_ALARMA_TRABAJO){  
      if ( millis()> inicioAlarma + 2*INTERVALO_BEEP){
        inicioAlarma = millis();
        tone(pinBuzzer, 1000, INTERVALO_BEEP);
      }   
    }else if (segundos == duracionActividad - 2){
      tone(pinBuzzer, 1000, 1000);
    }         
  }

  if (instanciaRutina == DESCANSO){
    Serial.println("Conteo regresivo - Tiempo Descanso: " + String(duracionDescanso - segundos)); 
    //if ( millis() - ultimoTiempo > 1000 ){
    if (pasoSegundo){
      pasoSegundo = 0;
      //segundos++;
      //ultimoTiempo = millis();
      cleanDisplay7(0);
      cuentaDisplay(duracionDescanso - segundos);            
    }    
    
    if ( segundos == duracionDescanso){
      instanciaRutina = FINALIZADA;
      segundos = 0;
      if (rounds == nRounds){ //termina por completo la actividad
        
      }else{ //repetir hasta alcanzar la cantidad de Rounds establecidos        
        rounds++;
        instanciaRutina = TRABAJO;
      }
    }
    
    if (segundos == duracionDescanso - T_ALARMA_DESCANSO){  
      if ( millis()> inicioAlarma + 2*INTERVALO_BEEP){
        inicioAlarma = millis();
        tone(pinBuzzer, 1000, INTERVALO_BEEP);
      }   
    }    
  }

  if (instanciaRutina == FINALIZADA){
    cleanDisplay7(0);       
  }

  //Serial.println("Segundos: " + String(segundos));delay(250);
}
