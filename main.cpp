#include <Adafruit_GFX.h> 
#include <Adafruit_ILI9341.h>

#define TFT_DC   7
#define TFT_CS   6
#define TFT_MOSI 11
#define TFT_CLK  13
#define TFT_RST  10
#define TFT_MISO 12

#define BTN_LEFT  18
#define BTN_RIGHT 19
#define BOTON_DISPARO 2
#define BTN_ARRIBA  20
#define BTN_ABAJO 21
#define PIN_BUZZER 8

Adafruit_ILI9341 screen = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

#include "defensa.h"
#include "jugador.h"
#include "balon.h"  

Balon balon;  

#include "portero.h"
#include "porteria.h"


enum EstadoNivel2 { JUGANDO_CON_DEFENSAS, MOSTRAR_PORTERIA };
EstadoNivel2 estadoNivel2 = JUGANDO_CON_DEFENSAS;


Jugador jugador(100, 280);

const int NUM_DEFENSAS = 10;

int posicionesX[NUM_DEFENSAS] = {20, 60, 100, 140, 180, 220, 40, 80, 160, 200};
int posicionesY[NUM_DEFENSAS] = {60, 80, 60, 100, 80, 120, 140, 160, 180, 200};

Defensa defensas[NUM_DEFENSAS] = {
  Defensa(posicionesX[0], posicionesY[0], 0),
  Defensa(posicionesX[1], posicionesY[1], 0),
  Defensa(posicionesX[2], posicionesY[2], 0),
  Defensa(posicionesX[3], posicionesY[3], 0),
  Defensa(posicionesX[4], posicionesY[4], 0),
  Defensa(posicionesX[5], posicionesY[5], 0),
  Defensa(posicionesX[6], posicionesY[6], 0),
  Defensa(posicionesX[7], posicionesY[7], 0),
  Defensa(posicionesX[8], posicionesY[8], 0),
  Defensa(posicionesX[9], posicionesY[9], 0),
};

const int NUM_DEFENSAS_MOVILES = 8;

Defensa defensasMoviles[NUM_DEFENSAS_MOVILES] = {
  Defensa(20, 60, 4, true, 20, 200, 0, 0),      // horizontal 1
  Defensa(120, 90, 24, true, 60, 220, 0, 0),    // horizontal 2
  Defensa(180, 130, 15, true, 140, 230, 0, 0),  // horizontal 3
  Defensa(60, 180, 30, true, 40, 190, 0, 0),    // horizontal 4 (último defensor)

  Defensa(30, 80, 42, false, 0, 0, 50, 200),    // vertical 1
  Defensa(90, 150, 18, false, 0, 0, 60, 210),   // vertical 2
  Defensa(160, 200, 12, false, 0, 0, 100, 260), // vertical 3
  Defensa(210, 110, 36, false, 0, 0, 40, 170)   // vertical 4
};

const int MAX_BALONES = 1;
Balon* balones[MAX_BALONES];
int numBalones = 0;

unsigned long lastUpdate = 0;  
const int frameDelay = 5;

int defensasEsquivadas = 0;
bool aparecePortero = false;
Portero portero;
Porteria porteria(60, 0); // Declaración global de la portería con posición fija

int melody[] = { 262, 294, 330, 349, 392, 440, 392, 349, 330, 294, 262, 294, 330, 262 };
int noteDurations[] = { 8, 8, 4, 8, 4, 4, 8, 8, 4, 4, 4, 8, 4, 2 };
int currentNote = 0;
unsigned long noteStartTime = 0;
bool musicaActiva = true;

void mostrarPantallaCarga() {
  screen.setTextColor(ILI9341_BLACK);
  screen.setTextSize(2);
  screen.setCursor(20, 50);
  screen.print("BIENVENIDO :)");
}

void pantallaPrincipal() {
  screen.fillScreen(ILI9341_GREEN);
  screen.setCursor(5, 80);
  screen.setTextColor(ILI9341_BLACK);
  screen.setTextSize(3);
  screen.print("GOAL RUNNER!");
}

