#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <getopt.h>
#include <string.h> 
#include <time.h>

#include "graph_utils.h"
#include "player.h"
#include "players.h"

/** \mainpage Projet Flood-filling games
 *
 * This project uses the GNU Scientific Libray.
 *
 * ### Before compiling the project
 *
 * - install the GNU Scientific Library (GSL)
 * - `export LD_LIBRARY_PATH=` *path/to/GSL/libraries*
 *
 * ### Project compilation
 * - `make server` : compiles the server at ./build/server
 * - `make player_valid` : compiles a player making random but valid moves at ./build/libplayer_valid.so
 * - `make player_glouton` : compiles a "gluttonous" player, maximizing the instant wins for each move at ./build/libplayer_max.so
 * - `make alltests` : compiles a series of tests and some ".gcno" trace files for code coverage
 * - `GSL_PATH=` *path/to/GSL/directories* ` make` : compiles all previous files
 * - `make install` : among the previously compiled files, creates a copy of every file that could be usefull to run a part of the project at ./install/*file_name*
 *
 * ### Project running
 * - `./install/server ./install/libplayer_max.so ./install/libplayer_valid.so` : plays a game between a gluttonous player and a random player.
 * Make sure not to use the same file for both players (you can still use a copy).
 * - `./install/alltests` : runs a series of tests
 */



/**
 *@brief A structure containing the initial settings of a game
 */
struct options{
  int length; ///< The length of the graph
  char shape; ///< The shape of the graph
  int num_color; ///< The number of different colors that can be on the graph
  int seed; ///< An int that defines the random seed of the game
  int forbidden; ///< The number of colors that cannot be played
  int games; ///< The number of games to be played
  int start_player; ///< Defines the player who starts the game
};

/**
*@brief Swaps the active and the waiting player
*
*@param id The id of the active player
*@return The id of the next player
*
* PRECOND: 
* - id should be either 0 or 1
*
* POSTCOND:
* - If id = 0 it should return 1 and if id = 1 it should return 0
*/
int compute_next_player(int id){
  return (1 - id);
}

/**
*@brief Decides who wins the game
*
*@param g The graph of the game
*@return An integer depending on the result of the game
*
* PRECOND: 
* - g contains an initialized compressed graph with the list of colors of the graph
*
* POSTCOND: 
* - It should return 0 if player 1 wins, 1 if player 2 wins and 2 if there is a draw
*/
int compute_winner(struct graph * g){
  int p0_nb_vertices = number_vertices_from_player(g, 0);
  int p1_nb_vertices = number_vertices_from_player(g, 1);

  if(p0_nb_vertices > p1_nb_vertices){
    return 0;
  }

  if(p0_nb_vertices < p1_nb_vertices){
    return 1;
  }
  
  return 2; //DRAW
}

