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