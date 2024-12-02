#include "./player.h"
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#ifndef ID
#define ID 0
#define ID2 1
#endif
#include<time.h>
#include "./graph_impl.h"
#include "./graph.h"
#include "color_impl.h"

#define NAME(x) ((x==0) ? "player_number_one" : "player_number_two")

typedef struct player{
  char const* name;
  size_t id;
  struct color_set_t forbidden;
  enum color_t color_departure;
  struct graph* tray;
  int position;
}player;  

#define player(y) player_##y
player player(ID);

player player(ID2);

//pour eviter le 1-ID et pas de compilation avec -DID=0 pour que:
//ID prend la valeur 0 des le depart ainsi 2 joueurs sont declares
//en globaux ici.(ID_2=>1-ID<=>1).
//car on aura des problemes apres pour la designation du joueur oppose
//donc on garde la definition de la macro ID2 avec sans -DID=0/1
//en ligne de compilation.

char const* get_player_name(){
  char const* player_name=(char const*)NAME(ID);
  return player_name;
}

void initialize(size_t id, struct graph_t* graph, enum color_t* colors, struct color_set_t forbidden[NUM_PLAYERS]){
  player(ID).name=get_player_name();
  player(ID).id=id;
  player(ID).forbidden=forbidden[(int)(player(ID).id)];
  player(ID).color_departure=rand()%(MAX_COLOR);
  //size_t i,j;
  srand(time(NULL));
  //size_t rslt_sum=i+j;
  size_t rslt_sum=rand()%(graph->num_vertices);
  player(ID).tray=(struct graph*)malloc(sizeof(struct graph));
  player(ID).tray->colors=colors;
  *(player(ID).tray->colors+rslt_sum)=rand()%(MAX_COLOR);
  player(ID).tray->djacense_matrix=graph->t;
  player(ID).tray->num_vertex=graph->num_vertices;
  player(ID).tray->neighbors_each=(struct vertex**)malloc(sizeof(void*));
  for(int h=0;h<player(ID).tray->num_vertex;h++){
    struct vertex vertex_h;
    vertex_h.data=h;
    struct graph* graph=player(ID).tray;
    //1er probleme: player(ID).tray->neighbors_each et de type struct vertex**
    //en h ca sera de type struct vertex*
    //remplacer: colors_neighbors par neighbors tout simple
    //avec ca on aura le struct vertex* qui represente la liste des voisins
    //qu'on recherche.
    player(ID).tray->neighbors_each[h]=neighbors(graph,&vertex_h);
  }
  player(ID).position=rand()%(graph->num_vertices);
  return;
}

void update_forbidden_departure(struct player* player,struct move_t* move){
  enum color_t colors_permitted[MAX_COLOR];
  int compteur=0;
  for(int b=0;b<MAX_COLOR;b++){
    //2ieme probleme: 2ieme argument de is_in_array dans color_impl.h
    //est de type: struct color_set_t* alors que player(ID).forbidden
    //est de type: struct color_set_t => remplacer par &player(ID).forbidden.
    if(!is_in_array((enum color_t)b,&player(ID).forbidden) && (enum color_t)b!=move->c){
      colors_permitted[compteur]=(enum color_t)b;
      compteur++;
    }}
  char* src; char* dest;
    player(ID).color_departure=player(ID).color_departure=rand()%(compteur);
    strcpy(src,player->forbidden.t);
    strcpy(dest,color_to_string(move->c));
    strcat(src,dest);
    strcpy(player->forbidden.t,dest);
}



struct move_t play(struct move_t previous_move){
  enum color_t color=previous_move.c;

  //3ieme probleme: comparaison entre un struct move_t et un enum color_t
  //resolution: remplacer le ==NO_COLOR par: l'instruction de controle suivante
  //if(color(previous_move.c)==NO_COLOR) et voila.
  if(color==NO_COLOR){
    update_forbidden_departure(&player(ID2),&previous_move);

    //update_graph_colors prend en argv[2] un type struct vertex*
    //donc on cree sur la pile un type nomme vertex_in
    //avec comme .data la valeur player(ID2).position
    //et .color_vertex la valeur: player(ID2).color_departure(de type

    //enum color_t dans les 2 sens).
    struct vertex vertex_in={.data=player(ID2).position,.color_vertex=player(ID2).color_departure};

    //on passe &vertex_in en parametres pour respecter les conventions
    //de specifications internes aux fonctions manipulees ici comme dans
    //le cas de update_forbidden_departure en terme de argv[2](&previous_move).

    update_graph_colors(player(ID2).tray,&vertex_in,player(ID2).color_departure);
    struct move_t next_move={player(ID).color_departure};
    return play(next_move);
  }
  else{
    //2ieme parametre de is_in_array est un pointeur : meme probleme
    //rencontre pour la toute premiere fonction.

    //&player(ID).forbidden est de type struct color_set_t*
    //le c undefined doit etre de meme type..
    //l'argument previous_move.c est de type enum color_t
    //on enleve ce test qui sert a rien a la fin de la copie.
    //normalement: on s'est rendu de cela, si la couleur
    //se trouve dans les forbidden*, on introduit la meme
    //updating du graphe des couleurs pour qu'il reste pratiquement
    //le meme sauf la position en terme de couleur change (la couleur
    //du sommet se mute) et on poursuit l'execution comme avant..///
    
    /*if(is_in_array(play(ID).color_departure,&player(ID).forbidden) || is_in_array(player(ID).color_departure,c)){*/
    
    if(is_in_array(player(ID).color_departure,&player(ID).forbidden)){
    
      update_forbidden_departure(&player(ID2),&previous_move);
      
       struct vertex vertex_in={.data=player(ID2).position,.color_vertex=player(ID2).color_departure};
       
      update_graph_colors(player(ID2).tray,&vertex_in,player(ID2).color_departure);
      struct move_t next_move={player(ID).color_departure};
      return play(next_move);
    }
    else{

struct vertex vertex_in={.data=player(ID).position,.color_vertex=player(ID).color_departure};
//meme erreur+erreur de update_graph_colors=>update(graph_colors(...))
//correction faite maitrisee OK .
      
      update_graph_colors(player(ID).tray,&vertex_in,color);
      struct move_t next_move={player(ID2).color_departure};
      return play(next_move);
    }
  }
}
