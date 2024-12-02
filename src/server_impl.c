#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include "player.h"
#include <getopt.h>
#include <string.h> 

struct player{
  void (*initialize)(size_t, struct graph_t*, enum color_t*, struct color_set_t*);
  struct move_t (*play)(struct move_t);
  void (*finalize)();
  char const* (*get_player_name)();
};

int compute_next_player(int id){
  return (1 - id);
}

int is_winning(){
  return 0;
}

int main(int argc, char* argv[]){
  struct player p[2];
  /*int length = 10;
  char shape = 'c';
  int num_color = 4;
  int seed;
  int forbidden = 4;
  int opt;
  while ((opt = getopt(argc, argv, "m:t:c:a:f:")) != -1){
    switch (opt){
    case 'm':
      length = atoi(optarg);
      break;
    case 't':
      shape = *optarg;
      break;
    case 'c':
      num_color = atoi(optarg);
      break;
    case 'a':
      seed = atoi(optarg);
      break;
    case 'f':
      forbidden = atoi(optarg);
      break;
    default:
      printf("Unknown parameters\n");
      break;
    }
  }
  int tmp = 1;
  while(tmp<argc && !strstr(argv[tmp], "install")){
    ++tmp;
    }
  */
  void *handle1 = dlopen(argv[1], RTLD_LAZY);
  void *handle2 = dlopen(argv[2], RTLD_LAZY);
  p[0].initialize = dlsym(handle1, "initialize");
  p[1].initialize = dlsym(handle2, "initialize");
  p[0].play = dlsym(handle1, "play");
  p[1].play = dlsym(handle2, "play");
  p[0].finalize = dlsym(handle1, "finalize");
  p[1].finalize = dlsym(handle2, "finalize");
  p[0].get_player_name = dlsym(handle1, "get_player_name");
  p[1].get_player_name = dlsym(handle2, "get_player_name");
  int start_player_id = rand()%2;
  int other_player_id = 1 - start_player_id;
  struct graph_t * graph = NULL; //= ...
  enum color_t* graph_colors = NULL; //= ...
  struct color_set_t forbidden_colors[NUM_PLAYERS] = {}; //= ...
  //initialisation correcte pour des variables simples qui ne font rien.
  
  //pas d'application de fonction d'adresse 0X00.
  //serveur QUI NE FAIT RIEN.

  //debut des SEGFAULTS ici au niveau de ces applications
  //gdb + break main + r(yes) + break la ligne de la boucle
  //+p i+p p[i]+n montre la faute de segmentation qui est p[0/1].initialize
  //est NULL dans l'avant fichier utilise et donc segfault bien sur.

  
  /*p[0].initialize(0, graph,graph_colors, forbidden_colors);
    p[0].initialize(1, graph,graph_colors, forbidden_colors);*/

  
  /*int active = other_player_id;
  struct move_t move;
  while(1){
    active = compute_next_player(active);
    move = p[active].play(move);
    if(is_winning()){
      break;
    }
    for(int i = 0; i < 2; i++){
      p[i].finalize();
    }
  }*/
  printf("Ca marche !\n");
  /*dlclose(handle1);
    dlclose(handle2);*/

  //segfault aussi au niveau de l'appel a la libc(probleme de compilation
  //qui sera regle par l'ajout d'une fonction void* unload(void* lib)
  //qui fera le boulot apres)
  
  return 0;
}
