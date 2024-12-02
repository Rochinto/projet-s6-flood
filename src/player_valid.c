#include "player.h"
#include "graph_utils.h"
#include "graph_basic.h"

/**
 * @struct player
 * @brief A structure containing some information about the player
 */
struct player{
  size_t id; /**< @brief The id of the player (may be 0 or 1) */
  struct graph * graph; /**< @brief The game board */
  struct color_set_t forbidden; /**< @brief A set of colors that cannot be played in the current game */
};

struct player current_player = {};

/**
 * @fn get_player_name
 * @brief Displays player information
 * @return The player name as an [a-zA-Z0-9 -_]* string
 */
char const * get_player_name(){
  char const * player="PLAYER_VALID";
  return player;
}

/**
 * @fn initialize
 * @brief Player initialization
 * @param id The id assigned to the player
 * @param graph The graph where the game is played (represents the structure of the board)
 * @param colors The coloration of the graph 
 * @param forbidden An array containing all forbidden colors for each player
 * 
 * PRECOND:
 * - `id` is either 0 or 1
 * - `graph` is a heap-allocated copy of the graph where the game is played, that must be freed in the end
 * - `colors` is a heap allocated copy array associating to each vertex id a color, that must be freed in the end
 * - `forbidden` is an array of color sets, one for each player, specifying the forbidden colors for each player (one per id)
 * - initialize has never been called before
 */
void initialize(size_t id, struct graph_t * graph, enum color_t * colors, struct color_set_t forbidden[NUM_PLAYERS]){
  current_player.id = id;
  current_player.graph = graph__merge_from(graph, colors);
  current_player.forbidden = forbidden[id];
}

// résultat stocké dans valid[] par effet de bord
// retourne le nombre de coups valides

/**
 * @brief Computes what colors the current player is authorized to play
 * @param valid[] An array of at least MAX_COLOR elements
 * @param opponent_color The current color of the opponent
 * @return The number n of possible moves for the current player
 * 
 * POSTCOND: the first n elements of valid[] are the possible moves
 */
int get_valid_moves(int valid[], int opponent_color){
  int n = 0;
  int possible_wins[MAX_COLOR];
  number_vertices_winnable_from_player(current_player.graph, current_player.id, possible_wins);
  if (opponent_color == NO_COLOR){
    opponent_color = graph__get_color(current_player.graph, graph__get_positions(current_player.graph).n[1 - current_player.id]);
  }
  for (int color = 0 ; color < MAX_COLOR ; color += 1){
    int isvalid = 1;
    if (current_player.forbidden.t[color]){
      isvalid = 0;
    }
    if (possible_wins[color] <= 0){
      isvalid = 0;
    }   // s'il n'y a aucun voisin de cette couleur
    if (is_touching_opponent(current_player.graph) && (color == opponent_color)){
      isvalid = 0;
    }
    if (isvalid){
      valid[n] = color;
      n++;
    }
  }

  return n;
}

/// @fn play
struct move_t play(struct move_t previous_move){
  coloring_graph_from_player(current_player.graph, 1-current_player.id, previous_move.c);

  int possible_moves[MAX_COLOR];
  int n = get_valid_moves(possible_moves, previous_move.c);
  
  struct move_t move = {NO_COLOR};
  if (n > 0){
    move.c = possible_moves[rand() % n];
  }

  coloring_graph_from_player(current_player.graph, current_player.id, move.c);
  return move;
}

/**
 * @fn finalize
 * @brief Announces the end of the game to the player, and cleans up the memory he may have been using.
 *
 * POSTCOND: every allocation done during the calls to initialize and play functions must have been freed
 */
void finalize(){
  // methode_elementaire_pour_free(current_player.graph);
  graph__free(current_player.graph);
}
