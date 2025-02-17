#include "../include/update.h"
#include "gen_graph.h"
#include <stdbool.h>
#include <stdio.h>

void gen_graph(void) {
  puts("digraph G {");
  for (int i = 0; i < NUM_STATES; i++) {
    for (int j = 0; j < NUM_EVENTS; j++) {
      if (transition_table[i][j] != -1)
        printf("\"%s\" -> \"%s\" [label=\"%s\"]\n", state_name[i],
               state_name[transition_table[i][j]], event_name[j]);
    }
  }
  puts("}");
}
