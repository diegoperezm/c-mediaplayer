#include "../include/raygui.h"
#include "../include/style_cyber.h"
#include "../include/utils.h"
#include <raylib.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef __APPLE__
#include <TargetConditionals.h>
#endif

int main(int argc, char **argv) {
    const char *file = "../resources/Deified-TheGreyRoom_Density&Time.mp3";
    gst_init(&argc, &argv);
    GstElement *pipeline = create_audio_pipeline(file);
    if (!pipeline) return -1;

    int screen_w = 800;
    int screen_h = 600;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screen_w, screen_h, "Media Player");

    SetTargetFPS(30);
    GuiLoadStyleCyber();
    const Color BGCOLOR = (GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
    media_player media_player = {.currentState = STATE_WAITING};

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BGCOLOR);
        grid_layout(&media_player, pipeline);
        EndDrawing();
    }

    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);

    CloseWindow();
    return EXIT_SUCCESS;
}
