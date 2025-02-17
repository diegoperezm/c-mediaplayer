#ifndef UPDATE_H
#define UPDATE_H
#define INVALID_STATE -1

#define STATE_TABLE                                                            \
  X(STATE_WAITING)                                                             \
  X(STATE_PLAY)                                                                \
  X(STATE_PAUSE)                                                               \
  X(STATE_STOP)                                                                \
  X(NUM_STATES)

#define EVENT_TABLE                                                            \
  X(event_play)                                                                \
  X(event_pause)                                                               \
  X(event_stop)                                                                \
  X(NUM_EVENTS)

#define X(state) state,
typedef enum { STATE_TABLE } State;
#undef X

#define X(event) event,
typedef enum { EVENT_TABLE } Event;
#undef X

extern char* state_name[];
extern char* event_name[];

extern State transition_table[NUM_STATES][NUM_EVENTS]; 

typedef struct {
  State currentState;
  struct {
    const char* display;
  } context;
} MediaPlayer;

void update(MediaPlayer* mediaPlayer, Event event);
#endif // !UPDATE_H
