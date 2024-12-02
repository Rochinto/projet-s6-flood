#ifndef _FLOOD_GRAPH_H_
#define _FLOOD_GRAPH_H_

#include <stddef.h>
#include <gsl/gsl_spmatrix.h>
#include <gsl/gsl_spmatrix_uint.h>
#include <gsl/gsl_spblas.h>

#include "color.h"

#define NUM_PLAYERS 2

enum graph_type_t { SQUARE=0, DONUT=1, HGRAPH=2, TORUS=3 };
#define TYPE(s) ((s == SQUARE) ? "square" : \
                 ((s == DONUT) ? "donut" : \
                  ((s == HGRAPH) ? "hashed" : "torus")))

struct graph_t {
  size_t num_vertices;  // Number of vertices in the graph
  gsl_spmatrix_uint* t; // Sparse matrix of size n*n,
                        // t[i][j] > 0 means there is an edge from i to j
  size_t positions[NUM_PLAYERS]; // Start positions of each player, indexed by id
};

#endif // _FLOOD_GRAPH_H_

