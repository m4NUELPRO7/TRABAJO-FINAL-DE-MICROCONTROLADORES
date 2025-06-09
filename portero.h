#ifndef PORTERO_H
#define PORTERO_H

#include <Adafruit_ILI9341.h>
#include "balon.h"  

const unsigned char porteroBitmap[] PROGMEM = {
  0b00000000, 0b00000000, 0b00000000,
  0b00000111, 0b11100000, 0b00000000, 
  0b00001111, 0b11110000, 0b00000000,
  0b00001111, 0b11110000, 0b00000000,
  0b00000111, 0b11100000, 0b00000000,
  0b00000111, 0b11100000, 0b00000000,
  0b00011111, 0b11111100, 0b00000000, 
  0b00011111, 0b11111100, 0b00000000,
  0b00001111, 0b11111000, 0b00000000, 
  0b00000111, 0b11100000, 0b00000000, 
  0b00000111, 0b11100000, 0b00000000, 
  0b00000111, 0b11100000, 0b00000000,
  0b00001100, 0b00110000, 0b00000000, 
  0b00001100, 0b00110000, 0b00000000,
  0b00001100, 0b00110000, 0b00000000,
  0b00001000, 0b00010000, 0b00000000, 
  0b00001000, 0b00010000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000,
};

class Portero {
private:
  int x, y;
  int ancho, alto;
  int velocidad;
  int limiteIzq, limiteDer;

public:
  Portero() {
    x = 100;       // Posición inicial
    y = 15;        // Justo debajo de la portería
    ancho = 20;
    alto = 20;
    velocidad = 30;
    limiteIzq = 60;   // Límite izquierdo de la portería
    limiteDer = 180;  // Límite derecho de la portería
  }

  void mover() {
    x += velocidad;
    if (x <= limiteIzq || x + ancho >= limiteDer) {
      velocidad = -velocidad;
    }
  }

  void dibujar(Adafruit_ILI9341& pantalla) {
    pantalla.drawBitmap(x, y, porteroBitmap, ancho, alto, ILI9341_WHITE);
  }

  void borrar(Adafruit_ILI9341& pantalla) {
    pantalla.drawBitmap(x, y, porteroBitmap, ancho, alto, ILI9341_GREEN);
  }

  bool colisionaCon(const Balon& balon) const {
    
    return !(
      balon.getX() + balon.getAncho() < x ||
      balon.getX() > x + ancho ||
      balon.getY() + balon.getAlto() < y ||
      balon.getY() > y + alto
    );
  }

  int getX() const { return x; }
  int getY() const { return y; }
  int getAncho() const { return ancho; }
  int getAlto() const { return alto; }
};

#endif
