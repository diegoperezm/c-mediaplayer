#include "include/raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define RAYGUI_IMPLEMENTATION
#include "event_table.h"
#include "include/raygui.h"
#include "state_table.h"

#define X(state) state,
typedef enum { STATE_TABLE } State;
#undef X

#define X(state) #state,
char *state_name[] = {STATE_TABLE};
#undef X

#define X(event) event,
typedef enum { EVENT_TABLE } Event;
#undef X

#define X(event) #event,
char *event_name[] = {EVENT_TABLE};
#undef X

typedef struct {
  int currentState;
  struct {
    const char *display;
  } context;
} MediaPlayer;

int transition_table[NUM_STATES][NUM_EVENTS] = {
    {PLAY, -1, -1},    // WAITING
    {-1, PAUSE, STOP}, // PLAY
    {PLAY, -1, STOP},  // PAUSE
    {PLAY, -1, -1}     // STOP
};

MediaPlayer *transition(MediaPlayer *mediaPlayer, Event event, bool depurar);
void update(MediaPlayer *mediaPlayer, Event event);

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

  MediaPlayer mediaPlayer = {.currentState = WAITING,
                             .context = {.display = "WAITING"}};

  while (!WindowShouldClose()) {

    if (IsKeyPressed(KEY_LEFT))
      update(&mediaPlayer, event_play);
    if (IsKeyPressed(KEY_RIGHT))
      update(&mediaPlayer, event_pause);
    if (IsKeyPressed(KEY_UP))
      update(&mediaPlayer, event_stop);
    // Begin
    BeginDrawing();
    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
    GuiPanel(Line, "Media Player");

    if (GuiButton(btnPlay, "play"))
      update(&mediaPlayer, event_play);

    if (GuiButton(btnPause, "pause"))
      update(&mediaPlayer, event_pause);

    if (GuiButton(btnStop, "stop"))
      update(&mediaPlayer, event_stop);

    DrawRectangleRec(textBox, LIGHTGRAY);
    DrawText(mediaPlayer.context.display, textBox.width + col, screenH / 3,
             20, BLACK);
    EndDrawing();
    // End
  }

  CloseWindow();
  return EXIT_SUCCESS;
}

MediaPlayer *transition(MediaPlayer *mediaPlayer, Event event, bool depurar) {
  int currentState = mediaPlayer->currentState;
  int input = event;
  int nextState = transition_table[currentState][input];

  if (depurar) {
    printf("|%10s|%10s|%10s|\n", state_name[currentState], event_name[input],
           state_name[nextState]);
  }

  if (nextState != -1) {
    mediaPlayer->currentState = nextState;
    switch (nextState) {
    case WAITING:
      mediaPlayer->context.display = "WAITING";
      break;
    case PLAY:
      mediaPlayer->context.display = "PLAY";
      break;
    case PAUSE:
      mediaPlayer->context.display = "PAUSE";
      break;
    case STOP:
      mediaPlayer->context.display = "STOP";
      break;
    }
  }

  return mediaPlayer;
}

void update(MediaPlayer *mediaPlayer, Event event) {
  mediaPlayer = transition(mediaPlayer, event, false);
}
