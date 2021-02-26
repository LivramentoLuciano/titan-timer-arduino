#include "Routine.h"

void Routine::init(int _tWork, int _tRest, int _tRestSet, char _rounds, char _sets)
{
    tWork = _tWork;
    tRest = _tRest;
    tRest = _tRestSet;
    rounds = _rounds;
    sets = _sets;

    state = STARTED;
    instance = INIT;
}

int Routine::get_tLeft(){
    if (instance == WORK) return tWork - t;
    else if (instance == REST) return tRest - t;
    else if (instance == REST_SET) return tRestSets - t;
    else if (instance == INIT) return tInit - t;
    else return 0;
}