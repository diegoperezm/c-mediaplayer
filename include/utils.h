#ifndef UTILS_H
#define UTILS_H
#include <raylib.h>
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
 X(NUM_STATES)

#define EVENT_TABLE \
 X(event_play)      \
 X(event_pause)     \
 X(event_stop)      \
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
  X(EL_BTN_PLAY)      \
  X(EL_BTN_PAUSE)     \
  X(EL_BTN_STOP)      \
  X(EL_LABEL)         \
  X(EL_DROP_FILES)
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


int GuiGetStyle(int control, int property);

Color GetColor(unsigned int hexValue);

void DrawText(const char *text, int posX, int posY, int fontSize, Color color);

int GuiTextBox(Rectangle bounds, char *text, int textSize, bool editMode);

int GuiLabel(Rectangle bounds, const char *text);

int GuiButton(Rectangle bounds, const char *text);

int GuiToggleGroup(Rectangle bounds, const char *text, int *active);

int GuiScrollPanel(Rectangle bounds, const char *text, Rectangle content, Vector2 *scroll, Rectangle *view);

void update_state(media_player *media_player, Event event);

int (*return_map(const State state))[SIZE_ROWS][SIZE_COLS];

void grid_layout(media_player *media_player, GstElement *pipeline, char **file_paths, int file_path_counter);

GstElement* create_audio_pipeline(const char* filename);




#endif
