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


// @formatter:off
State transition_table[NUM_STATES][NUM_EVENTS] = {
                     /*event_play    |event_pause |  event_stop   | event_prev   | event_next */
    [STATE_WAITING] = {STATE_PLAY,    INVALID_STATE, INVALID_STATE, INVALID_STATE, INVALID_STATE},
    [STATE_PLAY]    = {INVALID_STATE, STATE_PAUSE,   STATE_STOP,    STATE_PREV,    STATE_NEXT},
    [STATE_PAUSE]   = {STATE_PLAY,    INVALID_STATE, STATE_STOP,    STATE_PREV,    STATE_NEXT},
    [STATE_STOP]    = {STATE_PLAY,    INVALID_STATE, INVALID_STATE, STATE_PREV,    STATE_NEXT},
    [STATE_PREV]    = {STATE_PLAY,    INVALID_STATE, INVALID_STATE, INVALID_STATE, INVALID_STATE},
    [STATE_NEXT]    = {STATE_PLAY,    INVALID_STATE, INVALID_STATE, INVALID_STATE, INVALID_STATE}
};
// @formatter:off

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
        {EL_DROP_FILES, EL_BLANK, EL_BLANK, EL_BLANK, EL_LYRICS},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BTN_PREV, EL_BTN_PLAY, EL_BTN_STOP, EL_BTN_NEXT, EL_PROGRESS_BAR},
    };

    int static map_state_play[SIZE_ROWS][SIZE_COLS] = {
        {EL_DROP_FILES, EL_BLANK, EL_BLANK, EL_BLANK, EL_LYRICS},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BTN_PREV, EL_BTN_PAUSE, EL_BTN_STOP, EL_BTN_NEXT, EL_PROGRESS_BAR},
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


