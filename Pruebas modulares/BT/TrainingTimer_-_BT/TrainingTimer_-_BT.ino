#include "definiciones.h"
#include <SoftwareSerial.h>

SoftwareSerial BT(10, 11); // RX, TX puertos emulados, lo emulo para poder debug

char instanciaRutina = 0;
char segundos = 0;
char duracionActividad = 20;
char duracionDescanso = 5;
char rounds = 1; 
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
    Serial.println("Dato recibido: " + dato);
  }

  if (dato == "iniciar"){
    dato = "";
    instanciaRutina = 1;
    Serial.println("Inicia Actividad fisica");  
  }

  if (instanciaRutina == 1){
    Serial.println("Conteo regresivo - Tiempo Actividad: " + String(duracionActividad - segundos));
    if (segundos == duracionActividad){
      instanciaRutina = 2;
      Serial.println("Comienza descanso");
      segundos = 0;
    }
  }

  if (instanciaRutina == 2){
    Serial.println("Conteo regresivo - Tiempo Descanso: " + String(duracionDescanso - segundos));
    if ( segundos == duracionDescanso){
      instanciaRutina = 0;
      segundos = 0;
      if (rounds == nRounds){
        //termina por completo la actividad
      }else{
        //repetir hasta alcanzar la cantidad de Rounds establecidos        
        rounds++;
        instanciaRutina = 1;
      }
    }
  }
  delay(1000);
  if(instanciaRutina != 0)segundos++;
  
}
