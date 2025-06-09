#ifndef BALON_H
#define BALON_H

#include <Adafruit_ILI9341.h>

extern Adafruit_ILI9341 screen;

class Balon {
private:
  int x, y;
  bool activo;
  int xAnterior, yAnterior;

public:
  Balon() : x(-1), y(-1), activo(false), xAnterior(-1), yAnterior(-1) {}

  Balon(int startX, int startY) : x(startX), y(startY), activo(true), xAnterior(startX), yAnterior(startY) {}

  void borrar() {
    if (activo) {
      screen.fillRect(x, y, 6, 6, ILI9341_BLACK);  // Tamaño actualizado a 6x6
      activo = false;
    }
  }

  void mover() {
    if (activo) {
      if (xAnterior != -1 && yAnterior != -1) {
        screen.fillRect(xAnterior, yAnterior, 6, 6, ILI9341_GREEN);  // Borrar anterior (6x6)
      }

      y -= 20;  // Movimiento vertical

      xAnterior = x;
      yAnterior = y;
    }
  }

  void dibujar(Adafruit_ILI9341& screen) {
    if (activo) {
      screen.fillRect(x, y, 6, 6, ILI9341_WHITE);  // Nuevo dibujo (6x6)
    }
  }

  void desactivar() {
    activo = false;
  }

  bool estaActiva() const {
    return activo;
  }

  int getX() const { return x; }
  int getY() const { return y; }
  int getAncho() const { return 6; }  // Nuevo ancho
  int getAlto() const { return 6; }   // Nuevo alto

  void reiniciar() {
    x = -1;
    y = -1;
    activo = false;
    xAnterior = -1;
    yAnterior = -1;
  }
};

#endif