void grid_layout(media_player *media_player, gpointer user_data, char **file_paths) {
    CustomData *data = (CustomData *) user_data;
    const float width = (float) GetScreenWidth();
    const float height = (float) GetScreenHeight();
    const float cell_width = width / GRID_COLS;
    const float cell_height = height / GRID_ROWS;

    const Color font_color = GetColor(GuiGetStyle(0, 2));
    const int font_size = (int) (cell_height / cell_width);

    gint64 position;
    gint64 duration;
    float current_position_track = 0.0f;
    float total_len_track = 1.0f;

    const int (*map)[SIZE_ROWS][SIZE_COLS] = return_map(media_player);

    for (int row = 0; row < SIZE_ROWS; row++) {
        for (int col = 0; col < SIZE_COLS; col++) {
            const float cell_x = (float) col * cell_width;
            const float cell_y = (float) row * cell_height;
            const Rectangle cell = {cell_x, cell_y, cell_width, cell_height};

            Rectangle drop_files_bounds = {cell_x, cell_y, cell_width * 4, cell_height * 11};
            Rectangle lyrics_bounds = {cell_x, cell_y, cell_width * 8, cell_height * 11};
            Rectangle progress_bar_bounds = {cell_x, cell_y, cell_width * 8, cell_height / 2};
            Rectangle control_btn_bounds = (Rectangle){cell.x, cell.y, cell.width, cell.height / 2};
            Vector2 scroll = {0, 0};
            Rectangle content = {0, 0, 0, 0};
            Rectangle view = {0};

            switch ((*map)[row][col]) {
                case EL_BLANK:
                    break;
                case EL_PROGRESS_BAR:
                    if (data->pipeline) {
                        if (gst_element_query_position(data->pipeline, GST_FORMAT_TIME, &position)) {
                            current_position_track = (float) position / GST_SECOND;
                        }
                        if (gst_element_query_duration(data->pipeline, GST_FORMAT_TIME, &duration)) {
                            total_len_track = (float) duration / GST_SECOND;
                        }
                    }

                    GuiProgressBar(progress_bar_bounds, NULL, NULL, &current_position_track, 0, total_len_track);
                    break;
                case EL_LYRICS:
                    GuiScrollPanel(lyrics_bounds, "Lyrics", content, &scroll, &view);
                    break;
                case EL_DROP_FILES:
                    GuiScrollPanel(drop_files_bounds, "Files", content, &scroll, &view);
                    for (int i = 0; i < data->file_path_counter; i++) {
                        if (data->current_track_index == i) {
                            DrawText(
                                GetFileName(file_paths[i]),
                                (int) (drop_files_bounds.x + (cell_height / 6)),
                                (int) (drop_files_bounds.y + (cell_height / 2) * ((float) i + 1) + cell_height / 6),
                                font_size,
                                YELLOW);
                        } else if (i % 2 == 0 && data->current_track_index != i) {
                            DrawRectangle(
                                (int) drop_files_bounds.x,
                                (int) (drop_files_bounds.y + (cell_height / 2.0f) * ((float) i + 1)),
                                (int) drop_files_bounds.width,
                                (int) cell_height / 2,
                                Fade(LIGHTGRAY, 0.5f));
                            DrawText(
                                GetFileName(file_paths[i]),
                                (int) (drop_files_bounds.x + (cell_height / 6)),
                                (int) (drop_files_bounds.y + (cell_height / 2) * ((float) i + 1) + cell_height / 6),
                                font_size,
                                WHITE);
                        } else {
                            DrawRectangle(
                                (int) drop_files_bounds.x,
                                (int) (drop_files_bounds.y + (cell_height / 2.0f) * ((float) i + 1)),
                                (int) drop_files_bounds.width,
                                (int) cell_height / 2,
                                Fade(LIGHTGRAY, 0.3f));
                            DrawText(
                                GetFileName(file_paths[i]),
                                (int) (drop_files_bounds.x + (cell_height / 6)),
                                (int) (drop_files_bounds.y + (cell_height / 2) * ((float) i + 1) + cell_height / 6),
                                font_size,
                                WHITE);
                        }
                    }
                    break;

                case EL_BTN_PLAY:
                    if (GuiButton(control_btn_bounds, ">")) {
                        if (data->file_path_counter == 0) {
                            break;
                        }
                        if (data->pipeline) {
                            GstStateChangeReturn ret = gst_element_set_state(data->pipeline, GST_STATE_PLAYING);
                            if (ret == GST_STATE_CHANGE_FAILURE) {
                                g_printerr("Failed to resume playback\n");
                                gst_element_set_state(data->pipeline, GST_STATE_NULL);
                                gst_object_unref(data->pipeline);
                                data->pipeline = NULL;
                                data->source = NULL;
                                data->sink = NULL;
                            } else {
                                g_print("Resuming playback\n");
                            }
                        } else {
                            load_and_play_track(user_data, file_paths);
                        }
                        update_state(media_player, event_play);
                    }
                    break;

                case EL_BTN_PAUSE:
                    if (GuiButton(control_btn_bounds, "||")) {
                        if (data->pipeline) {
                            gst_element_set_state(data->pipeline, GST_STATE_PAUSED);
                        }
                        update_state(media_player, event_pause);
                    }
                    break;

                case EL_BTN_STOP:
                    if (GuiButton(control_btn_bounds,
                                  "[]")) {
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
                    if (GuiButton(control_btn_bounds, "<<")) {
                        if (data->file_path_counter > 0) {
                            data->current_track_index =
                                    (data->current_track_index - 1 + data->file_path_counter) % data->file_path_counter;
                        }
                        load_and_play_track(data, file_paths);
                        update_state(media_player, event_prev);
                        update_state(media_player, event_play);
                    }

                    break;

                case EL_BTN_NEXT:
                    if (GuiButton(control_btn_bounds, ">>")) {
                        if (data->file_path_counter > 0) {
                            data->current_track_index = (data->current_track_index + 1) % data->file_path_counter;
                        }
                        load_and_play_track(data, file_paths);
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

static void pad_added_handler(GstElement *src, GstPad *new_pad, gpointer user_data) {
    CustomData *data = (CustomData *) user_data;

    if (!data || !data->sink) {
        g_printerr("Invalid data in pad_added_handler\n");
        return;
    }
    GstPad *sink_pad = gst_element_get_static_pad(data->sink, "sink");
    if (!sink_pad) {
        g_printerr("Failed to get sink pad\n");
        return;
    }

    if (gst_pad_is_linked(sink_pad)) {
        g_print("Sink pad already linked\n");
        gst_object_unref(sink_pad);
        return;
    }

    GstCaps *new_pad_caps = gst_pad_get_current_caps(new_pad);
    if (!new_pad_caps) {
        new_pad_caps = gst_pad_query_caps(new_pad, NULL);
    }

    if (!new_pad_caps) {
        g_printerr("Failed to get pad capabilities\n");
        gst_object_unref(sink_pad);
        return;
    }

    GstStructure *new_pad_struct = gst_caps_get_structure(new_pad_caps, 0);
    const gchar *new_pad_type = gst_structure_get_name(new_pad_struct);

    gboolean is_audio = g_str_has_prefix(new_pad_type, "audio/x-raw") ||
                        g_str_has_prefix(new_pad_type, "audio/");


    if (!is_audio) {
        g_print("Skipping non-audio pad: %s\n", new_pad_type);
        gst_caps_unref(new_pad_caps);
        gst_object_unref(sink_pad);
        return;
    }

    gst_caps_unref(new_pad_caps);

    GstPadLinkReturn link_result = gst_pad_link(new_pad, sink_pad);
    if (link_result != GST_PAD_LINK_OK) {
        g_printerr("Failed to link pads: %d\n", link_result);
    } else {
        g_print("Successfully linked audio pad: %s\n", new_pad_type);
    }
    gst_object_unref(sink_pad);
}

void load_and_play_track(CustomData *data, char **file_paths) {
    if (data->pipeline) {
        gst_element_set_state(data->pipeline, GST_STATE_NULL);
        gst_object_unref(data->pipeline);
        data->pipeline = NULL;
        data->source = NULL;
        data->sink = NULL;
    }

    data->source = gst_element_factory_make("uridecodebin", "source");
    data->sink = gst_element_factory_make("autoaudiosink", "sink");
    data->pipeline = gst_pipeline_new("audio-pipeline");

    if (!data->source || !data->sink || !data->pipeline) {
        g_printerr("Error creating GStreamer elements for track %s\n",
                   file_paths[data->current_track_index]);

        if (data->source) {
            gst_object_unref(data->source);
            data->source = NULL;
        }
        if (data->sink) {
            gst_object_unref(data->sink);
            data->sink = NULL;
        }
        if (data->pipeline) {
            gst_object_unref(data->pipeline);
            data->pipeline = NULL;
        }
        return;
    }

    gchar *uri = gst_filename_to_uri(file_paths[data->current_track_index], NULL);
    if (!uri) {
        g_printerr("Error converting path to uri for track %d\n", data->current_track_index);
        gst_object_unref(data->pipeline);
        data->pipeline = NULL;
        data->source = NULL;
        data->sink = NULL;
        return;
    }

    g_object_set(data->source, "uri", uri, NULL);
    g_free(uri);

    gst_bin_add_many(GST_BIN(data->pipeline), data->source, data->sink, NULL);
    g_signal_connect(data->source, "pad-added", G_CALLBACK(pad_added_handler), data);

    GstStateChangeReturn ret = gst_element_set_state(data->pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr("Failed to play track %d\n", data->current_track_index);
        gst_element_set_state(data->pipeline, GST_STATE_NULL);
        gst_object_unref(data->pipeline);
        data->pipeline = NULL;
        data->source = NULL;
        data->sink = NULL;
    } else {
        g_print("Playing track %d: %s\n", data->current_track_index,
                GetFileName(file_paths[data->current_track_index]));
    }
}
