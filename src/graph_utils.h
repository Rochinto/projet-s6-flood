
#ifndef _GRAPH_UTILS_
#define _GRAPH_UTILS_



#include "graph_basic.h"


struct graph_t;//type abstrait.

struct graph * create_square_graph(int n);

struct graph * create_torus_graph(int n);




void coloring_graph_randomly(struct graph * g);

void display_graph_as_square(struct graph * g, int width, int is_sdl);

void display_graph_as_torus(struct graph * g, int width, int is_sdl);

size_t DFS_color(struct graph * g, enum color_t color, int * visited, size_t from);//le nombre de voisins en profondeur(appels recursifs) ayant la meme couleur
//que from(specifie apres)

struct size_t_list get_component(struct graph * g, size_t from);
//recupere la composante connexe cad. le sous-graphe qui a la meme couleur que le composant from de type struct vertex dans le graph g en entier.

struct size_t_list get_component_outer_border(struct graph * g, size_t from);

size_t component_intersection_size(struct size_t_list component1, struct size_t_list component2);

struct size_t_list component_intersection(struct size_t_list component1, struct size_t_list component2);


//Les dernieres fonctions qui permettent d'appliquer les precedentes
//aux joueurs en question.
int number_vertices_from_player(struct graph * g, int player);

void number_vertices_winnable_from_player(struct graph * g, int player, int result_by_color[]);

void coloring_graph_from_player(struct graph * g, int player, enum color_t color);

int is_touching_opponent(struct graph * g);

#endif
