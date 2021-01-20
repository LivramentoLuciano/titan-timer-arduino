#include <Arduino.h>
#include "definiciones.h"
#include <TimerOne.h>
//#include "routineModel.h"


//extern Routine routine; //ver si habria que ponerle SI o SI 'volatile' porque si lo hago, me tira error con los 'Strings' de 'routine.mode'

void initTrainingTimer (void){
  //configuro entradas y salidas
  for (int i = 0; i < 8; i++){
    pinMode(display7[0][i],OUTPUT);
  }
  pinMode(pinBuzzer, OUTPUT);

  /*Serial.println("Routine Default: " + String(routine.mode) + ", " + String(routine.workTime)+ ", " +  String(routine.restTime) + "...");
  routine.setRoutine(50,10,300,6,4,3,"tabata","init");
  Serial.println("Supongo recibida START (modifica la rutina, segun lo que recibo del celu)");
  Serial.println("Routine recibida: " + String(routine.mode) + ", " + String(routine.workTime)+ ", " +  String(routine.restTime) + "...");*/
  
  //Configuro Timer1, para operar en Modo: CTC, con 1Hz de frecuencia de interrupcion
  TCCR1A = 0;                //El registro de control A queda todo en 0
  TCCR1B = 0;                //Limpia el Registro de control B
  TCNT1  = 0;                //Inicializa el contador del Timer en 0
  OCR1A = 0x3D09;            //Carga el registro de comparación, de manera de obtener una interrupcion cada 1 seg (1Hz): 16MHz/1024/1Hz=15625=0x3D09
  TCCR1B |= (1 << WGM12)|(1<<CS10)|(1 << CS12);  //Establece modo CTC, prescaler 1024 
  TIMSK1 &= ~(1 << OCIE1A);   //De arranque inhabilita interrupción por comparación  
}

ISR(TIMER1_COMPA_vect)          // interrupción por igualdad de comparación en TIMER1
{
  segundos++;
  pasoSegundo = 1;  
}

void roundIncrement(){
  if(actualRound < totalRounds){
    // Primero pongo a 0 el Timer, para que no dispare instantaneamente, sino que cambie de Round y espere 1 segundo completo para seguir
    // Limpio el flag para evitar que dispare interrupcion apenas habilito
    TIFR1 = (1<<OCF1A);
    TCNT1  = 0;
    //estaba mal, xq si estaba en pause, lo habilitaba de vuelta y arrancaba, pero en la app no esta hecho de esa forma (sigue en pause)
    //ponerse de acuerdo y hacerlo igual en los 2 lados, o hacer que la app al mandar 'roundIn/Decrement' tmb haga un 'pause' por ej
    //tmb ver de cancelar el timer en la app al enviar esto (xq sino a veces, se incrementa el round e instantaneamente dispara un cambio de timeLeft)
    // de ultima, dejarlo asi en la app y sacar aca las 2 lineas anteriores de TIFR1 y TCNT1, pero creo que lo ideal es q las 2 esperen 1 segundo entero al cambiarse de round
    //TIMSK1 |= (1 << OCIE1A);   //Habilita interrupción por comparación (por si el 'roundIncrement' estando en pause)
    actualRound++;
    instanciaRutina = TRABAJO;
    segundos = 0; // Equiv timeLeft = workTime;
    cleanDisplay7(0);
    cuentaDisplay(duracionActividad - segundos);
  }else{
    if (actualSet < totalSets){
      // Primero pongo a 0 el Timer, para que no dispare instantaneamente, sino que cambie de Round y espere 1 segundo completo para seguir
      // Limpio el flag para evitar que dispare interrupcion apenas habilito
      TIFR1 = (1<<OCF1A);
      TCNT1  = 0;
      //TIMSK1 |= (1 << OCIE1A);   //Habilita interrupción por comparación (por si el 'roundIncrement' estando en pause)      
      actualRound = 1;
      actualSet++;
      instanciaRutina = TRABAJO;
      segundos =0; // Equiv timeLeft = workTime;
      cleanDisplay7(0);
      cuentaDisplay(duracionActividad - segundos);
    } 
    //En el ultimo round del ultimo set, 'roundIncrement' no tiene efecto
  }
}

void roundDecrement (){
  segundos = 0; // Equiv timeLeft = workTime;
  instanciaRutina = TRABAJO;
  if (actualRound > 1){
    // Primero pongo a 0 el Timer, para que no dispare instantaneamente, sino que cambie de Round y espere 1 segundo completo para seguir
    // Limpio el flag para evitar que dispare interrupcion apenas habilito
    TIFR1 = (1<<OCF1A);
    TCNT1  = 0;
    //TIMSK1 |= (1 << OCIE1A);   //Habilita interrupción por comparación (por si el 'roundDecrement' estando en pause)    
    actualRound--;
    cleanDisplay7(0);
    cuentaDisplay(duracionActividad - segundos);
  }else{
    if (actualSet > 1){
      TIFR1 = (1<<OCF1A);
      TCNT1  = 0;
      //TIMSK1 |= (1 << OCIE1A);   //Habilita interrupción por comparación (por si el 'roundDecrement' estando en pause)      
      actualRound = totalRounds;
      actualSet--;
      cleanDisplay7(0);
      cuentaDisplay(duracionActividad - segundos);
    }else{}
  }
}

