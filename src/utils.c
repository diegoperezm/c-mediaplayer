#include <raylib.h>
#include "../include/raygui.h"
#include "../include/style_cyber.h"
#include "../include/utils.h"

#define X(state) #state,
char *state_name[] = {STATE_TABLE};
#undef X

#define X(event) #event,
char *event_name[] = {EVENT_TABLE};
#undef X

#define X(element) #element,
char *element_list[] = {ELEMENT_LIST};
#undef X

State transition_table[NUM_STATES][NUM_EVENTS] = {
    /* event_play | event_pause |   event_stop  | event_prev     | event_next */
    [STATE_WAITING] = {STATE_PLAY, INVALID_STATE, INVALID_STATE, INVALID_STATE, INVALID_STATE},
    [STATE_PLAY] = {INVALID_STATE, STATE_PAUSE, STATE_STOP, STATE_PREV, STATE_NEXT},
    [STATE_PAUSE] = {STATE_PLAY, INVALID_STATE, STATE_STOP, STATE_PREV, STATE_NEXT},
    [STATE_STOP] = {STATE_PLAY, INVALID_STATE, INVALID_STATE, STATE_PREV, STATE_NEXT},
    [STATE_PREV] = {STATE_PLAY, INVALID_STATE, INVALID_STATE, INVALID_STATE, INVALID_STATE},
    [STATE_NEXT] = {STATE_PLAY, INVALID_STATE, INVALID_STATE, INVALID_STATE, INVALID_STATE}
};

void update_state(media_player *media_player, Event event) {
    State current_state = media_player->currentState;
    State next_state = transition_table[current_state][event] != INVALID_STATE
                           ? transition_table[current_state][event]
                           : current_state;

    media_player->currentState = next_state;
}

int (*return_map(media_player *media_player))[SIZE_ROWS][SIZE_COLS] {
    const State state = media_player->currentState;
    int static map[SIZE_ROWS][SIZE_COLS] = {0};

    int static map_state_waiting[SIZE_ROWS][SIZE_COLS] = {
        {EL_BLANK},
        {EL_BLANK, EL_DROP_FILES, EL_BLANK, EL_BLANK, EL_BLANK, EL_LYRICS},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK, EL_BTN_PREV, EL_BTN_PLAY, EL_BTN_STOP, EL_BTN_NEXT},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK, EL_LABEL},
        {EL_BLANK},
    };

    int static map_state_play[SIZE_ROWS][SIZE_COLS] = {
        {EL_BLANK},
        {EL_BLANK, EL_DROP_FILES, EL_BLANK, EL_BLANK, EL_BLANK, EL_LYRICS},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK, EL_BTN_PREV, EL_BTN_PAUSE, EL_BTN_STOP, EL_BTN_NEXT},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK, EL_LABEL},
        {EL_BLANK},
    };


    switch (state) {
        case STATE_WAITING:
            return &map_state_waiting;
        case STATE_PLAY:
            return &map_state_play;
        case STATE_PAUSE:
            return &map_state_waiting;
        case STATE_STOP:
            return &map_state_waiting;
        case STATE_PREV:
            return &map_state_waiting;
        case STATE_NEXT:
            return &map_state_waiting;
        default:
            return &map;
    }
}

