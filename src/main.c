#include <raylib.h>
#include "../include/utils.h"
#include <stdlib.h>
#include <unistd.h>
#include <curl/curl.h>

//#include "../include/style_candy.h"
#ifdef __APPLE__
#include <TargetConditionals.h>
#endif

typedef struct {
    char *ptr;
    size_t len;
} String;

void init_string(String *s) {
    s->len = 0;
    s->ptr = malloc(s->len + 1);
    if (s->ptr == NULL) {
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}

size_t write_function(void *ptr, size_t size, size_t nmemb, String *s) {
    size_t new_len = s->len + size * nmemb;
    s->ptr = realloc(s->ptr, new_len + 1);
    if (s->ptr == NULL) {
        fprintf(stderr, "realloc() failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(s->ptr + s->len, ptr, size * nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;
    return size * nmemb;
}


int main(int argc, char **argv) {
    const Color BGCOLOR = (Color){0, 34, 43, 255};
    media_player media_player = {.currentState = STATE_WAITING};

    char *file_paths[MAX_FILEPATH_RECORDED] = {0};

    for (int i = 0; i < MAX_FILEPATH_RECORDED; i++) {
        file_paths[i] = (char *) RL_CALLOC(MAX_FILEPATH_SIZE, 1);
    }

    gst_init(&argc, &argv);
    CustomData data = {0};

    setup_raylib();

    CURL *handle = curl_easy_init();
    String response;
    init_string(&response);

    if (handle) {
        curl_easy_setopt(handle, CURLOPT_URL, "http://example.com/");
//        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_function);
 //       curl_easy_setopt(handle, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(handle);
  //      if (res == CURLE_OK) {
            printf("Respose: %s\n", response.ptr);
   //     } else {
     //       printf("curl_easy_perform() failes: %s\n", curl_easy_strerror(res));
    //    }

        curl_easy_cleanup(handle);
        free(response.ptr);
    }

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
