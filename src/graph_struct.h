
#ifndef _GRAPH_STRUCT_
#define _GRAPH_STRUCT_

#include "graph.h"

struct graph{
  struct graph_t* graph;
  enum color_t* colors;
  int is_compressed;
};

#endif