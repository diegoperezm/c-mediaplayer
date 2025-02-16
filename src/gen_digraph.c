/*
#include "event_table.h"
#include "state_table.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define X(state) state,
typedef enum { STATE_TABLE } State;
#undef X

#define X(state) #state,
char *state_name[] = {STATE_TABLE};
#undef X

#define X(event) event,
typedef enum { EVENT_TABLE } Event;
#undef X

#define X(event) #event,
char *event_name[] = {EVENT_TABLE};
#undef X

int transiciones[NUM_STATES][NUM_EVENTS] = {
    {PLAY, -1, -1},    // WAITING
    {-1, PAUSE, STOP}, // PLAY
    {PLAY, -1, STOP},  // PAUSE
    {PLAY, -1, -1}     // STOP
};

void generarDigraph(void);
int main(void) { generarDigraph(); }

void generarDigraph(void) {
  puts("digraph G {");
  for (int i = 0; i < NUM_STATES; i++) {
    for (int j = 0; j < NUM_EVENTS; j++) {
      if (transiciones[i][j] != -1)
        printf("\"%s\" -> \"%s\" [label=\"%s\"]\n", state_name[i],
               state_name[transiciones[i][j]], event_name[j]);
    }
  }
  puts("}");
}
*/
