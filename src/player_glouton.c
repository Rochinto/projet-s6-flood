#include "player.h"
#include "graph_utils.h"
#include "graph_basic.h"

/// @struct player
struct player{
  size_t id;
  struct graph* graph;
  struct color_set_t forbidden;
};

struct player current_player = {};

/// @fn get_player_name
char const * get_player_name(){
  char const * player="PLAYER_MAX";
  return player;
}



/**
 * @brief Player initialization
 * @param id the id assigned to the player
 * @param graph the graph where the game is played (represents the structure of the board)
 * @param colors the coloration of the graph 
 * @param forbidden an array containing all forbidden colors for each player
 * 
 * PRECOND:
 * - `id` is either 0 or 1
 * - `graph` is a heap-allocated copy of the graph where the game is played, that must be freed in the end
 * - `colors` is a heap allocated copy array associating to each vertex id a color, that must be freed in the end
 * - `forbidden` is an array of color sets, one for each player, specifying the forbidden colors for each player (one per id)
 * - initialize has never been called before
 */
void initialize(size_t id, struct graph_t* graph, enum color_t* colors, struct color_set_t forbidden[NUM_PLAYERS]){
  current_player.id = id;
  current_player.graph = graph__merge_from(graph, colors);
  current_player.forbidden = forbidden[id];
}


// retourne un tableau indiquant pour chaque couleur le nombre de sommets gagnables, et 0 si le coup est invalide

/**
 * @brief Computes the number of cells the player can win by playing each color
 * @param possible_gains[] An array of at least MAX_COLOR elements
 *
 * POSTCOND : The potential scores are stored in the array possible_gains[]
 */
void get_gains_by_colors(int possible_gains[]){
  number_vertices_winnable_from_player(current_player.graph, current_player.id, possible_gains);
  
  for (int color = 0 ; color < MAX_COLOR ; color++){
    if (current_player.forbidden.t[color] || possible_gains[color] <= 0){
      possible_gains[color] = 0;
    }
  }
  
  enum color_t opponent_color = graph__get_color(current_player.graph, graph__get_positions(current_player.graph).n[1 - current_player.id]);

  if (is_touching_opponent(current_player.graph)){
    possible_gains[opponent_color] = 0;
  }
}

/**
 * @brief Computes the next move
 * @param previous_move The move from the previous player. If this is the first move of the game, `previous_move` is equal to NO_COLOR.
 * @return The next move for the player.
 */
struct move_t play(struct move_t previous_move){
  coloring_graph_from_player(current_player.graph, 1 - current_player.id, previous_move.c);
  int possible_gains[MAX_COLOR];
  get_gains_by_colors(possible_gains);
  int best_gain = 0;
  struct move_t move = {NO_COLOR};

  for (int color=0 ; color<MAX_COLOR ; color++){
    if(possible_gains[color] > best_gain){
      best_gain = possible_gains[color];
      move.c = color;
    }
  }

  coloring_graph_from_player(current_player.graph, current_player.id, move.c);
  return move;
}

/// @fn finalize
void finalize(){
  // methode_elementaire_pour_free(current_player.graph);
  graph__free(current_player.graph);
}
