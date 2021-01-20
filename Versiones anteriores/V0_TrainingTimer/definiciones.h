#define disp1_a 2
#define disp1_b 3
#define disp1_c 4
#define disp1_d 5
#define disp1_e 6
#define disp1_f 7
#define disp1_g 8
#define disp1_p 9

#define ENCENDER_LED HIGH
#define APAGAR_LED LOW

const int display7_1[8] = {disp1_a,disp1_b,disp1_c,disp1_d,disp1_e,disp1_f,disp1_g,disp1_p};
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
void imprimirNum (int);
void cuentaDisplay(int);
