
#include<stdio.h>
#include<stdlib.h>

#include "./color.h"
#include "graph_impl.h"

#include <stddef.h>
#include <gsl/gsl_spmatrix.h>
#include <gsl/gsl_spmatrix_uint.h>
#include <gsl/gsl_spblas.h>
#define LIMIT 10
#include<math.h>
#include<time.h>


struct vertex* neighbors(struct graph* graph,struct vertex* vertex){
  struct vertex* vx_all=(struct vertex*)malloc(sizeof(*vx_all));
  int data=vertex->data;
  int color=graph->colors[data];
  int k=0;
  for(int i=0;i<graph->num_vertex;i++){
    int value=gsl_spmatrix_uint_get(graph->djacense_matrix,(size_t)i,(size_t)data);
    if(value==1){
      //sommet numero i a prendre en compte.
      struct vertex vx={i,data_int__to_color(i,graph)};
      vx_all[k]=vx;
      k++;//incrementation de l'indexation des sommets adjacents dans le tab
      //.retourne.
    }}
  return vx_all;
}



enum color_t* colors_neighbors(struct graph* graph, struct vertex* vertex){
  enum color_t* colors=(enum color_t*)calloc(1,sizeof(enum color_t));
  int data=vertex->data;
  int color=graph->colors[data];
  int k=0;
  for(int i=0;i<graph->num_vertex;i++){
    int value=gsl_spmatrix_uint_get(graph->djacense_matrix,(size_t)i,(size_t)data);
    if(value==1){
      colors[k]=i;k++;
      vertex->color_vertex=*(graph->colors+vertex->data);
    }}
    for(int j=0;j<graph->num_vertex;j++){
      int value=gsl_spmatrix_uint_get(graph->djacense_matrix,(size_t)data,(size_t)j);
      if(value==1){
	colors[k]=j;k++;
	vertex->color_vertex=*(graph->colors+vertex->data);
      }}
  return colors;
}

unsigned int number_neighbors(struct graph* graph, struct vertex* vertex){
  int data=vertex->data;
  int color=graph->colors[data];
  int k=0;
  for(int i=0;i<graph->num_vertex;i++){
    int value=gsl_spmatrix_uint_get(graph->djacense_matrix,(size_t)i,(size_t)data);
    if(value==1){
      k++;
    }}
    for(int j=0;j<graph->num_vertex;j++){
      int value=gsl_spmatrix_uint_get(graph->djacense_matrix,(size_t)data,(size_t)j);
      if(value==1){
	k++;
      }}
  return k/2;
}

unsigned int* data_neighbors(struct graph* graph, struct vertex* vertex){
  unsigned int* data_neighbors=(unsigned int*)malloc(sizeof(unsigned int));
  int data=vertex->data;
  int color=graph->colors[data];
  int k=0;
  for(int i=0;i<graph->num_vertex;i++){
    int value=gsl_spmatrix_uint_get(graph->djacense_matrix,(size_t)i,(size_t)data);
    if(value==1){
      data_neighbors[k]=i;
      k++;
    }}
    for(int j=0;j<graph->num_vertex;j++){
      int value=gsl_spmatrix_uint_get(graph->djacense_matrix,(size_t)data,(size_t)j);
      if(value==1){
	data_neighbors[k]=j;
	k++;
      }}
  return data_neighbors;
}

void display_vertex_color(struct vertex* vx){
  printf("%d--%s\n",vx->data,color_to_string(vx->color_vertex));
}

/*
--------------------------------------------------------
--------------------------------------------------------
*/
  
struct vertex* vertex_same_color__1(struct graph* graph, struct vertex* vertex){
  gsl_spmatrix_uint* matrix=graph->djacense_matrix;
  int len=graph->num_vertex;
  enum color_t color_vertex=vertex->color_vertex;
  struct vertex* vertex_all1=(struct vertex*)malloc(sizeof(*vertex_all1));
  int k=0;
  for(int i=0;i<len;i++){
      int val=gsl_spmatrix_uint_get(graph->djacense_matrix,i,vertex->data);
      if(val==1){
	enum color_t color=*(graph->colors+i);
	if(color==color_vertex){
	  struct vertex vertex1;
	  vertex1.data=i;vertex1.color_vertex=*(graph->colors+vertex1.data);
	  vertex_all1[k++]=vertex1;
	}
      }
  }
  return vertex_all1;
}

struct vertex* vertex_same_color__2(struct graph* graph, struct vertex* vertex){
  gsl_spmatrix_uint* matrix=graph->djacense_matrix;
  int len=graph->num_vertex;
  enum color_t color_vertex=vertex->color_vertex;
  struct vertex* vertex_all2=(struct vertex*)malloc(sizeof(*vertex_all2));
  int k=0;
   for(int j=0;j<len;j++){
      int val=gsl_spmatrix_uint_get(graph->djacense_matrix,vertex->data,j);
      if(val==1){
	enum color_t color=*(graph->colors+j);
	if(color==color_vertex){
	  struct vertex vertex2;
	  vertex2.data=j;vertex2.color_vertex=*(graph->colors+vertex2.data);
	  vertex_all2[k++]=vertex2;
	}
      }
   }
   return vertex_all2;
}