void setIncrement(){
  if (actualSet < totalSets){
    // Primero pongo a 0 el Timer, para que no dispare instantaneamente, sino que cambie de Set y espere 1 segundo completo para seguir
    // Limpio el flag para evitar que dispare interrupcion apenas habilito
    TIFR1 = (1<<OCF1A);
    TCNT1  = 0;
    //TIMSK1 |= (1 << OCIE1A);   //Habilita interrupción por comparación (por si el 'setIncrement' estando en pause)    
    actualSet++;
    actualRound = 1;
    segundos = 0; // Equiv timeLeft = workTime;
    instanciaRutina = TRABAJO;
    cleanDisplay7(0);
    cuentaDisplay(duracionActividad - segundos);
  }else{}  
}

void setDecrement(){
  segundos = 0; // Equiv timeLeft = workTime;
  instanciaRutina = TRABAJO;
  if(actualSet > 1){
    // Primero pongo a 0 el Timer, para que no dispare instantaneamente, sino que cambie de Set y espere 1 segundo completo para seguir
    // Limpio el flag para evitar que dispare interrupcion apenas habilito
    TIFR1 = (1<<OCF1A);
    TCNT1  = 0;
    //TIMSK1 |= (1 << OCIE1A);   //Habilita interrupción por comparación (por si el 'setDecrement' estando en pause)    
    actualSet--;
    actualRound = 1;
    cleanDisplay7(0);
    cuentaDisplay(duracionActividad - segundos);
  }else{}  
}

void secondsForward (int seconds){
  //TIMSK1 &= ~(1 << OCIE1A);   //Inhabilita interrupción por comparación
  TIFR1 = (1<<OCF1A);        //Limpio el flag para evitar que dispare interrupcion apenas habilito, no se xq es con '1' en vez de '0' que se limpia,porque en el manual dice otra cosa //TIFR1 &= ~(1<<OCF1A);TIFR1 = (1<<OCF1A); //TIFR1 &= ~(1<<OCF1A);      //Limpio el flag para evitar que dispare interrupcion apenas habilito
  TCNT1  = 0;                //Inicializa el contador del Timer en 0
  int _duracion; //esto en realidad convendira tenerlo en el main (resolver como 'timeLeft' de la App
  
  if (instanciaRutina == TRABAJO) _duracion = duracionActividad;
  else if (instanciaRutina == DESCANSO) _duracion = duracionDescanso;
  else if(instanciaRutina == DESCANSO_SETS) _duracion = duracionDescansoSets;
  
  if (_duracion - segundos > seconds){
    segundos += seconds;
    cleanDisplay7(0);
    cuentaDisplay(_duracion - segundos); 
  }else{
    //Por ahora solo chequeo que incrementar esos 'seconds' no sobrepase la actividad, 
    //en caso de sobrepasarla, omite el incremento
    //en un futuro, poner que pase al siguiente 'state' (supongamos, pasar de work a rest) como en el caso de roundIncrement, por ejemplo, que pasa al siguiente SET si estaba en el ultimo round
    /*
     segundos = 0;
     if (instanciaRutina == DESCANSO || instanciaRutina == DESCANSO_SETS) instanciaRutina = TRABAJO;
     else if (instanciaRutina == TRABAJO){
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
     */
  }

  //TIMSK1 |= (1 << OCIE1A);   //Habilita interrupción por comparación 
}

void secondsBackward (int seconds){
  //TIMSK1 &= ~(1 << OCIE1A);   //Inhabilita interrupción por comparación
  TIFR1 = (1<<OCF1A);        //Limpio el flag para evitar que dispare interrupcion apenas habilito, no se xq es con '1' en vez de '0' que se limpia,porque en el manual dice otra cosa //TIFR1 &= ~(1<<OCF1A);TIFR1 = (1<<OCF1A); //TIFR1 &= ~(1<<OCF1A);      //Limpio el flag para evitar que dispare interrupcion apenas habilito
  TCNT1  = 0;                //Inicializa el contador del Timer en 0
  int _duracion; //esto en realidad convendira tenerlo en el main (resolver como 'timeLeft' de la App
  
  if (instanciaRutina == TRABAJO) _duracion = duracionActividad;
  else if (instanciaRutina == DESCANSO) _duracion = duracionDescanso;
  else if(instanciaRutina == DESCANSO_SETS) _duracion = duracionDescansoSets;

  if (segundos > seconds){
    segundos -= seconds;
    cleanDisplay7(0);
    cuentaDisplay(_duracion - segundos); 
  }else{
    //Por ahora solo chequeo que los segundos actuales sean mayor a lo que quiero restar, 
    //en caso caso contrario, omite el decremento
    //en un futuro, poner que pase al anterior 'state' (supongamos, pasar de rest a work) como en el caso de roundDecrement, por ejemplo, que pasa al anterior SET si estaba en el primer round
  }

  //TIMSK1 |= (1 << OCIE1A);   //Habilita interrupción por comparación 
}
