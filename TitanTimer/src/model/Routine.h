#ifndef Routine_h
#define Routine_h

enum RoutineState
{
    STARTED,
    PAUSED,
    STOPPED
};
enum RoutineInstance
{
    WORK,
    REST,
    REST_SET,
    INIT,
    NOTHING
};

class Routine
{
private:
    int tWork, tRest, tRestSets;
    char rounds, sets, actualRound, actualSet;
    RoutineState state;
    RoutineInstance instance;
    unsigned long int t;

public:
    // constructor -> No lo uso (Ver notas) -> Uso en cambio '.init()'
    Routine(int _tWork, int _tRest, int _tRestSets, int _rounds, int _sets) : state(STOPPED), instance(NOTHING)
    {
        tWork = _tWork;
        tRest = _tRest;
        tRestSets = _tRestSets;
        rounds = _rounds;
        sets = _sets;
    };

    // constructor pelao
    Routine() : state(STOPPED), instance(NOTHING){};

    // getters y setters basicos
    int get_tWork() { return tWork; }
    void set_tWork(int t) { tWork = t; }
    int get_tRest() { return tRest; }
    void set_tRest(int t) { tRest = t; }
    int get_tRestSets() { return tRestSets; }
    void set_tRestSets(int t) { tRestSets = t; }
    char get_rounds() { return rounds; }
    void set_rounds(char r) { rounds = r; }
    char get_sets() { return sets; }
    void set_sets(char s) { sets = s; }
    char get_actualRound() { return actualRound; }
    void set_actualRound(char aR) { actualRound = aR; }
    char get_actualSet() { return actualSet; }
    void set_actualSet(char aS) { actualSet = aS; }
    RoutineState get_state() { return state; };
    void set_state(RoutineState s) { state = s; };
    RoutineInstance get_instance() { return instance; }
    void set_instance(RoutineInstance i) { instance = i; }
    unsigned long int get_t() { return t; }
    void set_t(unsigned long int _t) { t = _t; }

    //
    void init(int _tWork, int _tRest, int _tRestSets, char _rounds, char _sets); // + state, instance,... ??
    void roundUp();
    void roundDown();
    void setUp();
    void setDown();

    unsigned long int get_tLeft(); // en funcion de t y la duracion de instance -> no necesito que el atributo tLeft exista}
};

#endif