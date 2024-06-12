#include "include/raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#define RAYGUI_IMPLEMENTATION
#include "include/raygui.h"

typedef struct {
  int estadoActual;
  struct {
    const char *pantalla;
  } contexto;
} MediaPlayer;

typedef enum { WAITING, PLAY, PAUSE, STOP, NUM_STATES } State;

typedef enum { event_play, event_pause, event_stop, NUM_EVENTS } Evento;

int transiciones[NUM_STATES][NUM_EVENTS] = {
    {PLAY, -1, -1},    // WAITING
    {-1, PAUSE, STOP}, // PLAY
    {PLAY, -1, STOP},  // PAUSE
    {PLAY, -1, -1}     // STOP
};

MediaPlayer* transicion(MediaPlayer* mediaPlayer, Evento evento, bool depurar);
void actualizar(MediaPlayer* mediaPlayer, Evento evento);

int main(void) {
  float screenW = 600.0f;
  float screenH = 600.0f;
  float col = screenW / 24.0f;
  float btnSize = col * 4;

  InitWindow(screenW, screenH, "Media Player");

  Rectangle btnPlay = {col * 5.0f, screenH / 2.0f, btnSize, btnSize};
  Rectangle btnPause = {col * 9.5f, screenH / 2.0f, btnSize, btnSize};
  Rectangle btnStop = {col * 14.0f, screenH / 2.0f, btnSize, btnSize};
  Rectangle Line = {col * 4.5f, screenH / 7.0f, btnSize * 3.5f, btnSize * 3.2f};
  Rectangle textBox = {screenW / 2.0f - 100, 180, 225, 50};

  SetTargetFPS(20);

  MediaPlayer mediaPlayer = {.estadoActual = WAITING,
                             .contexto = {.pantalla = "WAITING"}};

  while (!WindowShouldClose()) {

    if (IsKeyPressed(KEY_LEFT))
      actualizar(&mediaPlayer, event_play);
    if (IsKeyPressed(KEY_RIGHT))
      actualizar(&mediaPlayer, event_pause);
    if (IsKeyPressed(KEY_UP))
      actualizar(&mediaPlayer, event_stop);
// Begin
    BeginDrawing();
    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
    GuiPanel(Line, "Media Player");

    if (GuiButton(btnPlay, "play"))
      actualizar(&mediaPlayer, event_play);

    if (GuiButton(btnPause, "pause"))
      actualizar(&mediaPlayer, event_pause);

    if (GuiButton(btnStop, "stop"))
      actualizar(&mediaPlayer, event_stop);

    DrawRectangleRec(textBox, LIGHTGRAY);
    DrawText(mediaPlayer.contexto.pantalla, textBox.width + col, screenH / 3,
             20, BLACK);
    EndDrawing();
// End 

  }

  CloseWindow();
  return EXIT_SUCCESS;
}

MediaPlayer* transicion(MediaPlayer *mediaPlayer, Evento evento, bool depurar) {
  int estado = mediaPlayer->estadoActual;
  int entrada = evento;
  int estadoSiguiente = transiciones[estado][entrada];

  if (depurar) {
    const char *estadosNombres[] = {"WAITING", "PLAY", "PAUSE", "STOP"};
    const char *eventosNombres[] = {"event_play", "event_pause", "event_stop"};

    printf("|%10s|%10s|%10s|\n", estadosNombres[estado],
           eventosNombres[entrada], estadosNombres[estadoSiguiente]);
  }

  if (estadoSiguiente != -1) {
    mediaPlayer->estadoActual = estadoSiguiente;
    switch (estadoSiguiente) {
    case WAITING:
      mediaPlayer->contexto.pantalla = "WAITING";
      break;
    case PLAY:
      mediaPlayer->contexto.pantalla = "PLAY";
      break;
    case PAUSE:
      mediaPlayer->contexto.pantalla = "PAUSE";
      break;
    case STOP:
      mediaPlayer->contexto.pantalla = "STOP";
      break;
    }
  }

  return mediaPlayer;
}

void actualizar(MediaPlayer* mediaPlayer, Evento evento) {
  mediaPlayer = transicion(mediaPlayer, evento, false);
}
