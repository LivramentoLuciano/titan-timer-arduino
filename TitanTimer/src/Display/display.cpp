#include <Arduino.h>
#include "display.h"
#include "../methods/methods.h"

void Display::init()
{
  screen.setIntensity(3);
  screen.setPosition(0, 0, 0); // The first display is at <0, 0>
  screen.setPosition(1, 1, 0); // The second display is at <1, 0>
  screen.setPosition(2, 2, 0); // The third display is at <2, 0>
  screen.setPosition(3, 3, 0); // And the last display is at <3, 0>
                               //  ...
  screen.setRotation(0, 1);    // The first display is position upside down
  screen.setRotation(1, 1);    // The same hold for the last display
  screen.setRotation(2, 1);    // The first display is position upside down
  screen.setRotation(3, 1);    // The same hold for the last display

  drawString("holis", 0, 0, true);
  screen.write();
}

void Display::clrscr()
{
  // limpio pantalla
  screen.fillScreen(LOW);
  screen.write();
}

void Display::updateTime(int t)
{
  clrscr();
  drawString(mmss(t),0,0,true);
  screen.write();
}


// drawString() -> Imprime un mensaje
// Argumentos:
//  @text: mensaje a imprimir
//  @margin: posicion inicial (margen hacia el lado al que se encuentra alineado)
//  @y: posicion en y (no lo probe con mas de 1 matriz en vertical aun)
//  @alig: alineación izquierda (normal) o derecha
//
// Ver si el 'margin' (en X) e 'y' deberian modificarse si usar el display rotado
void Display::drawString(String text, int margin, int y, bool align)
{

  if (align) // alineado a la izquierda (normal)
  {
    for (int c = 0; c < text.length(); c++)
    {
      int x = margin + _width * c;
      // carga el bitmap (como un buffer donde pongo todo lo que quiero imprimir y dsp mando 'write')
      screen.drawChar(x, y, text[c], HIGH, LOW, 1);
    }
  }
  else // alineado a la derecha
  {
    for (int c = text.length() - 1; c >= 0; c--)
    {
      int x = screen.width() - _width * (text.length() - c) - margin;
      // carga el bitmap (como un buffer donde pongo todo lo que quiero imprimir y dsp mando 'write')
      screen.drawChar(x, 0, text[c], HIGH, LOW, 1);
    }
  }
}