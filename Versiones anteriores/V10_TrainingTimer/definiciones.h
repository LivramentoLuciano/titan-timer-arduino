#ifndef definiciones_h
#define definiciones_h

/********************************* Declaracion de Variables Globales ************************/
extern char cG_bufferRx[100];
extern volatile char cG_nuevaTrama;

extern volatile int instanciaRutina; 
extern volatile int segundos, pasoSegundo;
extern volatile long inicioAlarma;
extern volatile int alarmaON;
extern volatile int actualRound, actualSet;
extern volatile int duracionActividad, duracionDescanso, totalRounds, totalSets, duracionDescansoSets;
extern char *mode;

/********************************* Definicion de Constantes *********************************/

#define DIG_DISPLAY 2
#define disp1_a 2
#define disp1_b 3
#define disp1_c 4
#define disp1_d 5
#define disp1_e 6
#define disp1_f 7
#define disp1_g 8
#define disp1_p 9

#define pinBuzzer 9

#define ENCENDER_LED HIGH
#define APAGAR_LED LOW

#define FINALIZADA        0
#define TRABAJO           1
#define DESCANSO          2
#define DESCANSO_SETS     3
#define PAUSA             4
#define REANUDAR          5
#define INIT              6
#define NONE              7

#define T_ALARMA_TRABAJO_SHORT  3
#define T_ALARMA_TRABAJO_LARGE  3
#define T_ALARMA_DESCANSO 3
#define INTERVALO_BEEP    150

#define HDR_START                 'S'
#define HDR_PAUSE                 'P'
#define HDR_RESUME                'R'
#define HDR_STOP                  's'
#define HDR_RESINCRONIZE          'r'
#define HDR_R_INCREMENT           'I'
#define HDR_S_INCREMENT           'i'
#define HDR_R_DECREMENT           'D'
#define HDR_S_DECREMENT           'd'

const int display7[DIG_DISPLAY][8] = {
  {disp1_a,disp1_b,disp1_c,disp1_d,disp1_e,disp1_f,disp1_g,disp1_p},
  {0,0,0,0,0,0,0,0}
};
const byte caracterNumerico[10] = {
  0b00111111,
  0b00000110,
  0b01011011,
  0b01001111,
  0b01100110,
  0b01101101,
  0b01111101,
  0b00000111,
  0b01111111,
  0b01100111  
};

const int duracionInit = 3;

/************************************** Declaracion de prototipos de funciones ********************************/
void initTrainingTimer (void);
void cleanDisplay7 (int);
void imprimirNum (int, int);
void cuentaDisplay(int);

void filtro_trama(char*);
void procesar_trama();

//las hago por ahora en el de functions, pero van a tener que se parte de la class Routine (metodo de esta)
void roundIncrement (void);
void roundDecrement (void);
void setIncrement (void);
void setDecrement (void);
void routineResincronize (void);

#endif