void grid_layout(media_player *media_player, gpointer user_data, char **file_paths, int file_path_counter) {
    CustomData *data = (CustomData *) user_data;
    const float width = (float) GetScreenWidth();
    const float height = (float) GetScreenHeight();
    const float cell_width = width / GRID_COLS;
    const float cell_height = height / GRID_ROWS;

    const Color font_color = GetColor(GuiGetStyle(0, 2));
    const int font_size = (int) (cell_height / cell_width);

    const int (*map)[SIZE_ROWS][SIZE_COLS] = return_map(media_player);

    for (int row = 0; row < SIZE_ROWS; row++) {
        for (int col = 0; col < SIZE_COLS; col++) {
            const float cell_x = (float) col * cell_width;
            const float cell_y = (float) row * cell_height;
            const Rectangle cell = {cell_x, cell_y, cell_width, cell_height};

            Rectangle panel_bounds = {cell_x, cell_y, cell_width * 4, cell_height * 6};
            Rectangle lyrics_bounds = {cell_x, cell_y, cell_width * 6, cell_height * 7};

            Vector2 scroll = {0, 0};
            Rectangle content = {0, 0, 0, 0};
            Rectangle view = {0};

            switch ((*map)[row][col]) {
                case EL_BLANK:
                    break;
                case EL_LYRICS:
                    GuiScrollPanel(lyrics_bounds, "Lyrics", content, &scroll, &view);
                    break;
                case EL_DROP_FILES:
                    GuiScrollPanel(panel_bounds, "Files", content, &scroll, &view);
                    for (int i = 0; i < file_path_counter; i++) {
                        if (i % 2 == 0) {
                            DrawRectangle(
                                (int) panel_bounds.x,
                                (int) (panel_bounds.y + (cell_height / 2.0f) * ((float) i + 1)),
                                (int) panel_bounds.width,
                                (int) cell_height / 2,
                                Fade(LIGHTGRAY, 0.5f));
                        } else {
                            DrawRectangle(
                                (int) panel_bounds.x,
                                (int) (panel_bounds.y + (cell_height / 2.0f) * ((float) i + 1)),
                                (int) panel_bounds.width,
                                (int) cell_height / 2,
                                Fade(LIGHTGRAY, 0.3f));
                        }
                        DrawText(
                            GetFileName(file_paths[i]),
                            (int) (panel_bounds.x + (cell_height / 6)),
                            (int) (panel_bounds.y + (cell_height / 2) * ((float) i + 1) + cell_height / 6),
                            font_size,
                            YELLOW);
                    }
                    break;

                case EL_BTN_PLAY:
                    if (GuiButton((Rectangle){cell.x, cell.y, cell.width, cell.height}, "PLAY")) {
                        if (file_path_counter == 0) {
                            break;
                        }
                        if (data->pipeline) {
                            GstStateChangeReturn ret = gst_element_set_state(data->pipeline, GST_STATE_PLAYING);
                            if (ret == GST_STATE_CHANGE_FAILURE) {
                                g_printerr("Failed to resume playback\n");
                            } else {
                                g_print("Resuming playback\n");
                            }
                        } else {
                            data->source = gst_element_factory_make("uridecodebin", "source");
                            data->sink = gst_element_factory_make("autoaudiosink", "sink");
                            data->pipeline = gst_pipeline_new("audio-pipeline");

                            if (!data->source || !data->sink || !data->pipeline) {
                                g_printerr("Error creating GStreamer elements\n");
                                break;
                            }

                            gchar *uri = gst_filename_to_uri(file_paths[0], NULL);
                            if (!uri) {
                                g_printerr("Error converting path to URI\n");
                                break;
                            }

                            g_object_set(data->source, "uri", uri, NULL);
                            g_free(uri);

                            gst_bin_add_many(GST_BIN(data->pipeline), data->source, data->sink, NULL);
                            g_signal_connect(data->source, "pad-added", G_CALLBACK(pad_added_handler), user_data);

                            GstStateChangeReturn ret = gst_element_set_state(data->pipeline, GST_STATE_PLAYING);
                            if (ret == GST_STATE_CHANGE_FAILURE) {
                                g_printerr("Failed to start playback\n");
                            } else {
                                g_print("Starting playback: %s\n", file_paths[0]);
                            }
                        }

                        update_state(media_player, event_play);
                    }
                    break;

                case EL_BTN_PAUSE:
                    if (GuiButton((Rectangle){cell.x, cell.y, cell.width, cell.height},
                                  "PAUSE")) {
                        if (data->pipeline) {
                            gst_element_set_state(data->pipeline, GST_STATE_PAUSED);
                        }
                        update_state(media_player, event_pause);
                    }
                    break;

                case EL_BTN_STOP:
                    if (GuiButton((Rectangle){cell.x, cell.y, cell.width, cell.height},
                                  "STOP")) {
                        if (data->pipeline) {
                            gst_element_set_state(data->pipeline, GST_STATE_NULL);
                            gst_object_unref(data->pipeline);
                            data->pipeline = NULL;
                            data->source = NULL;
                            data->sink = NULL;
                        }

                        update_state(media_player, event_stop);
                    }
                    break;
                case EL_BTN_PREV:
                    if (GuiButton((Rectangle){cell.x, cell.y, cell.width, cell.height}, "PREV")) {
                        update_state(media_player, event_prev);
                        update_state(media_player, event_play);
                    }

                    break;

                case EL_BTN_NEXT:
                    if (GuiButton((Rectangle){cell.x, cell.y, cell.width, cell.height}, "NEXT")) {
                        update_state(media_player, event_next);
                        update_state(media_player, event_play);
                    }
                    break;

                case EL_LABEL:
                    DrawText(state_name[media_player->currentState],
                             (int) cell.x, (int) cell.y, font_size, font_color);
                    break;

                default:
                    return;
            }
        }
    }
}