void pantallaGanar() {
  musicaActiva = false;
  noTone(PIN_BUZZER);
  screen.fillScreen(ILI9341_BLACK);
  screen.setCursor(30, 80);
  screen.setTextColor(ILI9341_GREEN);
  screen.setTextSize(3);
  screen.print("¡YOU WIN!");
  while (true);
}

void configurarPines() {
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);

  pinMode(BTN_ARRIBA, INPUT_PULLUP);
  pinMode(BTN_ABAJO, INPUT_PULLUP);

  pinMode(BOTON_DISPARO, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);
}

void inicializarJuego1() {
  jugador.dibujar();
  for (int i = 0; i < NUM_DEFENSAS; i++) {
    defensas[i].dibujar();
  }
  
}

void inicializarJuego() {
  jugador.dibujar();
  for (int i = 0; i < NUM_DEFENSAS; i++) {
    defensas[i].dibujar();
  }
  
}

void reiniciarJuego() {
  aparecePortero = false;  // Oculta portero en nueva partida
  screen.fillScreen(ILI9341_GREEN);  // Limpia toda la pantalla

  jugador.reiniciar();               // Coloca al jugador en su posición inicial

  for (int i = 0; i < numBalones; i++) {
    balones[i]->reiniciar();         // Desactiva balones
  }
  numBalones = 0;

  inicializarJuego1();               // Dibuja jugador y defensores (sin portería)
}

bool colisiona(const Jugador& jugador, const Defensa& defensa) {
  return !(
    jugador.getX() + JUGADOR_WIDTH < defensa.getX() ||
    jugador.getX() > defensa.getX() + DEFENSA_WIDTH ||
    jugador.getY() + JUGADOR_HEIGHT < defensa.getY() ||
    jugador.getY() > defensa.getY() + DEFENSA_HEIGHT
  );
}

bool colisionBalonPortero(Balon& balon, Portero& portero) {
  int bx = balon.getX();
  int by = balon.getY();
  int px = portero.getX();
  int py = portero.getY();

  const int anchoPortero = 20;
  const int altoPortero = 20;

  return !(
    bx + 3 < px ||
    bx > px + anchoPortero ||
    by + 6 < py ||
    by > py + altoPortero
  );
}

bool balonEntraPorteria(Balon& balon) {
  int x = balon.getX();
  int y = balon.getY();
  return (x > 60 && x < 180 && y < 15);
}

bool balonFueraPantalla(Balon& balon) {
  int x = balon.getX();
  int y = balon.getY();
  return (x < 0 || x > 240 || y < 0 || y > 320);
}

bool etapa2 = false;

bool reiniciarNivel2 = false;

void reiniciosegundonivel() {
  // Borrar defensas móviles actuales
  for (int i = 0; i < NUM_DEFENSAS_MOVILES; i++) {
    defensasMoviles[i].borrar();
  }

  // Redefinir las defensas móviles con nuevas posiciones y velocidades
  defensasMoviles[0] = Defensa(0, 50, 2, true, 0, 200);      
  defensasMoviles[1] = Defensa(180, 80, 2, true, 0, 200);    
  defensasMoviles[2] = Defensa(60, 0, 2, false, 0, 0, 0, 200);  
  defensasMoviles[3] = Defensa(120, 120, 2, false, 0, 0, 0, 200); 

  // Dibujar defensas
  for (int i = 0; i < NUM_DEFENSAS_MOVILES; i++) {
    defensasMoviles[i].dibujar();
  }

  // Reiniciar jugador
  jugador.reiniciar();

  // Reiniciar balón
  balon = Balon();

  // Volver a la primera parte del segundo nivel
  etapa2 = false;
}

