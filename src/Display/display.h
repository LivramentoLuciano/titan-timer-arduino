#ifndef display_H
#define display_H

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Max72xxPanel.h>
#include "../model/Routine.h"

#define _pinCS 10
#define _hDisplays 8
#define _vDisplays 2
#define DEFAULT_WIDTH 5

const char SLIDER_MSG_WAIT = 10;

class Display
{
private:
  Max72xxPanel screen = Max72xxPanel(_pinCS, _hDisplays, _vDisplays);
  // const int _spacer = 1;
  // const int _width = 5 + _spacer; // ancho de fuente

public:
  Display(){};
  void init();

  // metodos
  void writeStringSlider(String text, uint8_t size);  // No bloqueante
  void drawString(String text, int x, int y, uint8_t size);
  void drawStringTopLeft(String text, uint8_t size, int marginX = 0, int marginY = 0);
  void drawStringTopRight(String text, uint8_t size, int marginX = 0, int marginY = 0);
  void drawStringBottomLeft(String text, uint8_t size, int marginX = 0, int marginY = 0);
  void drawStringBottomRight(String text, uint8_t size, int marginX = 0, int marginY = 0);
  void drawStringCenterLeft(String text, uint8_t size, int marginX = 0);
  void drawStringCenterRight(String text, uint8_t size, int marginX = 0);
  void drawStringTopCenter(String text, uint8_t size, int marginY = 0);
  void drawStringBottomCenter(String text, uint8_t size, int marginY = 0);
  void drawStringCenterCenter(String text, uint8_t size);
  void clrscr();

  void updateTime(int t);
  void updateRound(char r, char rt);
  void updateSet(char s, char st);
  void updateAll(int t, char r, char rt, char s, char st, String i); 
  void updateInstance(String instance); // ver si hacer que reciba RoutineInstance (enum)
  void showNewInstanceMsg(RoutineInstance instance, RoutineMode mode); // estas funciones no se si seria mejor tenerlas en p/ej 'methods' y q llamen a 'display.draw...' en lugar de tenerlas en 'display'
  void updateInitMsg(int t);
  void showIdleMsg(); // Muestra algo en tiempos muertos ?? 
  void resetIdleMsg();
};

#endif