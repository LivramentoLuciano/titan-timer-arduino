
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
    INIT
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
    // getters y setters basicos
    int get_tWork();
    void set_tWork(int t);
    int get_tRest();
    void set_tRest(int t);
    int get_tRestSets();
    void set_tRestSets(int t);
    char get_rounds();
    void set_rounds(char r);
    char get_sets();
    void set_sets(char s);
    char get_actualRound();
    void set_actualRound(char aR);
    char get_actualSet();
    void set_actualSet(char aS);
    RoutineState get_state();
    void set_state(RoutineState state);
    RoutineInstance get_instance();
    void set_instanceget_instance(RoutineInstance instanceget_instance);
    unsigned long int get_t();
    void set_t(unsigned long int t);

    //
    void set_routineSettings(int tWork, int tRest, int tRestSets, char rounds, char sets);
    void initRoutine(); // (state, instance,...) + settings ?? 
    void roundUp();
    void roundDown();
    void setUp();
    void roundUp();

    unsigned long int get_tLeft(); // en funcion de t y la duracion de instance -> no necesito que el atributo tLeft exista}
};