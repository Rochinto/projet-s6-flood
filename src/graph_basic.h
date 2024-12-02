#ifndef _GRAPH
#define _GRAPH

#include <stddef.h>
#include "color.h"

struct graph;
struct graph_t;

struct size_t_list{
  size_t size;
  unsigned int* list;
};

struct doublon{
  size_t n[2];
};

struct graph* graph__empty(size_t n);

struct graph* graph__merge_from(struct graph_t *gt, enum color_t *colors);

void graph__free(struct graph *g);

int graph__add_edge(struct graph *g, size_t a, size_t b);

struct size_t_list graph__get_neighbors(struct graph *g, size_t vertex);

enum color_t graph__get_color(struct graph *g, size_t vertex);

enum color_t* graph__get_colors_copy(struct graph *g);

int graph__set_color(struct graph *g, size_t vertex, enum color_t color);

struct doublon graph__get_positions(struct graph *g);

int graph__set_positions(struct graph *g, size_t p0, size_t p1);

size_t graph__get_number_of_vertices(struct graph *g);

void graph__compress(struct graph *g);

struct graph_t *graph__get_graph_t(struct graph *g);

size_t graph_t__get_number_of_vertices(struct graph_t *gt);

struct doublon graph_t__get_positions(struct graph_t *gt);

//@brief Fast copy function to gain time complexity (not elementary)
struct graph_t* graph__get_graph_t_copy(struct graph *g);

#endif
