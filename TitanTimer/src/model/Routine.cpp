#include "Routine.h"
#include "./../Alarm/alarm.h"

extern Alarma alarmaLastSeconds;

// Separo la carga de rutina de el 'start'
void Routine:: set_settings(String _mode, int _tWork, int _tRest, int _tRestSets, char _rounds, char _sets){
  set_mode(_mode);
  tWork = _tWork;
  tRest = _tRest;
  tRestSets = _tRestSets;
  rounds = _rounds;
  sets = _sets;

  isLoaded = true;
}

void Routine::init()
{
  instance = INIT;
  t = 0;
  actualRound = 1;
  actualSet = 1;
}

int Routine::get_tLeft()
{
  if (instance == WORK)
    return tWork - t;
  else if (instance == REST)
    return tRest - t;
  else if (instance == REST_SET)
    return tRestSets - t;
  else if (instance == INIT)
    return tInit - t;
  else
    return 0;
}

bool Routine:: lastSeconds(){
  return get_tLeft() <= 3;
}

void Routine::nextInstance()
{
  t = 0;
  alarmaLastSeconds.off();
  
  if (instance == INIT)
    set_instance(WORK);
  else if (instance == WORK)
  {
    if (actualRound == rounds)
    {
      if (actualSet == sets)
        set_instance(FINISHED);
      else
        set_instance(REST_SET);
    }
    else
    {
      set_instance(REST);
    }
  }
  else if (instance == REST)
  {
    set_instance(WORK);
    roundUp();
  }
  else if (instance == REST_SET)
  {
    actualRound = 1;
    set_instance(WORK);
    setUp();
  }
}

char* Routine:: get_instanceString(){
  if (instance == WORK)
    return "work";
  else if (instance == REST)
    return "prep";
  else if (instance == REST_SET)
    return "rest";
  else if(instance == NOTHING)
    return "";
}

void Routine:: set_mode(String m){
  if (m == "amrap") mode = AMRAP;
  else if (m == "hiit") mode = HIIT;
  else if (m == "tabata") mode = TABATA;
  else if (m = "combate") mode = COMBATE;
}

bool Routine:: enabled() {
  return isLoaded && (instance != NOTHING) && (instance != FINISHED);
}