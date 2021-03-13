#include "Routine.h"

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

// Combate/Amrap -> Faltando 10 segundos, 1 solo seg (beeps cortos)
// Resto -> Durante los ultimos 3 segundos
bool Routine:: lastSeconds()
{
  if (instance == INIT) return get_tLeft() <= LAST_SECONDS_NORMAL;
  if (mode == COMBATE || mode == AMRAP) return get_tLeft() == LAST_SECONDS_XL;
  else if (mode == HIIT || mode == TABATA) return get_tLeft() <= LAST_SECONDS_NORMAL;
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
  if (instance == WORK){
    if (mode == COMBATE) return "fight";
    else return "work";
  }
  else if (instance == REST){
    if (mode == COMBATE || mode == AMRAP) return "rest";
    else return "prep"; // hiit/tabata descansos de transicion
  } 
  else if (instance == REST_SET) return "rest";
  else if (instance == NOTHING) return "";
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

AlarmMode Routine:: get_alarmMode(){
  if (mode == AMRAP || mode == COMBATE) return ALARM_SHORT;
  else return ALARM_NORMAL;
}