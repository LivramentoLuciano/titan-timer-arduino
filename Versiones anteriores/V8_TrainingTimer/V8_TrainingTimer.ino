#include "definiciones.h"
#include <TimerOne.h>

String datoSerie;
char cG_bufferRx[100];
char cG_nuevaTrama = false;
char *ptr_nuevaTrama = &cG_nuevaTrama;

int instanciaRutina = FINALIZADA;
int *ptr_instanciaRutina = &instanciaRutina;
volatile int segundos = 0, pasoSegundo = 0;
volatile int *ptr_segundos = &segundos;
long inicioAlarma = 0;

int alarmaON = 1;
int *ptr_alarmaON = &alarmaON;

int rounds = 1, sets = 1;
int duracionActividad = 10, duracionDescanso = 8, totalRounds = 2,  totalSets = 3, duracionDescansoSets = 120;
int *ptr_duracionActividad = &duracionActividad;
int *ptr_duracionDescanso = &duracionDescanso;
int *ptr_totalRounds = &totalRounds;
int *ptr_totalSets = &totalSets;
int *ptr_duracionDescansoSets = &duracionDescansoSets;


void setup() {
  initTrainingTimer(); //inicializa entradas, salidas, timer
  //Serial.begin(9600);
  Serial.println("asd");
}

void loop() {
  while(Serial.available()){ 
    filtro_trama(cG_bufferRx,ptr_nuevaTrama);
  }
  if (cG_nuevaTrama == true){
    procesar_trama(cG_bufferRx, ptr_nuevaTrama, ptr_duracionActividad, ptr_duracionDescanso, ptr_totalRounds, ptr_totalSets, ptr_duracionDescansoSets, ptr_instanciaRutina, ptr_segundos, ptr_alarmaON);
  }else{
    //Serial.println("Recibio datos basura, NO TRAMA DE DATOS");
  }  

  if (instanciaRutina == TRABAJO){
    if (pasoSegundo){
      pasoSegundo = 0;
      Serial.print("Set: " + String(sets) + " - Round: " + String(rounds)+ " - Tiempo Actividad: " + String(duracionActividad - segundos)+ "  ***  ");  
      cleanDisplay7(0);
      cuentaDisplay(duracionActividad - segundos);   
      if (segundos == duracionActividad){
        segundos = 0;
        cleanDisplay7(0);
        if (rounds == totalRounds){
          if (sets == totalSets){
            instanciaRutina = FINALIZADA;           
          }else{
            instanciaRutina = DESCANSO_SETS;
            cuentaDisplay(duracionDescansoSets - segundos);           
            //Serial.println("");               
          }
        }else{
          instanciaRutina = DESCANSO;
          cuentaDisplay(duracionDescanso - segundos); 
        }        
      }  
    }
    if ((segundos == duracionActividad - T_ALARMA_TRABAJO_SHORT) && alarmaON){  
      if ( millis()> inicioAlarma + 2*INTERVALO_BEEP){
        inicioAlarma = millis();
        tone(pinBuzzer, 1000, INTERVALO_BEEP);
      }   
    }else if ((segundos == duracionActividad - 2) && alarmaON){
      //tone(pinBuzzer, 1000, 1000);
    }         
  }

  if (instanciaRutina == DESCANSO){
    if (pasoSegundo){
      pasoSegundo = 0;
      //Serial.print("Set: " + String(sets) + " - Round: " + String(rounds)+ " - Tiempo Descanso: " + String(duracionDescanso - segundos) + "  ***  "); 
      cleanDisplay7(0);
      cuentaDisplay(duracionDescanso - segundos);            
    }    
    
    if ( segundos == duracionDescanso){
      segundos = 0;
      //repetir hasta alcanzar la cantidad de Rounds establecidos        
      rounds++;
      instanciaRutina = TRABAJO;
      cleanDisplay7(0);
      cuentaDisplay(duracionActividad - segundos);
    }else if ((segundos == duracionDescanso - T_ALARMA_DESCANSO) && alarmaON){
      if ( millis()> inicioAlarma + 2*INTERVALO_BEEP){
        inicioAlarma = millis();
        tone(pinBuzzer, 1000, INTERVALO_BEEP);
      }   
    }    
  }

  if (instanciaRutina == DESCANSO_SETS){
    if (pasoSegundo){
      pasoSegundo = 0;
      //Serial.print("Set: " + String(sets) + " - Round: " + String(rounds)+ " - Tiempo Descanso Set: " + String(duracionDescansoSets - segundos) + "  ***  "); 
      cleanDisplay7(0);
      cuentaDisplay(duracionDescansoSets - segundos);            
    }      
    if (segundos == duracionDescansoSets){
      segundos = 0;
      sets++;
      rounds = 1;
      instanciaRutina = TRABAJO;
      cleanDisplay7(0);
      cuentaDisplay(duracionActividad - segundos);        
      //Serial.println("");
    }
  }

  if (instanciaRutina == FINALIZADA){
    cleanDisplay7(0);
    rounds = 1; 
    sets = 1;  
    segundos = 0;
    TCNT1  = 0;                //Inicializa el contador del Timer en 0
    TIMSK1 &= ~(1 << OCIE1A);   //Inhabilita interrupción por comparación     
    //Timer1.restart(); //no se porque me esta flahsando en el arranque si dejo este 'restart'
    //Timer1.stop();    
  }

  if (instanciaRutina == INIT){
    if (pasoSegundo){
      pasoSegundo = 0;
      //Serial.print("Set: " + String(sets) + " - Round: " + String(rounds)+ " - Tiempo Descanso Set: " + String(duracionDescansoSets - segundos) + "  ***  "); 
      cleanDisplay7(0);
      cuentaDisplay(duracionInit - segundos);            
    }
    if ( segundos == duracionInit){
      segundos = 0;
      instanciaRutina = TRABAJO;
      cleanDisplay7(0);
      cuentaDisplay(duracionActividad - segundos);        
      //Serial.println("");
    } 
  }
}