void setup_raylib() {
    const int screen_w = 800;
    const int screen_h = 600;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screen_w, screen_h, "Media Player");
    SetTargetFPS(30);
    GuiLoadStyleCyber();
}

void cleanup_pipeline(CustomData *data) {
    if (data->pipeline) {
        gst_element_set_state(data->pipeline, GST_STATE_NULL);
        gst_object_unref(data->pipeline);
        data->pipeline = NULL;
        data->source = NULL; // These become invalid after pipeline cleanup
        data->sink = NULL;
    }
}

/*
static void pad_added_handler(GstElement *src, GstPad *new_pad, gpointer user_data) {
    CustomData *data = (CustomData *) user_data;

    if (!data || !GST_IS_ELEMENT(data->sink)) {
        g_printerr("pad_added_handler: sink no inicializado\n");
        return;
    }

    GstPad *sink_pad = gst_element_get_static_pad(data->sink, "sink");

    if (!GST_IS_PAD(sink_pad)) {
        g_printerr("pad_added_handler: sink_pad no obtenido\n");
        return;
    }

    if (gst_pad_is_linked(sink_pad)) {
        g_print("pad ya enlazado\n");
        gst_object_unref(sink_pad);
        return;
    }

    if (gst_pad_link(new_pad, sink_pad) != GST_PAD_LINK_OK) {
        g_printerr("pad_added_handler: no se pudo enlazar el pad\n");
    } else {
        g_print(" pad enlazado correctamente\n");
    }

    gst_object_unref(sink_pad);
}
*/
static void pad_added_handler(GstElement *src, GstPad *new_pad, gpointer user_data) {
    CustomData *data = (CustomData *) user_data;

    if (!data || !data->sink) {
        g_printerr("Invalid data in pad_added_handler\n");
        return;
    }

    GstCaps *new_pad_caps = gst_pad_get_current_caps(new_pad);
    if (!new_pad_caps) {
        return;
    }

    GstStructure *new_pad_struct = gst_caps_get_structure(new_pad_caps, 0);
    const gchar *new_pad_type = gst_structure_get_name(new_pad_struct);

    // Only link audio pads
    if (!g_str_has_prefix(new_pad_type, "audio/x-raw")) {
        gst_caps_unref(new_pad_caps);
        return;
    }

    gst_caps_unref(new_pad_caps);

    GstPad *sink_pad = gst_element_get_static_pad(data->sink, "sink");
    if (gst_pad_is_linked(sink_pad)) {
        gst_object_unref(sink_pad);
        return;
    }

    if (gst_pad_link(new_pad, sink_pad) != GST_PAD_LINK_OK) {
        g_printerr("Failed to link pads\n");
    }

    gst_object_unref(sink_pad);
}
