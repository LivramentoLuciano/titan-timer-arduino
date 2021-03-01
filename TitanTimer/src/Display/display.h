#ifndef display_H
#define display_H

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Max72xxPanel.h>

#define _pinCS 10
#define _hDisplays 8
#define _vDisplays 2

class Display
{
private:
  Max72xxPanel screen = Max72xxPanel(_pinCS, _hDisplays, _vDisplays);
  const int _spacer = 1;
  const int _width = 5 + _spacer; // ancho de fuente

public:
  Display(){};
  void init();

  // metodos
  void drawString(String text, int margin, int y, bool align, uint8_t size);
  void writeStringSlider(String text, uint8_t size);
  void clrscr();
  void updateTime(int t);
};

#endif