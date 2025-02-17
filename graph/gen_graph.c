#include "../include/update.h"
#include "gen_graph.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "/opt/local/include/graphviz/cgraph.h"
#include "/opt/local/include/graphviz/gvc.h"

// TODO review code (chatGPT generated)
void gen_graph(void) {

  Agraph_t* g = agopen("example", Agdirected, NULL);

  Agnode_t* nodes[NUM_STATES];
  for (int i = 0; i < NUM_STATES; i++) {
    nodes[i] = agnode(g, state_name[i], 1);
  }

  for (int i = 0; i < NUM_STATES; i++) {
    for (int j = 0; j < NUM_EVENTS; j++) {
      if (transition_table[i][j] != -1) {
        Agnode_t* target = nodes[transition_table[i][j]];
        Agedge_t* e = agedge(g, nodes[i], target, NULL, 1);
        agset(e, "label", event_name[j]);
      }
    }
  }

  GVC_t* gvc;
  gvc = gvContext();

  if (gvLayout(gvc, g, "dot") != 0) {
    if (fprintf(stderr, "Error: Graph layout failed\n") < 0) {
      perror("fprintf failed");
    }
    gvFreeContext(gvc);
    agclose(g);
    return;
  }

  FILE* outfile = fopen("graph.png", "w");
  if (outfile) {
    gvRender(gvc, g, "png", outfile);
    if (printf(stderr, "Graph successfully written to graph.png\n") < 0) {
      perror("fprintf failed");
    }

  } else {
    printf("Error: Could not open file for writing\n");
  }

  gvFreeContext(gvc);
  agclose(g);
}

/* "digraph G {\n"
    "  \"STATE_WAITING\" -> \"STATE_PLAY\" [label=\"event_play\"]\n"
    "  \"STATE_PLAY\" -> \"STATE_PAUSE\" [label=\"event_pause\"]\n"
    "  \"STATE_PLAY\" -> \"STATE_STOP\" [label=\"event_stop\"]\n"
    "  \"STATE_PAUSE\" -> \"STATE_PLAY\" [label=\"event_play\"]\n"
    "  \"STATE_PAUSE\" -> \"STATE_STOP\" [label=\"event_stop\"]\n"
    "  \"STATE_STOP\" -> \"STATE_PLAY\" [label=\"event_play\"]\n"
    "}
  static char result[2048];
  char output[1024];


  for (int i = 0; i < NUM_STATES; i++) {
    for (int j = 0; j < NUM_EVENTS; j++) {
      if (transition_table[i][j] != -1) {
        snprintf(output + strlen(output), 1024 - strlen(output),
                 "  \"%s\" -> \"%s\" [label=\"%s\"]\n", state_name[i],
                 state_name[transition_table[i][j]], event_name[j]);
      }
    }
  }
  snprintf(result, sizeof(result), "digraph G {\n%s}\n", output);
  // Now generate the Graphviz PNG using the result
  FILE* fp = popen("dot -Tpng -o output.png", "w");
  if (fp == NULL) {
    fprintf(stderr, "Failed to run Graphviz (dot command).\n");
    return;
  }

  // Write the Graphviz DOT string to the process input
  if (fputs(result, fp) == EOF) {
    fprintf(stderr, "Error writing to Graphviz process.\n");
    if (fclose(fp) != 0) {
      fprintf(stderr, "Error closing Graphviz process.\n");
    }
    return;
  }

  // Close the process and check for errors
  if (fclose(fp) != 0) {
    fprintf(stderr, "Error closing Graphviz process.\n");
    return;
  }

  printf("Graphviz PNG generated as output.png\n");

*/