/**
*@brief Checks if the color chosen by a player is authorized by the game rules or not
*
*@param g The graph of the game
*@param player The id of the player who has chosen the color
*@param move The color chosen by the player
*@return An integer depending on the move being valid or not
*
* PRECOND: 
* - g contains an initialized compressed graph with the list of colors of the graph
* - The player id should be either 0 or 1 
* - move contains a color
*
* POSTCOND:
* - It should return 0 if the move is not valid, or 1 if it is valid
*/
int valid(struct graph *g, int player, struct move_t move){
  
  if(move.c == NO_COLOR){
    return 1;
  }

  enum color_t opponent_color = graph__get_color(g, graph__get_positions(g).n[1 - player]);
  
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

/**
*@brief Plays a game between two players until someone breaks a rule or if there is no move available
*
*@param opt A structure containing the options of the game
*@param p A structure dedicated for the functions of the players
*@param player_one The name of the player of the first library
*@param player_two The name of the player of the second library
*@return winner The id of the winner, or 2 if there is a draw
*
* PRECOND: 
* - g contains an initialized compressed graph with the list of colors of the graph
* - p is a table with two structures containing four functions each
*/
int game(struct options opt, struct dlplayer* p, char* player_one, char* player_two){
  p[0] = player_load(player_one);
  p[1] = player_load(player_two);

  int start_player_id = opt.start_player;
  int other_player_id = 1 - start_player_id;
  struct graph *graph = create_square_graph(opt.length);
  coloring_graph_randomly(graph);
  struct color_set_t forbidden_colors[NUM_PLAYERS] = {};

  p[0].initialize(0, graph__get_graph_t_copy(graph), graph__get_colors_copy(graph), forbidden_colors);
  p[1].initialize(1, graph__get_graph_t_copy(graph), graph__get_colors_copy(graph), forbidden_colors);
  
  if(opt.games == 1){
    if(SDL){
      fprintf(stderr, "(Player 0) : %s\n", p[0].get_player_name());
      fprintf(stderr, "(Player 1) : %s\n", p[1].get_player_name());
    }else{
      printf("(Player 0) : %s\n", p[0].get_player_name());
      printf("(Player 1) : %s\n", p[1].get_player_name());
    }
  }
  
  if(opt.games == 1){
    display_graph_as_square(graph, opt.length, SDL);
  }

  int active = other_player_id;
  struct move_t previous_move = {NO_COLOR};
  int turn = 0;
  int winner = -1;

  while(winner < 0){
    active = compute_next_player(active);
    struct move_t move = p[active].play(previous_move);
    
    if(!valid(graph, active, move)){
      if(SDL && opt.games == 1){
	      fprintf(stderr, "%s : coup non valide\n", p[active].get_player_name());
      }else if(!(SDL) && opt.games == 1){
        printf("%s : coup non valide\n", p[active].get_player_name());
      }
      winner = compute_next_player(active);
      break;
    }

    coloring_graph_from_player(graph, active, move.c);

    if(opt.games == 1){
      display_graph_as_square(graph, opt.length, SDL);
    }

    if(move.c == previous_move.c && move.c == NO_COLOR && turn > 0){
      winner = compute_winner(graph);
    }
    
    previous_move = move;
    turn++;
  }

  if(opt.games == 1){
    if(winner == 2){ //DRAW
      if(SDL){
	      fprintf(stderr, "Game over, DRAW\n"); 
      }else{
	      printf("Game over, DRAW\n");
      }
    }else{
      if(SDL){
	      fprintf(stderr, "Game over, WINNER : (%d) %s !\n", winner, p[winner].get_player_name());
      }else{
	      printf("Game over, WINNER : (%d) %s !\n", winner, p[winner].get_player_name());
      }
    } 
    if(SDL){
      fprintf(stderr, "(0) %s owns %d vertices\n", p[0].get_player_name(), number_vertices_from_player(graph,0));
      fprintf(stderr, "(1) %s owns %d vertices\n", p[1].get_player_name(), number_vertices_from_player(graph,1));
    }else{
      printf("(0) %s owns %d vertices\n", p[0].get_player_name(), number_vertices_from_player(graph,0));
      printf("(1) %s owns %d vertices\n", p[1].get_player_name(), number_vertices_from_player(graph,1));
    }
  }

  graph__free(graph);

  for(int i = 0; i < 2; i++){
    p[i].finalize();
  }

  player_close(&p[0]);
  player_close(&p[1]);
  return winner;
}

int main(int argc, char* argv[]){
  struct dlplayer p[2];
  struct options option = {10, 'c', 4, time(0), 4, 1};
  int opt;

  while ((opt = getopt(argc, argv, "m:t:c:a:f:r:")) != -1){
    switch (opt){
    case 'm':
      option.length = atoi(optarg);
      break;
    case 't':
      option.shape = *optarg;
      break;
    case 'c':
      option.num_color = atoi(optarg);
      break;
    case 'a':
      option.seed = atoi(optarg);
      break;
    case 'f':
      option.games = atoi(optarg);
      break;
    case 'r':
      option.games = atoi(optarg);
      break;
    case '?':
      printf("Unknown option: %c\n", optopt);
      break;
    case ':':
      printf("Missing argument for %c\n", optopt);
      break;
    default:
      printf("Unknown parameters\n");
      break;
    }
  }

  srand(option.seed);
  int tmp = 1;

  while(tmp<argc && !strstr(argv[tmp], "install")){
    ++tmp;
  }

  char* player_one = argv[tmp];
  ++tmp;

  while(tmp<argc && !strstr(argv[tmp], "install")){
    ++tmp;
  }

  char* player_two = argv[tmp];

  option.start_player = rand()%2;
  if(SDL){
    printf("%d %d\n", option.length, option.length);
  }

  int wins_counter[3] = {0,0,0};

  for(int round = 0; round < option.games; round++){
    int winner = game(option, p, player_one, player_two);
    wins_counter[winner]++;
  }
  
  p[0] = player_load(player_one);
  p[1] = player_load(player_two);

  printf("\n");
  printf("(Player 0) %s wins : %d\n", p[0].get_player_name(), wins_counter[0]);
  printf("(Player 1) %s wins : %d\n", p[1].get_player_name(), wins_counter[1]);
  printf("Number of draws : %d\n", wins_counter[2]);
  
  player_close(&p[0]);
  player_close(&p[1]);
  return 0;
}
