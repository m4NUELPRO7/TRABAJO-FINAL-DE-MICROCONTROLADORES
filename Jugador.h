#ifndef JUGADOR_H
#define JUGADOR_H

#include <Adafruit_ILI9341.h>
#include "balon.h"

extern Adafruit_ILI9341 screen;
const unsigned char jugadorBitmap[] PROGMEM = {
  0b00011111, 0b11111000, 
  0b00111111, 0b11111100,
  0b00111111, 0b11111100,
  0b00011111, 0b11111000,
  0b00011111, 0b11111000,
  0b01111111, 0b11111110, 
  0b01111111, 0b11111110,
  0b00111111, 0b11111100, 
  0b00011111, 0b11111000, 
  0b00011111, 0b11111000,
  0b00011111, 0b11111000,
  0b00110000, 0b00001100, 
  0b00110000, 0b00001100,
  0b00110000, 0b00001100,
  0b00110000, 0b00001100, 
  0b00000000, 0b00000000,
};


  

  #define JUGADOR_WIDTH  16
  #define JUGADOR_HEIGHT 16
  

class Jugador {
    private:
      int x, y;
  
    public:
      Jugador(int startX, int startY) : x(startX), y(startY) {}
  
      void dibujar() {
        screen.drawBitmap(x, y, jugadorBitmap, JUGADOR_WIDTH, JUGADOR_HEIGHT, ILI9341_BLUE);
      }

      int getAncho() {
        return 16;  
      }
      
      int getAlto() {
        return 16;  
      }
      

      void borrar() {
        screen.drawBitmap(x, y, jugadorBitmap, JUGADOR_WIDTH, JUGADOR_HEIGHT, ILI9341_GREEN);
      }
  
      void moverIzquierda(int paso = 22) {
        if (x - paso >= 0) {
          borrar();
          x -= paso;
          dibujar();
        }
      }
  
      void moverDerecha(int paso = 22) {
        if (x + JUGADOR_WIDTH + paso <= screen.width()) {
          borrar();
          x += paso;
          dibujar();
        }
      }

      void moverArriba(int paso = 22) {
        if (y - paso >= 0) {
          borrar();
          y -= paso;
          dibujar();
        }
      }
  
      void moverAbajo(int paso = 22) {
        if (y + JUGADOR_WIDTH + paso <= screen.width()) {
          borrar();
          y += paso;
          dibujar();
        }
      }
      

      void reiniciar() {
        borrar();     // Borra al jugador en su posición actual
        x = 100;      // Coordenada X inicial
        y = 280;      // Coordenada Y inicial
        dibujar();    // Dibuja a el jugdaen la nueva posición
      }

      int getX() const { return x; }
      int getY() const { return y; }
  };
  
  #endif