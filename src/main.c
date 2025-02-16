#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define RAYGUI_IMPLEMENTATION
#include "../include/event_table.h"
#include "../include/raygui.h"
#include "../include/state_table.h"
#define INVALID_STATE -1

#define X(state) state,
typedef enum { STATE_TABLE } State;
#undef X

#define X(state) #state,
char* state_name[] = { STATE_TABLE };
#undef X

#define X(event) event,
typedef enum { EVENT_TABLE } Event;
#undef X

#define X(event) #event,
char* event_name[] = { EVENT_TABLE };
#undef X

typedef struct {
  State currentState;
  struct {
    const char* display;
  } context;
} MediaPlayer;

MediaPlayer* transition(MediaPlayer* mediaPlayer, Event event, bool depurar);

State transition_table[NUM_STATES][NUM_EVENTS] = {
  /*           EVT_PLAY       EVT_PAUSE       EVT_STOP      */
  [WAITING] = { PLAY, INVALID_STATE, INVALID_STATE }, // WAITING
  [PLAY] = { INVALID_STATE, PAUSE, STOP },            // PLAY
  [PAUSE] = { PLAY, INVALID_STATE, STOP },            // PAUSE
  [STOP] = { PLAY, INVALID_STATE, INVALID_STATE }     // STOP
};

void update(MediaPlayer* mediaPlayer, Event event);

int main(void) {
  float screenW = 600.0F;
  float screenH = 600.0F;
  float col = screenW / 24.0F;
  float btnSize = col * 4.0F;

  InitWindow((int)screenW, (int)screenH, "Media Player");

  Rectangle btnPlay = { col * 5.0F, screenH / 2.0F, btnSize, btnSize };
  Rectangle btnPause = { col * 9.5F, screenH / 2.0F, btnSize, btnSize };
  Rectangle btnStop = { col * 14.0F, screenH / 2.0F, btnSize, btnSize };
  Rectangle Line = { col * 4.5F, screenH / 7.0F, btnSize * 3.5F,
                     btnSize * 3.2F };
  Rectangle textBox = { screenW / 2.0F - 100, 180, 225, 50 };

  SetTargetFPS(20);

  MediaPlayer mediaPlayer = { .currentState = WAITING,
                              .context = { .display = "WAITING" } };

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
    DrawText(mediaPlayer.context.display, (int)(textBox.width + col),
             (int)(screenH / 3), 20, BLACK);
    EndDrawing();
    // End
  }

  CloseWindow();
  return EXIT_SUCCESS;
}

MediaPlayer* transition(MediaPlayer* mediaPlayer, Event event, bool depurar) {
  State currentState = mediaPlayer->currentState;
  int input = event;

  // int nextState = transition_table[currentState][input];
  State nextState = transition_table[currentState][input];

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
    default:
      break;
    }
  }

  return mediaPlayer;
}

void update(MediaPlayer* mediaPlayer, Event event) {
  mediaPlayer = transition(mediaPlayer, event, false);
}