void segundonivel() {
  static bool iniciado = false;
  static bool jugadorEmpezoAMoverse = false;  // NUEVO: Para evitar activación inmediata
  static bool porteriaMostrada = false;       // NUEVO: Para evitar que se repita

  if (!iniciado || reiniciarNivel2) {
    jugador.reiniciar();
    iniciado = true;
    jugadorEmpezoAMoverse = false;
    porteriaMostrada = false;
    estadoNivel2 = JUGANDO_CON_DEFENSAS;
    reiniciarNivel2 = false;  // 
  }
  

  if (estadoNivel2 == JUGANDO_CON_DEFENSAS) {
    // Leer botones
    bool izquierdaPresionada = digitalRead(BTN_LEFT) == LOW;
    bool derechaPresionada = digitalRead(BTN_RIGHT) == LOW;
    bool arribaPresionada = digitalRead(BTN_ARRIBA) == LOW;
    bool abajoPresionada = digitalRead(BTN_ABAJO) == LOW;

    if (izquierdaPresionada || derechaPresionada || arribaPresionada || abajoPresionada) {
      jugadorEmpezoAMoverse = true;  // Marca que ya empezó
    }

    if (izquierdaPresionada) jugador.moverIzquierda();
    if (derechaPresionada) jugador.moverDerecha();
    if (arribaPresionada) jugador.moverArriba();
    if (abajoPresionada) jugador.moverAbajo();

    // Actualizar defensas móviles
    for (int i = 0; i < NUM_DEFENSAS_MOVILES; i++) {
      defensasMoviles[i].actualizar();
    }

    // Verificar colisión
    for (int d = 0; d < NUM_DEFENSAS_MOVILES; d++) {
      int jx = jugador.getX();
      int jy = jugador.getY();
      int jw = jugador.getAncho();
      int jh = jugador.getAlto();

      int dx = defensasMoviles[d].getX();
      int dy = defensasMoviles[d].getY();
      int dw = DEFENSA_WIDTH;
      int dh = DEFENSA_HEIGHT;

      if (!(jx + jw < dx || jx > dx + dw || jy + jh < dy || jy > dy + dh)) {
        reiniciosegundonivel();
        return;
      }
    }

    // Verificar si ya pasó al último defensor
    int yMinDefensa = 9999;
    for (int i = 0; i < NUM_DEFENSAS_MOVILES; i++) {
      if (defensasMoviles[i].getY() < yMinDefensa) {
        yMinDefensa = defensasMoviles[i].getY();
      }
    }

    if (jugadorEmpezoAMoverse && !porteriaMostrada && jugador.getY() < yMinDefensa - 10) {
      estadoNivel2 = MOSTRAR_PORTERIA;
      aparecePortero = true;
      porteriaMostrada = true;

      screen.fillScreen(ILI9341_GREEN);
      portero.dibujar(screen);
      porteria.dibujar(screen);
    }
  }
  else if (estadoNivel2 == MOSTRAR_PORTERIA) {
    // Borrar, mover y dibujar portero
    portero.borrar(screen);
    portero.mover();
    portero.dibujar(screen);
  
    // Dibuja fondo arriba (ajustar según tu diseño)
    screen.fillRect(60, 0, 120, 15, ILI9341_BLUE);
  
    // Dibuja portería
    porteria.dibujar(screen);
  
    // Dibuja jugador
    jugador.dibujar();
  
    // Disparo: solo si botón disparo presionado y balón no activo
    if (digitalRead(BOTON_DISPARO) == LOW && !balon.estaActiva()) {
      balon = Balon(jugador.getX() + jugador.getAncho(), jugador.getY() + jugador.getAlto() / 2);
    }
  
    // Si balón activo, mover, dibujar y checar colisiones
    if (balon.estaActiva()) {
      balon.mover();
      balon.dibujar(screen);
    
      // Colisiones y condiciones de reinicio o victoria
      if (colisionBalonPortero(balon, portero)) {
        reiniciosegundonivel();  
      } else if (balonEntraPorteria(balon)) {
        pantallaGanar();
      } else if (balonFueraPantalla(balon)) {
        reiniciosegundonivel();  
      }
    }
    
  }
}

void setup() {
  screen.begin();
  screen.setRotation(0);
  screen.fillScreen(ILI9341_GREEN);
  mostrarPantallaCarga();
  delay(1000);
  pantallaPrincipal();
  delay(1000);
  screen.fillScreen(ILI9341_GREEN);
  configurarPines();
  inicializarJuego();
}

