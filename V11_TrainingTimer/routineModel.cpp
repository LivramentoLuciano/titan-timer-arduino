#include <Arduino.h>
#include "routineModel.h"

Routine:: Routine(){
  //Para la inicializacion (creacion) del objeto routine, pongo valores por default,
  //Solo para tener algo, creo que es al recontra pedo, porque nunca lo voy a usar esto, pero fue para safar de poder crear el objeto inicialmente 
  //y referenciarlo desde distintas partes del programa
  //Al recibir una rutina, usare 'setRoutine',
  workTime = 900;
  restTime = 180;
  restBtwnSetsTime = 0;
  totalRounds = 2;
  totalSets = 1;
  timeLeft = 3;
  mode = "amrap";
  state = "init";
  actualRound = 1;
  actualSet = 1;
}

void Routine:: setRoutine(int wTime, int rTime, int rbtwnTime, int tRounds, int tSets, int time_left, String rMode, String rState){
    workTime = wTime;
    restTime = rTime;
    restBtwnSetsTime = rbtwnTime;
    totalRounds = tRounds;
    totalSets = tSets;
    timeLeft = time_left;
    mode = rMode;
    state = rState;
    actualRound = 1;
    actualSet = 1;
}

void Routine::roundIncrement(){
  if(actualRound < totalRounds){
    actualRound++;
    timeLeft = workTime;
    state = "work";
  }else {
    if (actualSet < totalSets){
      actualRound = 1;
      actualSet++;
      timeLeft = workTime;
      state = "work";
    } 
    //En el ultimo round del ultimo set, 'roundIncrement' no tiene efecto
  }
}

void Routine::roundDecrement(){
  timeLeft = workTime;
  state = "work";
  if (actualRound > 1){
    actualRound--;
  }else{
    if (actualSet > 1){
      actualRound = totalRounds;
      actualSet--;
    }else{}
  }
}

void Routine::setIncrement(){
  if (actualSet < totalSets){
    actualSet++;
    actualRound = 1;
    timeLeft = workTime;
    state = "work";
  }else{}
}

void Routine::setDecrement(){
  timeLeft = workTime;
  state = "work";
  if(actualSet > 1){
    actualSet--;
    actualRound = 1;
  }else{}
}
