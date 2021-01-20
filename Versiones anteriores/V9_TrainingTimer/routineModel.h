#include <Arduino.h>

class Routine{
  public:
  int workTime, restTime, restBtwnSetsTime, totalRounds, totalSets;
  int actualRound, actualSet;
  int timeLeft;
  const int initialPrepareTime = 3, finishAlarmTime = 3;
  
  String mode; //amrap, tabata, combat... (tipo de rutina)
  String state; // ="NONE"; //Work, Rest, RestBtwnSets, NONE, Finished?  (para saber en que estado se encuentra la rutina)

  Routine();
  void setRoutine(int wTime, int rTime, int rbtwnTime, int tRounds, int tSets, int time_left, String rMode, String rState);
  void roundIncrement();
  void roundDecrement();
  void setIncrement();
  void setDecrement();
};