void loop() {
  unsigned long now = millis();
  if (now - lastUpdate < frameDelay) return;
  lastUpdate = now;

  // Música
  if (musicaActiva) {
    int duration = 1000 / noteDurations[currentNote];
    if (now - noteStartTime > duration * 1.3) {
      tone(PIN_BUZZER, melody[currentNote], duration);
      noteStartTime = now;
      currentNote = (currentNote + 1) % 8;
    }
  }

  // Controles del jugador (movimiento)
  if (digitalRead(BTN_LEFT) == LOW) jugador.moverIzquierda();
  if (digitalRead(BTN_RIGHT) == LOW) jugador.moverDerecha();
  if (digitalRead(BTN_ARRIBA) == LOW) jugador.moverArriba();
  if (digitalRead(BTN_ABAJO) == LOW) jugador.moverAbajo();

  // Disparo de balón
  if (digitalRead(BOTON_DISPARO) == LOW) {
    if (numBalones == 0 && aparecePortero) {  // Solo puede disparar en etapa portero
      balones[0] = new Balon(jugador.getX() + JUGADOR_WIDTH / 2 - 1, jugador.getY() - 6);
      numBalones = 1;
    }
    delay(100);
  }

  if (aparecePortero) {
    // ETAPA DEL PORTERO

    // Borrar, mover y dibujar portero
    portero.borrar(screen);
    portero.mover();
    portero.dibujar(screen);

    // Dibuja fondo arriba (puedes ajustar color o quitar)
    screen.fillRect(60, 0, 120, 15, ILI9341_BLUE);

    // Dibuja jugador
    jugador.dibujar();

    // Dibuja portería
    porteria.dibujar(screen);

    // Mover, dibujar balones y checar colisiones
    for (int i = 0; i < numBalones; i++) {
      balones[i]->mover();
      balones[i]->dibujar(screen);

      if (colisionBalonPortero(*balones[i], portero)) {
      
        reiniciarJuego();
      } 
      else if (balonEntraPorteria(*balones[i])) {
        segundonivel();
      } 
      else if (balonFueraPantalla(*balones[i])) {

        reiniciarJuego();
      }

      // Checar colisión con defensas en esta etapa, si quieres que sigan ahí
      for (int d = 0; d < NUM_DEFENSAS; d++) {
        int bx = balones[i]->getX();
        int by = balones[i]->getY();
        int dx = defensas[d].getX();
        int dy = defensas[d].getY();

        const int anchoBalon = 16;
        const int altoBalon = 16;

        if (!(bx + anchoBalon < dx || bx > dx + DEFENSA_WIDTH ||
              by + altoBalon < dy || by > dy + DEFENSA_HEIGHT)) {
                reiniciarJuego();
        }
      }
    }

    // Limpiar balones inactivos
    for (int i = 0; i < numBalones; i++) {
      if (!balones[i]->estaActiva()) {
        delete balones[i];
        for (int j = i; j < numBalones - 1; j++) {
          balones[j] = balones[j + 1];
        }
        numBalones--;
        i--;
      }
    }
    return;  // Salir del loop para no procesar más lógica en esta iteración
  }

  // --- ETAPA DEFENSAS ---

  // Actualizar defensas
  for (int i = 0; i < NUM_DEFENSAS; i++) {
    defensas[i].actualizar();

    if (colisiona(jugador, defensas[i])) {
      reiniciarJuego();
    }
  }

  // Verificar si jugador sobrepasó al último defensor (el que tiene menor Y)
  int yMinDefensa = 9999;  // valor grande para empezar
  for (int i = 0; i < NUM_DEFENSAS; i++) {
    if (defensas[i].getY() < yMinDefensa) {
      yMinDefensa = defensas[i].getY();
    }
  }

  if (jugador.getY() < yMinDefensa - 10) {  
    aparecePortero = true;
    screen.fillScreen(ILI9341_GREEN);
    portero.dibujar(screen);
    porteria.dibujar(screen);
  }

}
