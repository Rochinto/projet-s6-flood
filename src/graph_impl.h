#include"./color.h"

#include <stddef.h>
#include <gsl/gsl_spmatrix.h>
#include <gsl/gsl_spmatrix_uint.h>
#include <gsl/gsl_spblas.h>



typedef struct vertex{
  int data;
  enum color_t color_vertex;
}vertice;

typedef struct graph{
  gsl_spmatrix_uint* djacense_matrix;
  unsigned int num_vertex;
  struct vertex** neighbors_each;
  enum color_t* colors;
}graph_colors;

//------part1:
struct vertex* neighbors(struct graph*,struct vertex*);
enum color_t* colors_neighbors(struct graph*, struct vertex*);
unsigned int number_neighbors(struct graph*, struct vertex*);
unsigned int* data_neighbors(struct graph*, struct vertex*);

//------part2:
enum color_t vertex_data__to_color(struct vertex*,struct graph*);
enum color_t data_int__to_color(int,struct graph*);
void display_vertex_color(struct vertex*);

//------part3:
struct vertex* vertex_same_color__1(struct graph*, struct vertex*);
struct vertex* vertex_same_color__2(struct graph*, struct vertex*);
unsigned int vertex_same_color_number(struct graph*,struct vertex*);
struct vertex* vertex_same_color(struct graph*, struct vertex*);
struct vertex** join_vertices(struct vertex*,struct graph*);
//cette derniere est la plus importante de toutes les autres.
//------------------------------------fin_part__3.

//generation du 1er type au moins des graphes ou on va operer:
typedef void(*coloration_graph)(struct graph*);
void display_gsl_matrix(gsl_spmatrix_uint*);
void graph_coloration_random(struct graph*);
struct graph* generating_type0_graph_colors(unsigned int,coloration_graph);
void display_graph_colors(struct graph*);
void display_graph_colors_type0(struct graph*);
//+algo de coloration sous forme de pointeurs de fonctions
//=>coloration aleatoire avec le nombre de couleurs suivant la fonction
//graph_coloration_random.
//FIN DE LA 4ieme partie.(join_vertices!!!)


//_________5ieme partie:
void update_graph_colors__0(struct graph*,size_t,enum color_t);
void update_graph_colors(struct graph*,struct vertex*,enum color_t);
//FAITS.(refaits plutot)
//END_OF(part_five).(tests a completer pour la prochaine fois!!!)
