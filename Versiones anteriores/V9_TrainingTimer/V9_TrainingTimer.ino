#include "definiciones.h"
#include <TimerOne.h>
#include "routineModel.h"

String datoSerie;
char cG_bufferRx[100];
volatile char cG_nuevaTrama = false;

volatile int instanciaRutina = NONE;
volatile int actualRound = 1, actualSet = 1;
volatile int duracionActividad = 10, duracionDescanso = 8, totalRounds = 2,  totalSets = 3, duracionDescansoSets = 120;
char *mode;

volatile int segundos = 0, pasoSegundo = 0;
volatile long inicioAlarma = 0;
volatile int alarmaON = 1;

//Routine routine;

void setup() {
  initTrainingTimer(); //inicializa entradas, salidas, timer
  Serial.begin(9600);
}

void loop(){
  while(Serial.available()){
    filtro_trama(cG_bufferRx);
  }
  if(cG_nuevaTrama == true){
    procesar_trama();
  }

  switch (instanciaRutina){
    case INIT:{
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
    }break;
    
    case TRABAJO:{
      if (pasoSegundo){
        pasoSegundo = 0;
        //Serial.print("work");
        //Serial.print("Set: " + String(actualSet) + " - Round: " + String(actualRound)+ " - Tiempo Actividad: " + String(duracionActividad - segundos)+ "  ***  ");  
        cleanDisplay7(0);
        cuentaDisplay(duracionActividad - segundos);   
          if (segundos == duracionActividad){
            segundos = 0;
            cleanDisplay7(0);
            if (actualRound == totalRounds){
              if (actualSet == totalSets){
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
    }break;
    
    case DESCANSO:{
      if (pasoSegundo){
        pasoSegundo = 0;
        Serial.print("Set: " + String(actualSet) + " - Round: " + String(actualRound)+ " - Tiempo Descanso: " + String(duracionDescanso - segundos) + "  ***  "); 
        cleanDisplay7(0);
        cuentaDisplay(duracionDescanso - segundos);            
      }    
      
      if ( segundos == duracionDescanso){
        segundos = 0;
        //repetir hasta alcanzar la cantidad de Rounds establecidos        
        actualRound++;
        instanciaRutina = TRABAJO;
        cleanDisplay7(0);
        cuentaDisplay(duracionActividad - segundos);
      }else if ((segundos == duracionDescanso - T_ALARMA_DESCANSO) && alarmaON){
        if ( millis()> inicioAlarma + 2*INTERVALO_BEEP){
          inicioAlarma = millis();
          tone(pinBuzzer, 1000, INTERVALO_BEEP);
        }   
      }
    }break;
    
    case DESCANSO_SETS:{
      if (pasoSegundo){
        pasoSegundo = 0;
        Serial.print("Set: " + String(actualSet) + " - Round: " + String(actualRound)+ " - Tiempo Descanso Set: " + String(duracionDescansoSets - segundos) + "  ***  "); 
        cleanDisplay7(0);
        cuentaDisplay(duracionDescansoSets - segundos);            
      }      
      if (segundos == duracionDescansoSets){
        segundos = 0;
        actualSet++;
        actualRound = 1;
        instanciaRutina = TRABAJO;
        cleanDisplay7(0);
        cuentaDisplay(duracionActividad - segundos);        
        //Serial.println("");
      }
    }break;

    case FINALIZADA:{
      Serial.println("Finalizada");
      cleanDisplay7(0);
      actualRound = 1; 
      actualSet = 1;  
      segundos = 0;
      TCNT1  = 0;                //Inicializa el contador del Timer en 0
      TIMSK1 &= ~(1 << OCIE1A);   //Inhabilita interrupción por comparación     
      //Timer1.restart(); //no se porque me esta flahsando en el arranque si dejo este 'restart'
      //Timer1.stop(); 
    }break;

    default:{
      //No iniciada aun
      //Serial.print("NONE");delay(2000);
    }break;
  }
}
