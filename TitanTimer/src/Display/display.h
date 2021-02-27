#ifndef display_h
#define display_h

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Max72xxPanel.h>

#define _pinCS 10
#define _hDisplays 4
#define _vDisplays 1

class Display
{
private:
  Max72xxPanel screen = Max72xxPanel(_pinCS, _hDisplays, _vDisplays);
  const int _spacer = 1;
  const int _width = 5 + _spacer; // ancho de fuente

public:
  // sarasa LCD / LedMatriz ...
  Display(){};

  void init();
  // void setPosition(byte display, byte x, byte y) { _screen.setPosition(display, x, y); }

  // metodos
  void updateTime(int t);
  void drawString(String text, int margin, int y, bool align);
};

#endif