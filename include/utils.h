#ifndef UTILS_H
#define UTILS_H
#include <gst/gst.h>

#define GRID_COLS 12.0F
#define GRID_ROWS 12.0F
#define GRID_PADDING 10.0F
#define GRID_PADDING 10.0F
#define CELL_MARGIN 12.0F

#define SIZE_COLS 12
#define SIZE_ROWS 12

#define MAX_FILEPATH_RECORDED 4096
#define MAX_FILEPATH_SIZE     2048

#define  STATE_TABLE \
 X(INVALID_STATE)    \
 X(STATE_WAITING)    \
 X(STATE_PLAY)       \
 X(STATE_PAUSE)      \
 X(STATE_STOP)       \
 X(STATE_PREV)       \
 X(STATE_NEXT)       \
 X(NUM_STATES)

#define EVENT_TABLE \
 X(event_play)      \
 X(event_pause)     \
 X(event_stop)      \
 X(event_prev)      \
 X(event_next)      \
 X(NUM_EVENTS)

#define X(state) state,

typedef enum {
    STATE_TABLE
} State;
#undef X

#define X(event) event,

typedef enum {
    EVENT_TABLE
} Event;
#undef X

#define ELEMENT_LIST \
  X(EL_BLANK)        \
  X(EL_BTN_PLAY)     \
  X(EL_BTN_PAUSE)    \
  X(EL_BTN_STOP)     \
  X(EL_LABEL)        \
  X(EL_DROP_FILES)   \
  X(EL_BTN_PREV)     \
  X(EL_BTN_NEXT)     \
  X(EL_PROGRESS_BAR) \
  X(EL_VOLUME_SLIDER)   \
  X(EL_LYRICS)

#define X(element) element,

typedef enum {
    ELEMENT_LIST
} Element;
#undef X

typedef struct {
    State currentState;

    struct {
        const char *display;
    } context;
} media_player;


typedef struct {
    GstElement *pipeline;
    GstElement *source;
    GstElement *volume;
    GstElement *sink;
    float current_volume_level;
    int file_path_counter;
    int current_track_index;
    gint64 position;
    gint64 duration;
    float current_position_track;
    float min_len_track;
    float total_len_track;
    float min_len_volume;
    float max_len_volume;
} CustomData;


void setup_raylib();

static void pad_added_handler(GstElement *src, GstPad *new_pad, gpointer user_data);

void update_state(media_player *media_player, Event event);

int (*return_map(media_player *media_player))[SIZE_ROWS][SIZE_COLS];

void grid_layout(media_player *media_player, gpointer user_data, char **file_paths);

void load_and_play_track(CustomData *data, char **file_paths);

#endif
