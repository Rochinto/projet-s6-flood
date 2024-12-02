#include "player.h" //les fonctions player eseentielles
//jouer un coup sachant le rpecedent renvoie un struct move_t (un coup)
//inisialize/finalize qui restent les memes avec l'inclusion des definitions des structures et des enum color TYPE.
#include "graph_utils.h"
#include "graph_basic.h"
//inclusion des fonctions elementaires et non elementaires pour terminer sur des operations sur des graphes depuis des coups tres maitrisees du joueur en question.


struct player{
  size_t id;
  struct graph* graph;
  struct color_set_t forbidden;
};
//reprise de la definition classique du joueur en terme d'un struct player
//INCLUANT LES champs princiapux images meme des arguments de la fonction
//inisialize.

struct player current_player = {};//definition primaire vide.

char const * get_player_name(){
  char const * player="PLAYER__MIN_MAX";
  return player;//depuis l'algo min max choisir le coup qui maximiserait l'espace qui donnerait un choix parmi les minimales lors du prochain coup de l'adversaire(principe simple mais difficiel a mettre en oeuvre).
}


void initialize(size_t id, struct graph_t* graph, enum color_t* colors, struct color_set_t forbidden[NUM_PLAYERS]){
  current_player.id = id;
  current_player.graph = graph__merge_from(graph, colors);
  current_player.forbidden = forbidden[id];
}

//traite les coups invalides suivant les 3 cas
//deja discutes dans ./src/player_random.c.//VOILA///
void get_gains_by_colors(struct graph *g, int player_id, int possible_gains[]){
  number_vertices_winnable_from_player(g, player_id, possible_gains);
  
  for (int color = 0 ; color < MAX_COLOR ; color++){
    if (current_player.forbidden.t[color] || possible_gains[color] <= 0){
      possible_gains[color] = 0;
    }
  }
  
  enum color_t opponent_color = graph__get_color(g, graph__get_positions(g).n[1 - player_id]);

  if (is_touching_opponent(g)){
    possible_gains[opponent_color] = 0;
  }
}

struct move_t play(struct move_t previous_move){
  coloring_graph_from_player(current_player.graph, 1 - current_player.id, previous_move.c);

  int possible_gains[MAX_COLOR];
  get_gains_by_colors(current_player.graph, current_player.id, possible_gains);

  int best_depth0_score = -1000000;
  struct move_t move = {NO_COLOR};
  
  //MINIMAX de profondeur 3
  for (int color1=-1; color1<MAX_COLOR ; color1++){
    if (color1 < 0 || possible_gains[color1] > 0){

      struct graph *graph2 = graph__merge_from(graph__get_graph_t_copy(current_player.graph), graph__get_colors_copy(current_player.graph));
      coloring_graph_from_player(graph2, current_player.id, color1);
      
      int possible_gains2[MAX_COLOR];
      get_gains_by_colors(graph2, 1-current_player.id, possible_gains2);

      int best_depth1_score = 1000000;
      for (int color2=-1; color2<MAX_COLOR ; color2++){
        if (color2 < 0 || possible_gains2[color2] > 0){
          
          struct graph *graph3 = graph__merge_from(graph__get_graph_t_copy(graph2), graph__get_colors_copy(graph2));
          coloring_graph_from_player(graph3, 1-current_player.id, color2);
      
          int possible_gains3[MAX_COLOR];
          get_gains_by_colors(graph3, current_player.id, possible_gains3);

          int number_vertices_player = number_vertices_from_player(graph3, current_player.id);
          int number_vertices_opponent = number_vertices_from_player(graph3, 1-current_player.id);
          int best_depth2_score = number_vertices_player - number_vertices_opponent;
          for (int color3=0; color3<MAX_COLOR ; color3++){
            int score = number_vertices_player - number_vertices_opponent + possible_gains3[color3];
            if (score > best_depth2_score){ //MAX
              best_depth2_score = score;
            }
          }
          graph__free(graph3);

          if (best_depth2_score < best_depth1_score){ //MIN
            best_depth1_score = best_depth2_score;
          }
          
        }
      }
      graph__free(graph2);

      if (best_depth1_score > best_depth0_score){ //MAX
        best_depth0_score = best_depth1_score;
        move.c = color1;
      }
    }
    
  }

  coloring_graph_from_player(current_player.graph, current_player.id, move.c);
  return move;
}


void finalize(){
  //comme toujours on nettoie les champs de player a savoir l'utilisation de la methode elementaire dans graph_basic.c (tres important ce fichier permet la lisibilite du code et met en oeuvre une facilite a programmer en fait impressionnante..///

  
  graph__free(current_player.graph);
}
