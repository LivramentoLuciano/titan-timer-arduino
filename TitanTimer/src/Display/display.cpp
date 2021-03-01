#include <Arduino.h>
#include "display.h"
#include "../methods/methods.h"

void Display::init()
{
  screen.setIntensity(3);
  for (int fila = 0; fila < _vDisplays; fila++)
    for (int col = 0; col < _hDisplays; col++)
    {
      // position <0,0> es topLeft y yo tengo mis display arrancando en <topBottom>
      // por eso _vDisplays - 1 - fila (sino seria 'fila' solo)
      // fila * _hDisplays -> En fila: 1, col:0, arranque en el display n:_hDisplays
      screen.setPosition(fila * _hDisplays + col, col, _vDisplays - 1 - fila);
      screen.setRotation(fila * _hDisplays + col, 1);
    }

  // drawString("Titan Timer", 0, 0, true, 2);
  writeStringSlider("Titan Timer", 1);
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
  // clrscr(); // saco que limpie todo ya que tengo sectorizado 'time,round,set' (deberia limpiar solo Time aca)
  // ademas, parece que es innecesario limpiar, drawChar repinta todo (borra lo previo, parece)
  drawString(mmss(t), 0, 0, true, 2);
  screen.write();
}

void Display::updateRound(char r)
{
  drawString(String(r, DEC), 0, 8, false, 1);
  screen.write();
}

void Display::updateSet(char s)
{
  drawString(String(s, DEC), 0, 0, false, 1);
  screen.write();
}

// actualiza: tiempo, round y set
void Display::updateAll(int t, char r, char s)
{
  updateTime(t);
  updateRound(r);
  updateSet(s);
  // ver si el hecho que c/u de estas tenga un 'screen.write()' tiene implicancias (como borrar cosas q no quiero borrar)
}

// drawString() -> Imprime un mensaje
// Argumentos:
//  @text: mensaje a imprimir
//  @margin: posicion inicial (margen hacia el lado al que se encuentra alineado)
//  @y: posicion en y
//  @alig: alineación izquierda (normal) o derecha
//  @size: grosor de fuente
//
// Ver si el 'margin' (en X) e 'y' deberian modificarse si usar el display rotado
void Display::drawString(String text, int margin, int y, bool align, uint8_t size)
{
  int _spacer = 1; //*size
  int _width = 5 * size + _spacer;

  if (align) // alineado a la izquierda (normal)
  {
    for (int c = 0; c < text.length(); c++)
    {
      int x = margin + _width * c;
      // carga el bitmap (como un buffer donde pongo todo lo que quiero imprimir y dsp mando 'write')
      screen.drawChar(x, y, text[c], HIGH, LOW, size);
    }
  }
  else // alineado a la derecha
  {
    for (int c = text.length() - 1; c >= 0; c--)
    {
      int x = screen.width() - _width * (text.length() - c) - margin;
      // carga el bitmap (como un buffer donde pongo todo lo que quiero imprimir y dsp mando 'write')
      screen.drawChar(x, y, text[c], HIGH, LOW, size);
    }
  }
}

void Display::writeStringSlider(String text, uint8_t size)
{
  int spacer = 1;
  int _width = 5 * size + _spacer;
  int wait = 30;

  for (int i = 0; i < _width * text.length() + screen.width() - 1 - spacer; i++)
  {
    screen.fillScreen(LOW);
    int letter = i / _width;
    int x = (screen.width() - 1) - i % _width;
    int y = (screen.height() - 8) / 2; // center the text vertically

    while (x + _width - spacer >= 0 && letter >= 0)
    {
      if (letter < text.length())
      {
        screen.drawChar(x, y, text[letter], HIGH, LOW, 1);
      }

      letter--;
      x -= _width;
    }

    screen.write(); // Send bitmap to display

    delay(wait);
  }
}