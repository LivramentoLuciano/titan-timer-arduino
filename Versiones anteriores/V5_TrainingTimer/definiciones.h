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

#define T_ALARMA_TRABAJO  5
#define T_ALARMA_DESCANSO 5
#define INTERVALO_BEEP    150

#define HDR_TABATA                'T'
#define HDR_AMRAP                 'A'
#define HDR_TIEMPOxCANTIDAD       'C'
#define HDR_KICKBOXING            'K'
#define HDR_TKD                   'W'
#define HDR_PAUSA                 'P'
#define HDR_REANUDAR              'R'
#define HDR_RESET                 'D'

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

void initTrainingTimer (void);
void cleanDisplay7 (int);
void imprimirNum (int, int);
void cuentaDisplay(int);
void interrupcion_1seg (void);

void filtro_trama(char*, char*);
void procesar_trama(char*, char*,int*, int*, int*, int*, int*, int*, volatile int*, int*);
char* subStr (char*, char*, int);
