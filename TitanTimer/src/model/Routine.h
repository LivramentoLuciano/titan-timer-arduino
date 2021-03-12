#ifndef routine_h
#define routine_h
#include "Arduino.h"

enum RoutineInstance
{
    WORK,
    REST,
    REST_SET,
    INIT,
    FINISHED,
    NOTHING
};

enum RoutineMode
{
    AMRAP,
    HIIT,
    TABATA,
    COMBATE
};

class Routine
{
private:
    int tWork, tRest, tRestSets;
    char rounds, sets, actualRound, actualSet;
    RoutineInstance instance;
    RoutineMode mode;
    int t;
    const int tInit = 3;
    bool isLoaded = false;

public:
    // constructor -> No lo uso (Ver notas) -> Uso en cambio '.init()'
    Routine(int _tWork, int _tRest, int _tRestSets, int _rounds, int _sets) : instance(NOTHING)
    {
        tWork = _tWork;
        tRest = _tRest;
        tRestSets = _tRestSets;
        rounds = _rounds;
        sets = _sets;
    };

    // constructor pelao
    Routine() : instance(NOTHING){};

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
    RoutineInstance get_instance() { return instance; }
    char *get_instanceString();
    void set_instance(RoutineInstance i) { instance = i; }
    RoutineMode get_mode() { return mode; }
    void set_mode(String m);
    int get_t() { return t; }
    void set_t(int _t) { t = _t; }
    bool get_isLoaded() { return isLoaded; } // si la rutina esta cargada (para evitar un Play sin rutina, ver q nunca lo borro que pasa si hago otras rutinas o repito a finalizar una)

    //
    void set_settings(String _mode, int _tWork, int _tRest, int _tRestSets, char _rounds, char _sets); // Carga rutina
    void init();                                                                         // start rutina
    void roundUp() { actualRound++; }                                                    /// No hago todo el chequeo para saber si avanzo o no de round, xq implica otras cosas como el timer en esa decision
    void roundDown() { actualRound--; }
    void setUp() { actualSet++; }
    void setDown() { actualSet--; }
    void nextInstance();

    void incSeconds() { t++; }
    int get_tLeft(); // en funcion de t y la duracion de instance -> no necesito que el atributo tLeft exista}
    bool isLastRound() { return actualRound == rounds; }
    bool isLastSet() { return actualSet == sets; }
    bool lastSeconds();
    bool enabled(); // Omitir comandos entrantes (roundUp/Dwn, etc) si no esta en habilitado
};

#endif