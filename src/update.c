#include <stdbool.h>
#include <stdio.h>
#include "../include/update.h"

#define X(state) #state,
char* state_name[] = { STATE_TABLE };
#undef X

#define X(event) #event,
char* event_name[] = { EVENT_TABLE };
#undef X

State transition_table[NUM_STATES][NUM_EVENTS] = {
  /*           EVT_PLAY       EVT_PAUSE       EVT_STOP      */
  [STATE_WAITING] = { STATE_PLAY, INVALID_STATE, INVALID_STATE },
  [STATE_PLAY] = { INVALID_STATE, STATE_PAUSE, STATE_STOP },
  [STATE_PAUSE] = { STATE_PLAY, INVALID_STATE, STATE_STOP },
  [STATE_STOP] = { STATE_PLAY, INVALID_STATE, INVALID_STATE }
};

void update(MediaPlayer* mediaPlayer, Event event) {
  bool depurar = false;
  State currentState = mediaPlayer->currentState;

  State nextState = transition_table[currentState][event];

  if (depurar) {
    printf("|%10s|%10s|%10s|\n", state_name[currentState], event_name[event],
           state_name[nextState]);
  }

  if (nextState != INVALID_STATE) {
    mediaPlayer->currentState = nextState;
    switch (nextState) {
    case STATE_WAITING:
      mediaPlayer->context.display = "WAITING";
      break;
    case STATE_PLAY:
      mediaPlayer->context.display = "PLAY";
      break;
    case STATE_PAUSE:
      mediaPlayer->context.display = "PAUSE";
      break;
    case STATE_STOP:
      mediaPlayer->context.display = "STOP";
      break;
    default:
      break;
    }
  }
}
