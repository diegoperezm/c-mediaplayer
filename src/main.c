#include "../include/raygui.h"
#include "../include/style_cyber.h"
#include "../include/utils.h"
#include <raylib.h>
#include <stdlib.h>

int main(void) {
    int screen_w = 800;
    int screen_h = 600;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screen_w, screen_h, "Media Player");
    SetTargetFPS(30);
    GuiLoadStyleCyber();
    media_player media_player = {.currentState = STATE_WAITING};

    while (!WindowShouldClose()) {
        // Begin
        BeginDrawing();
        ClearBackground(
            GetColor((unsigned int) GuiGetStyle(DEFAULT, BACKGROUND_COLOR))
        );
        grid_layout(&media_player);
        EndDrawing();
        // End
    }
    CloseWindow();
    return EXIT_SUCCESS;
}
