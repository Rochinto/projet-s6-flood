#include <assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<getopt.h> 

#include "../src/graph_utils.h"
#include "../src/color.h"

int compute_winner(struct graph *g){
  int p0_nb_vertices = number_vertices_from_player(g, 0);
  int p1_nb_vertices = number_vertices_from_player(g, 1);

  if(p0_nb_vertices > p1_nb_vertices) return 0;
  if(p0_nb_vertices < p1_nb_vertices) return 1;
  return 2; //DRAW
}

int valid(struct graph *g, int player, struct move_t move){
  if(move.c == NO_COLOR) return 1;

  enum color_t opponent_color = graph__get_color(g, graph__get_positions(g).n[1-player]);
  if(is_touching_opponent(g) && move.c == opponent_color){
    return 0;
  }
  int result_by_color[MAX_COLOR];
  number_vertices_winnable_from_player(g,player,result_by_color);
  if(result_by_color[move.c] <= 0){
    return 0;
  }
  return 1;
}

static void test_compute_winner(void){
  printf("%s", __func__);
  int length = 10;
  struct graph *graph = create_square_graph(length);
  for(size_t i = 0; i < graph__get_number_of_vertices(graph); i++){
    graph__set_color(graph,i,0);
  }
  assert(compute_winner(graph)==2);
  graph__set_color(graph,0,1);
  assert(compute_winner(graph)==1);
  graph__set_color(graph,graph__get_number_of_vertices(graph)-1,1);
  graph__set_color(graph,0,0);
  printf("\tOK\n");
}

static void test_is_valid(void){
  printf("%s", __func__);
  int length = 10;
  struct graph *graph = create_square_graph(length);
  for(size_t i = 0; i < graph__get_number_of_vertices(graph); i++){
    graph__set_color(graph,i,0);
  }
  graph__set_color(graph,0,1);
  graph__set_color(graph,graph__get_number_of_vertices(graph)-1,1);
  struct move_t move = {2};
  assert(valid(graph, 0, move)==0);
  move.c = 1;
  assert(valid(graph, 0, move)==0);
  move.c = 0;
  assert(valid(graph, 0, move)==1);
  move.c = -1;
  assert(valid(graph, 0, move)==1);
  printf("\tOK\n");
}

int main(int argc, char* argv[]){
  printf("%s\n", __FILE__);
  test_compute_winner();
  test_is_valid();
  return 0;
}
