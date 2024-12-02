#include "../src/graph_impl.h"
#include <assert.h>
#include "../src/color.h"
#include "../src/color_impl.h"

#include <stddef.h>
//stdlib pour problemes de valgrind .(stddef.h deja)
#include <gsl/gsl_spmatrix.h>
#include <gsl/gsl_spmatrix_uint.h>
#include <gsl/gsl_spblas.h>

static void test_function_PART1(){
  struct graph* graph=(struct graph*)malloc(sizeof(*graph));
  graph->num_vertex=4;
  graph->djacense_matrix=gsl_spmatrix_uint_alloc(4,4);
  gsl_spmatrix_uint_set(graph->djacense_matrix,0,0,0);
  gsl_spmatrix_uint_set(graph->djacense_matrix,0,1,1);
  gsl_spmatrix_uint_set(graph->djacense_matrix,0,2,1);
  gsl_spmatrix_uint_set(graph->djacense_matrix,0,3,0);
  //les adjacents au sommet 1 numerote 0 graphe cyclique sous forme de carre.
  gsl_spmatrix_uint_set(graph->djacense_matrix,1,0,1);
  gsl_spmatrix_uint_set(graph->djacense_matrix,1,1,0);
  gsl_spmatrix_uint_set(graph->djacense_matrix,1,2,0);
  gsl_spmatrix_uint_set(graph->djacense_matrix,1,3,1);
  //de meme ; les adjacents au sommet 2 numerote 1.(graphe non dirige)
  gsl_spmatrix_uint_set(graph->djacense_matrix,2,0,1);
  gsl_spmatrix_uint_set(graph->djacense_matrix,2,1,0);
  gsl_spmatrix_uint_set(graph->djacense_matrix,2,2,0);
  gsl_spmatrix_uint_set(graph->djacense_matrix,2,3,1);
  //sommet numero 3 numerote 2 dans l'ordre.
  gsl_spmatrix_uint_set(graph->djacense_matrix,3,0,0);
  gsl_spmatrix_uint_set(graph->djacense_matrix,3,1,1);
  gsl_spmatrix_uint_set(graph->djacense_matrix,3,2,1);
  gsl_spmatrix_uint_set(graph->djacense_matrix,3,3,0);
  graph->colors=(enum color_t*)calloc(4,sizeof(enum color_t));
  graph->colors[0]=BLUE;
  graph->colors[1]=RED;
  graph->colors[2]=GREEN;
  graph->colors[3]=YELLOW;
  struct vertex vertex0={0,BLUE};
  enum color_t* colors_from_vertex_0=colors_neighbors(graph,&vertex0);
  printf("%s--",color_to_string(colors_from_vertex_0[0]));
  printf("%s--",color_to_string(colors_from_vertex_0[1]));
  //printf("%s--",color_to_string(colors_from_vertex_0[2]));
  //printf("%s\n",color_to_string(colors_from_vertex_0[3]));
  //il y en a 2 en fait.
  printf("%d\n",number_neighbors(graph,&vertex0));
  printf("%d--",data_neighbors(graph,&vertex0)[0]);
  printf("%d--",data_neighbors(graph,&vertex0)[1]);
  printf("%d--",data_neighbors(graph,&vertex0)[2]);
  printf("%d--\n",data_neighbors(graph,&vertex0)[3]);
  printf("test de data_to_color:%s\n",color_to_string(data_int__to_color(2,graph)));
  free(graph->colors);
  gsl_spmatrix_uint_free(graph->djacense_matrix);
  free(graph);
  //test des 3 fonctions primaires:
  //data_neighbors/number_neighbors/colors_neighbors.
  return;
}

