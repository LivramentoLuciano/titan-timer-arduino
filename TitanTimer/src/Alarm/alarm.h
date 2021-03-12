#ifndef ALARM_H
#define ALARM_H

#define ENCENDER_BUZZER HIGH
#define APAGAR_BUZZER   LOW


class Alarma
{
private:
    const char PIN_BUZZER = 9;
    const int BEEP_INTERVAL = 500; // 500 ms de sonido
    bool isBuzzerOn;        // Para toogle (alarma encendida pero intervalos)
    void buzzerOn();
    void buzzerOff();

public:
    Alarma();
    void on();
    void off();
};

#endif