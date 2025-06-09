#ifndef PORTERIA_H
#define PORTERIA_H

#include <Adafruit_ILI9341.h>

const uint8_t PORTERIA_WIDTH = 20;
const uint8_t PORTERIA_HEIGHT = 20;

// Bitmap simple de una portería (20x20)
const unsigned char porteriaBitmap[] = {
  0b11111111, 0b11000000,  
  0b10000000, 0b01000000,  
  0b10000000, 0b01000000,
  0b10000000, 0b01000000,
  0b10000000, 0b01000000,
  0b10000000, 0b01000000,
  0b10000000, 0b01000000,
  0b10000000, 0b01000000,
  0b10000000, 0b01000000,
  0b10000000, 0b01000000,
  0b10000000, 0b01000000,
  0b10000000, 0b01000000,
  0b10000000, 0b01000000,
  0b10000000, 0b01000000,
  0b10000000, 0b01000000,
  0b10000000, 0b01000000,
  0b10000000, 0b01000000,
  0b10000000, 0b01000000,
  0b11111111, 0b11000000,  
  0b00000000, 0b00000000   
};

class Porteria {
  public:
    int x, y, w, h;

    // Constructor por defecto
    Porteria() {
      x = 60;
      y = 10;
      w = PORTERIA_WIDTH;
      h = PORTERIA_HEIGHT;
    }

    // Constructor con parámetros
    Porteria(int posX, int posY) {
      x = posX;
      y = posY;
      w = PORTERIA_WIDTH;
      h = PORTERIA_HEIGHT;
    }

    void dibujar(Adafruit_ILI9341& screen) {
      screen.drawBitmap(x, y, porteriaBitmap, w, h, ILI9341_WHITE);
    }

    void borrar(Adafruit_ILI9341& screen) {
      screen.drawBitmap(x, y, porteriaBitmap, w, h, ILI9341_GREEN);
    }
};

#endif