static void test_function_PART2(){
  struct graph* graph=(struct graph*)malloc(sizeof(*graph));
  graph->num_vertex=4;
  graph->djacense_matrix=gsl_spmatrix_uint_alloc(4,4);
  gsl_spmatrix_uint_set(graph->djacense_matrix,0,0,0);
  gsl_spmatrix_uint_set(graph->djacense_matrix,0,1,1);
  gsl_spmatrix_uint_set(graph->djacense_matrix,0,2,1);
  gsl_spmatrix_uint_set(graph->djacense_matrix,0,3,0);
  //les adjacents au sommet 1 numerote 0 graphe cyclique sous forme de carre.
  gsl_spmatrix_uint_set(graph->djacense_matrix,1,0,1);
  gsl_spmatrix_uint_set(graph->djacense_matrix,1,1,0);
  gsl_spmatrix_uint_set(graph->djacense_matrix,1,2,0);
  gsl_spmatrix_uint_set(graph->djacense_matrix,1,3,1);
  //de meme ; les adjacents au sommet 2 numerote 1.(graphe non dirige)
  gsl_spmatrix_uint_set(graph->djacense_matrix,2,0,1);
  gsl_spmatrix_uint_set(graph->djacense_matrix,2,1,0);
  gsl_spmatrix_uint_set(graph->djacense_matrix,2,2,0);
  gsl_spmatrix_uint_set(graph->djacense_matrix,2,3,1);
  //sommet numero 3 numerote 2 dans l'ordre.
  gsl_spmatrix_uint_set(graph->djacense_matrix,3,0,0);
  gsl_spmatrix_uint_set(graph->djacense_matrix,3,1,1);
  gsl_spmatrix_uint_set(graph->djacense_matrix,3,2,1);
  gsl_spmatrix_uint_set(graph->djacense_matrix,3,3,0);
  graph->colors=(enum color_t*)malloc(sizeof(enum color_t));
  graph->colors[0]=BLUE;
  graph->colors[1]=BLUE;
  graph->colors[2]=BLUE;
  graph->colors[3]=RED;
  struct vertex vertex0={0,BLUE};
  display_vertex_color(&vertex0);
  printf("******************************************\n");
  struct vertex* vx1=vertex_same_color__1(graph,&vertex0);
  display_vertex_color(&vx1[0]);
  display_vertex_color(&vx1[1]);
  struct vertex* vx2=vertex_same_color__2(graph,&vertex0);
  display_vertex_color(&vx2[0]);
  display_vertex_color(&vx2[1]);
  printf("Le nombre de voisins de meme couleur est:%d\n",vertex_same_color_number(graph,&vertex0));
  printf("******************************************\n");
  struct vertex* vx=vertex_same_color(graph,&vertex0);
  display_vertex_color(&vx[0]);
  display_vertex_color(&vx[1]);
  printf("test de data_to_color:%s\n",color_to_string(vertex_data__to_color(&vertex0,graph)));
  free(graph->colors);
  gsl_spmatrix_uint_free(graph->djacense_matrix);
  free(graph);
  return;
}

static void test_function_PART3(){
  printf("DEBUT des tests:*********\n");
  //on va generer le graph carre le meme que c dans la page des projets.
  struct graph* graph=generating_type0_graph_colors(9,graph_coloration_random);
  display_gsl_matrix(graph->djacense_matrix);
  display_graph_colors(graph);
  display_graph_colors_type0(graph);
  printf("TOUT EST OK;passons aux tests des derniers fonctions:\n");
  /*enum color_t color=BLUE;
  update_graph_colors__0(graph,1,color);
  display_graph_colors(graph);*/
  return;
}

int main(){
  printf("****************************************************************\n\n");
  printf("TEST de la part_1:\n");
  test_function_PART1();
  printf("OK:______________PASSONS AU DEUXIEME TEST:\n");
  test_function_PART2();
  printf("OK:_______________________PASSONS AU TROISIEME:\n");
  test_function_PART3();
  printf("*************************************************:TESTS(4)\n");
  return 0;
}
/*
gcc  -std=c99 -I/net/ens/renault/save/gsl-2.6/install/include -L/net/ens/renault/save/gsl-2.6/install/lib ./src/player_impl.c ./src/graph_impl.c ./tst/test_graph_impl.c ./src/color_impl.c -lgsl -lgslcblas -lm
 =>test direct avec updating du chemin librarie commande automatique 
a integrer dans le makefile comme export.sh dans le projet javascript.
et voila!!
*/
