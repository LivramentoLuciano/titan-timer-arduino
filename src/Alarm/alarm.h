#ifndef ALARM_H
#define ALARM_H

#define ENCENDER_BUZZER HIGH
#define APAGAR_BUZZER LOW

enum AlarmMode
{
    ALARM_SHORT,
    ALARM_NORMAL,
    ALARM_XL
};

class Alarma
{
private:
    const char PIN_BUZZER = 9;
    const int BEEP_INTERVAL_NORMAL = 500; // 500 ms de sonido, durante los ultimos 3s
    const int BEEP_INTERVAL_SHORT = 200;  // En modos combate y amrap suenan beeps mas cortos, solo al faltar 10s
    bool isBuzzerOn;                      // Para toogle (alarma encendida pero intervalos)
    void buzzerOn();
    void buzzerOff();

public:
    Alarma();
    void on(AlarmMode mode);    // Beeps intermitentes (variables)
    void onCont();              // Beep continuo
    void off();
};

#endif