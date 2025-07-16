#include <raylib.h>
#include "../include/utils.h"
#include <stdlib.h>
#include <unistd.h>

//#include "../include/style_candy.h"
#ifdef __APPLE__
#include <TargetConditionals.h>
#endif


int main(int argc, char **argv) {
    const Color BGCOLOR = (Color){0, 34, 43, 255};
    media_player media_player = {.currentState = STATE_WAITING};

    char *file_paths[MAX_FILEPATH_RECORDED] = {0};

    for (int i = 0; i < MAX_FILEPATH_RECORDED; i++) {
        file_paths[i] = (char *) RL_CALLOC(MAX_FILEPATH_SIZE, 1);
    }

    gst_init(&argc, &argv);
    CustomData data = {.current_volume_level = 0.3f};
    setup_raylib();

    while (!WindowShouldClose()) {
        if (IsFileDropped()) {
            const FilePathList droppedFiles = LoadDroppedFiles();
            for (int i = 0, offset = data.file_path_counter; i < (int) droppedFiles.count; i++) {
                if (data.file_path_counter < (MAX_FILEPATH_RECORDED - 1)) {
                    TextCopy(file_paths[offset + i], droppedFiles.paths[i]);
                    data.file_path_counter++;
                }
            }
            UnloadDroppedFiles(droppedFiles);
        }


        BeginDrawing();
        ClearBackground(BGCOLOR);
        grid_layout(&media_player, &data, file_paths);
        EndDrawing();
    } // end: while
    if (data.pipeline) {
        gst_element_set_state(data.pipeline, GST_STATE_NULL);
        gst_object_unref(data.pipeline);
        data.pipeline = NULL;
        data.source = NULL;
        data.volume = NULL;
        data.sink = NULL;
    }

    gst_deinit();
    for (int i = 0; i < MAX_FILEPATH_RECORDED; i++) {
        RL_FREE(file_paths[i]);
    }

    // free???: GuiLoadStyleCyber()
    //  GuiLoadStyleDefault();

    CloseWindow();
    return EXIT_SUCCESS;
}
