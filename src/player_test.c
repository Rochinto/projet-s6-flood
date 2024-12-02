#include"player.h"
#include"graph_impl.h"
#include"color_impl.h"
#include"graph.h"
#include<stdio.h>
#include<stdlib.h>

char const* get_player_name(){
  return NULL;
}
void initialize(size_t id, struct graph_t* graph, enum color_t* colors, struct color_set_t forbidden[NUM_PLAYERS]){
  return;
}
void finalize(){
  return;
}
struct move_t play(struct move_t previous_move){
  struct move_t no_move={NO_COLOR};
  return no_move;
}
