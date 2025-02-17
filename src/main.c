#include <raylib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../include/raygui.h"
#include "../include/update.h"

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

  MediaPlayer mediaPlayer = { .currentState = STATE_WAITING,
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
