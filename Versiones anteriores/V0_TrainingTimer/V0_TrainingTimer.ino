#include "definiciones.h"
#include <SoftwareSerial.h>

SoftwareSerial BT(10, 11); // RX, TX puertos emulados, lo emulo para poder debug

char instanciaRutina = 0;
char segundos = 0;
long ultimoTiempo = 0;
char rounds = 1;
char duracionActividad = 20;
char duracionDescanso = 5;
char nRounds = 1;

void setup() {
  initTrainingTimer(); //inicializa entradas, salidas
  Serial.begin(9600);
  BT.begin(9600);
}

void loop() {
  String dato;
  if (BT.available()){
    dato = BT.readString();
  }

  if (dato == "iniciar"){
    dato = "";
    instanciaRutina = 1;
    Serial.println("Inicia Actividad fisica"); 
    ultimoTiempo = millis(); 
  }

  if (instanciaRutina == 1){
    Serial.println("Conteo regresivo - Tiempo Actividad: " + String(duracionActividad - segundos));
    cuentaDisplay(duracionActividad - segundos);  
    delay(500);
    cleanDisplay7(1);  
    if ( millis() - ultimoTiempo > 1000 ){
      segundos++;
      ultimoTiempo = millis();  
    }
    if (segundos == duracionActividad){
      instanciaRutina = 2;
      Serial.println("Comienza descanso");
      segundos = 0;
      ultimoTiempo = millis();
    }      
  }

  if (instanciaRutina == 2){
    Serial.println("Conteo regresivo - Tiempo Descanso: " + String(duracionDescanso - segundos));
    cuentaDisplay(duracionDescanso - segundos);  
    delay(500);
    cleanDisplay7(1);  
    if ( millis() - ultimoTiempo > 1000 ){
      segundos++;
      ultimoTiempo = millis();   
    }    
    
    if ( segundos == duracionDescanso){
      instanciaRutina = 0;
      segundos = 0;
      if (rounds == nRounds){ //termina por completo la actividad
        
      }else{ //repetir hasta alcanzar la cantidad de Rounds establecidos        
        rounds++;
        instanciaRutina = 1;
      }
    }
  } 
}
