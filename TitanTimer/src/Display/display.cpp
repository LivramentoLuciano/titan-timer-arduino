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

  // drawStringCenterCenter("TITAN", 2);
  // screen.write();
  // delay(2000);
  writeStringSlider("ESTO ES BOKE", 2);
  clrscr();
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
  drawStringBottomLeft(mmss(t), 1, 1);
  screen.write();
}

void Display::updateRound(char r, char rt)
{
  drawStringTopRight(String(r, DEC) + "/" + String(rt, DEC), 1, 1);
  screen.write();
}

void Display::updateSet(char s, char st)
{
  drawStringBottomRight(String(s, DEC) + "/" + String(st, DEC), 1, 1);
  screen.write();
}

void Display::updateInstance(String instance)
{
  instance.toUpperCase();
  drawStringTopLeft(instance, 1, 1);
  screen.write();
}

// actualiza: tiempo, round, set e instancia
void Display::updateAll(int t, char r, char rt, char s, char st, String i)
{
  clrscr();
  updateTime(t);
  updateRound(r, rt);
  updateSet(s, st);
  updateInstance(i);
  // ver si el hecho que c/u de estas tenga un 'screen.write()' tiene implicancias (como borrar cosas q no quiero borrar)
}

void Display::showNewInstanceMsg(String newInstanceMsg)
{
  clrscr();
  drawStringCenterCenter(newInstanceMsg, 2);
  screen.write();
}

void Display::updateInitMsg(int t)
{
  drawStringTopCenter("Inicio en:", 1);
  drawStringBottomCenter(String(t, DEC), 1);
  screen.write();
}

void Display::showIdleMsg()
{
  // O el mismo slider del principio (cada X segundos, sleep 5 seconds + mensaje + sleep ...)
  drawStringTopCenter("Titan", 1);
  drawStringBottomCenter("Academy", 1);
  screen.write();
}

// drawString() -> Imprime un mensaje
// Argumentos:
//  @text: mensaje a imprimir
//  @x: posicion en x
//  @y: posicion en y
//  @size: grosor de fuente
void Display::drawString(String text, int x, int y, uint8_t size)
{
  int _spacer = 1 * size;
  int _width = DEFAULT_WIDTH * size + _spacer;
  for (int c = 0; c < text.length(); c++)
  {
    int _x = x + _width * c;
    // carga el bitmap (como un buffer donde pongo todo lo que quiero imprimir y dsp mando 'write')
    screen.drawChar(_x, y, text[c], HIGH, LOW, size);
  }
}

// Metodos con alineacion
// 'marginX / Y'-> resuelven margen izq/der, inf/sup
// 'size' -> (aparenta andar, mi display no alcanza)
void Display::drawStringTopLeft(String text, uint8_t size, int marginX = 0, int marginY = 0)
{
  int x = 0;
  int y = 0;
  drawString(text, x + marginX, y + marginY, size);
}

void Display::drawStringTopCenter(String text, uint8_t size, int marginY = 0)
{
  int y = 0;
  int x = (screen.width() - text.length() * (DEFAULT_WIDTH * size + 1)) / 2;
  drawString(text, x, y + marginY, size);
}

void Display::drawStringTopRight(String text, uint8_t size, int marginX = 0, int marginY = 0)
{
  int y = 0;
  int x = screen.width() - text.length() * (DEFAULT_WIDTH * size + 1);
  drawString(text, x - marginX, y + marginY, size);
}

void Display::drawStringCenterLeft(String text, uint8_t size, int marginX = 0)
{
  int x = 0;
  int y = (screen.height() - 8 * size) / 2;
  drawString(text, x + marginX, y, size);
}

void Display::drawStringCenterCenter(String text, uint8_t size)
{
  int x = (screen.width() - text.length() * (DEFAULT_WIDTH * size + 1)) / 2;
  int y = (screen.height() - 8 * size) / 2;
  drawString(text, x, y, size);
}

void Display::drawStringCenterRight(String text, uint8_t size, int marginX = 0)
{
  int x = screen.width() - text.length() * (DEFAULT_WIDTH * size + 1);
  int y = (screen.height() - 8 * size) / 2;
  drawString(text, x - marginX, y, size);
}

void Display::drawStringBottomLeft(String text, uint8_t size, int marginX = 0, int marginY = 0)
{
  int x = 0;
  int y = screen.height() - 8 * size;
  drawString(text, x + marginX, y - marginY, size);
}

void Display::drawStringBottomCenter(String text, uint8_t size, int marginY = 0)
{
  int x = (screen.width() - text.length() * (DEFAULT_WIDTH * size + 1)) / 2;
  int y = screen.height() - 8 * size;
  drawString(text, x, y - marginY, size);
}

void Display::drawStringBottomRight(String text, uint8_t size, int marginX = 0, int marginY = 0)
{
  int y = screen.height() - 8 * size;
  int x = screen.width() - text.length() * (DEFAULT_WIDTH * size + 1);
  drawString(text, x - marginX, y - marginY, size);
}

void Display::writeStringSlider(String text, uint8_t size)
{
  int _spacer = 1 * size;
  int _width = 5 * size + _spacer;
  int wait = 10;

  for (int i = 0; i < _width * text.length() + screen.width() - 1 - _spacer; i++)
  {
    screen.fillScreen(LOW);
    int letter = i / _width;
    int x = (screen.width() - 1) - i % _width;
    int y = (screen.height() - 8 * size) / 2; // center the text vertically

    while (x + _width - _spacer >= 0 && letter >= 0)
    {
      if (letter < text.length())
      {
        screen.drawChar(x, y, text[letter], HIGH, LOW, size);
      }

      letter--;
      x -= _width;
    }

    screen.write(); // Send bitmap to display

    delay(wait);
  }
}