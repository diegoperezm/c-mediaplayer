#include "../include/raygui.h"
#include "../include/style_cyber.h"
#include "../include/utils.h"
#include <raylib.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>

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

    char *file_paths[MAX_FILEPATH_RECORDED] =  {0};
    int file_path_counter = 0;

    for(int i = 0; i < MAX_FILEPATH_RECORDED; i++){
      file_paths[i] =  (char *) RL_CALLOC(MAX_FILEPATH_SIZE,1);
    }

    while (!WindowShouldClose()) {

      if(IsFileDropped()) {
        FilePathList droppedFiles = LoadDroppedFiles(); 
        for(int i = 0 , offset = file_path_counter; i < (int)droppedFiles.count; i++) {
           if(file_path_counter < (MAX_FILEPATH_RECORDED - 1)) {
               TextCopy(file_paths[offset+i], droppedFiles.paths[i]);
               file_path_counter++;
            }
         }
          UnloadDroppedFiles(droppedFiles);
       }


      BeginDrawing();
        ClearBackground(BGCOLOR);
        grid_layout(&media_player, pipeline, file_paths, file_path_counter);
      EndDrawing();
    } // end: while

    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);

    for(int i=0; i < MAX_FILEPATH_RECORDED; i++){
       RL_FREE(file_paths[i]);
    }

    // free???: GuiLoadStyleCyber()
   //  GuiLoadStyleDefault();

    CloseWindow();
    return EXIT_SUCCESS;
}





