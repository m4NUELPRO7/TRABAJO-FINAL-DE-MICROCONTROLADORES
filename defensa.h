#ifndef DEFENSA_H
#define DEFENSA_H

#include <Adafruit_ILI9341.h>

extern Adafruit_ILI9341 screen;

const uint8_t DEFENSA_WIDTH = 20;
const uint8_t DEFENSA_HEIGHT = 20;

const unsigned char personaBitmap[] PROGMEM = {
  
  0b00001111, 0b11111111, 0b00000000, 
  0b00011111, 0b11111111, 0b10000000,
  0b00011111, 0b11111111, 0b10000000,
  0b00001111, 0b11111111, 0b00000000,
  0b00001111, 0b11111111, 0b00000000,

  0b01111111, 0b11111111, 0b11100000, 
  0b01111111, 0b11111111, 0b11100000,
  0b00111111, 0b11111111, 0b11000000, 
  0b00111111, 0b11111111, 0b11000000,
  0b00011111, 0b11111111, 0b10000000, 

  0b00011111, 0b11111111, 0b10000000,
  0b00011111, 0b11111111, 0b10000000,
  0b00111000, 0b00000000, 0b11100000, 
  0b00111000, 0b00000000, 0b11100000,
  0b00111000, 0b00000000, 0b11100000,

  0b00111000, 0b00000000, 0b11100000, 
  0b00111000, 0b00000000, 0b11100000,
  0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000,
};


class Defensa {
private:
  int x, y;
  int velocidad;
  bool movimientoHorizontal;

  int limiteIzq, limiteDer;
  int limiteSup, limiteInf;

public:
  Defensa(int startX = 0, int startY = 0, int vel = 0, bool horizontal = true,
          int limIzq = 0, int limDer = 0, int limSup = 0, int limInf = 0)
    : x(startX), y(startY), velocidad(vel), movimientoHorizontal(horizontal),
      limiteIzq(limIzq), limiteDer(limDer), limiteSup(limSup), limiteInf(limInf) {}

  void dibujar() {
    screen.drawBitmap(x, y, personaBitmap, DEFENSA_WIDTH, DEFENSA_HEIGHT, ILI9341_BLACK);
  }

  void borrar() {
    screen.fillRect(x, y, DEFENSA_WIDTH, DEFENSA_HEIGHT, ILI9341_GREEN);
  }

  void mover() {
    if (movimientoHorizontal) {
      x += velocidad;
      if (x <= limiteIzq || x + DEFENSA_WIDTH >= limiteDer) {
        velocidad = -velocidad;
      }
    } else {
      y += velocidad;
      if (y <= limiteSup || y + DEFENSA_HEIGHT >= limiteInf) {
        velocidad = -velocidad;
      }
    }
  }

  void actualizar() {
    borrar();
    mover();
    dibujar();
  }

  int getX() const { return x; }
  int getY() const { return y; }
};

#endif