unsigned int vertex_same_color_number(struct graph* graph,struct vertex* vx){
  gsl_spmatrix_uint* matrix=graph->djacense_matrix;
  int len=graph->num_vertex;
  enum color_t color_vertex=vx->color_vertex;
  int k=0;
   for(int j=0;j<len;j++){
      int val=gsl_spmatrix_uint_get(graph->djacense_matrix,vx->data,j);
      //le graph est non dirige=>la matrice est symetrique
      //=>les 2 cas sont parfaitement semblables.
      if(val==1){
	enum color_t color=*(graph->colors+j);
	if(color==color_vertex){
	  k++;
	}
      }
   }
   return k;
}


struct vertex* vertex_same_color(struct graph* g, struct vertex* v){
  struct vertex* v1=vertex_same_color__1(g,v);
  return v1;//le graphe est non dirige=>la matrice est symetrique(type gsl)
  //=>le retour de same_color__1 est le meme que same_color__2.
}


/*
--------------------------------------------------------
--------------------------------------------------------
*/

enum color_t vertex_data__to_color(struct vertex* vx, struct graph* graph){
  return graph->colors[vx->data];
}

enum color_t data_int__to_color(int data_vertex, struct graph* graph){
  return graph->colors[data_vertex];
}

struct graph* generating_type0_graph_colors(unsigned int number_vertices,coloration_graph coloration){
  
  struct graph* graph=(struct graph*)malloc(sizeof(*graph));

  graph->num_vertex=number_vertices;//par exemple: 64 pour le graphe carre c.
  
  size_t number_vertices2=(size_t)number_vertices;
  graph->djacense_matrix=gsl_spmatrix_uint_alloc(number_vertices2,number_vertices2);
  //remplir la matrice creuse d'ajcense du graphe de type carre.     
  for(size_t h1=0;h1<number_vertices2;h1++){
    for(size_t h2=0;h2<number_vertices2;h2++){
      //on fixe le sommet i a traiter:la matrice etant symetrique(voir apres).
      //si h2 appartient à l'une des 2 repertories ci-dessous
      //cela veut dire qui ne depasse pas les limites de la matrice.
      //c'est plus simple comme ca(voir l'impl. complete dans src/impl.txt).
      size_t v1=h1-1;
      size_t v2=h1+1;
      size_t v3=h1-(size_t)sqrt(number_vertices2);
      size_t v4=h1+(size_t)sqrt(number_vertices2);
      if(h2==v1) gsl_spmatrix_uint_set(graph->djacense_matrix,h1,v1,1);
      if(h2==v2) gsl_spmatrix_uint_set(graph->djacense_matrix,h1,v2,1);
      if(h2==v3) gsl_spmatrix_uint_set(graph->djacense_matrix,h1,v3,1);
      if(h2==v4) gsl_spmatrix_uint_set(graph->djacense_matrix,h1,v4,1);
      //else
      //gsl_spmatrix_uint_set(graph->djacense_matrix,h1,h2,0);
      //par defaut gsl_spmatrix genere une matrice de zeros.(comme avec calloc..)
    }
  }
  //puis on colorie.
  coloration(graph);
  return graph;
}

void graph_coloration_random(struct graph* graph){
  unsigned int number=graph->num_vertex;
  graph->colors=(enum color_t*)malloc(number*sizeof(enum color_t));
  for(int k=0;k<number;k++){
    //srand(time(NULL));
    graph->colors[k]=rand()%(MAX_COLOR);//de 0->7<=>en string: de 'BLUE'à'PINK'.
  }
  return;
}


void display_gsl_matrix(gsl_spmatrix_uint* m){
  for(size_t k1=0;k1<(m->size1);k1++){
    for(size_t k2=0;k2<(m->size2);k2++){
      unsigned int data=gsl_spmatrix_uint_get(m,k1,k2);
      printf("%d\t",data);
    }
    printf("\n");
  }
}

void display_graph_colors(struct graph* graph){
  for(int total=0;total<graph->num_vertex;total++){
      unsigned int data=graph->colors[total];
      printf("%s\t",color_to_string(data_int__to_color(data,graph)));
  }
  printf("\n");
}

void display_graph_colors_type0(struct graph* graph){
  for(int total=0;total<(int)sqrt(graph->num_vertex);total++){
    for(int total2=0;total2<(int)sqrt(graph->num_vertex);total2++){
      enum color_t data=graph->colors[total*(int)sqrt(graph->num_vertex)+total2];
      printf("%s\t",color_to_string(data_int__to_color(data,graph)));
    }
    printf("\n");
  }
}


/*
--------------------------------------------------------
--------------------------------------------------------
*/



void update_graph_colors__0(struct graph* g, size_t position,enum color_t color){
  g->colors[(int)position]=color;
  return;
}
							   

void update_graph_colors(struct graph* g,struct vertex* v,enum color_t color){
  *(g->colors+v->data)=color;
  struct vertex* vertex_same_color1=vertex_same_color(g,v);
  int cpt=0;
  unsigned int number_neighbors1=number_neighbors(g,v);
  while(cpt<(int)number_neighbors1){
    struct vertex vertex_recover=vertex_same_color1[cpt];
    cpt++;
    enum color_t recover_color=vertex_recover.color_vertex;
    int data=vertex_recover.data;
    update_graph_colors__0(g,(size_t)data,color);
  }
  return;
}











    

  
  




      
