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
    /* event_play | event_pause | event_stop */
    [STATE_WAITING] = {STATE_PLAY, INVALID_STATE, INVALID_STATE},
    [STATE_PLAY] = {INVALID_STATE, STATE_PAUSE, STATE_STOP},
    [STATE_PAUSE] = {STATE_PLAY, INVALID_STATE, STATE_STOP},
    [STATE_STOP] = {STATE_PLAY, INVALID_STATE, INVALID_STATE}
};

void update_state(media_player *media_player_ps, Event event) {
    State current_state = media_player_ps->currentState;
    State next_state = transition_table[current_state][event] != INVALID_STATE
                           ? transition_table[current_state][event]
                           : current_state;

    media_player_ps->currentState = next_state;
}

int (*return_map(const State state))[SIZE_ROWS][SIZE_COLS] {
    int static map[SIZE_ROWS][SIZE_COLS] = {0};
    int static map_state_waiting[SIZE_ROWS][SIZE_COLS] = {
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK, EL_BLANK, EL_BLANK, EL_BLANK, EL_LABEL},
        {EL_BLANK, EL_BLANK, EL_BLANK, EL_BLANK, EL_BTN_PLAY, EL_BTN_PAUSE, EL_BTN_STOP},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK},
        {EL_BLANK},
    };


    switch (state) {
        case STATE_WAITING:
            return &map_state_waiting;
        case STATE_PLAY:
            return &map_state_waiting;
        case STATE_PAUSE:
            return &map_state_waiting;
        case STATE_STOP:
            return &map_state_waiting;
        default:
            return &map;
    }
}

void grid_layout(media_player *media_player) {
    const float width = (float) GetScreenWidth();
    const float height = (float) GetScreenHeight();
    const float cell_width = width / GRID_COLS;
    const float cell_height = height / GRID_ROWS;

    const Color font_color = GetColor(GuiGetStyle(0, 2));
    const int font_size = (int)(cell_height/cell_width);

    const int (*map)[SIZE_ROWS][SIZE_COLS] = return_map(media_player->currentState);

    for (int row = 0; row < SIZE_ROWS; row++) {
        for (int col = 0; col < SIZE_COLS; col++) {
            const float cell_x = (float) col * cell_width;
            const float cell_y = (float) row * cell_height;
            const Rectangle cell = {cell_x, cell_y, cell_width, cell_height};

            switch ((*map)[row][col]) {
                case EL_BLANK:
                    break;

                case EL_BTN_PLAY:
                    if (GuiButton((Rectangle){cell.x, cell.y, cell.width, cell.height},
                                  "PLAY")) {
                        update_state(media_player, event_play);
                    }
                    break;

                case EL_BTN_PAUSE:
                    if (GuiButton((Rectangle){cell.x, cell.y, cell.width, cell.height},
                                  "PAUSE")) {
                        update_state(media_player, event_pause);
                    }
                    break;

                case EL_BTN_STOP:
                    if (GuiButton((Rectangle){cell.x, cell.y, cell.width, cell.height},
                                  "STOP")) {
                        update_state(media_player, event_stop);
                    }
                    break;

                case EL_LABEL:
                   DrawText(state_name[media_player->currentState],
                       (int)cell.x, (int)cell.y , font_size, font_color);
                    break;

                default:
                    return;
            }
        }
    }
}
