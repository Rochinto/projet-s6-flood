#include "../src/graph.h"
#include<stdio.h>
#include<stdlib.h>
#include<getopt.h> //sera prise en compte apres.


//fichier qui va tester quelques fonctionnalitees de la nature du graphe
//impose dans graph.h.


struct graph {
  size_t num_vertices; 
  gsl_spmatrix_uint* t; 
  size_t positions[NUM_PLAYERS]; 
};
//type incomplet..
//rappels des fonctions gsl utiles (objet de ce test):
//gsl_spmatrix_uint_malloc(size_t size1, size_t size2)
//gsl_spmatrix_free(gsl_spmatrix_uint*)
//gsl_spmatrix_uint_set
//(gsl_spmatrix_uint*,(coordonées)((couple)),   value:double)
//gsl_spmatrix_uint_get
//(gsl_spmatrix_uint*,(coordonées)((couple))):double.



/*const char* color_to_string(enum color_t c){
  char* colors[]={"BLUE","RED","GREEN","YELLOW","ORANGE","VIOLET","CYAN","PINK","MAX_COLORS","NO_COLOR"};
  return colors[(int)c];
  }*/
//que des doubles(on parle de types) => pas de visualisation des strings representant directement "la grille".=>
//les couleurs sur l'affichage final par make(version1).
  
int main(int argc, char* argv[]){
  //initialisation d'un modele de dessin d'un graphe.
  struct graph* graph_example=(struct graph*)malloc(sizeof(struct graph));
  graph_example->num_vertices=64;

  graph_example->t=gsl_spmatrix_uint_alloc(8,8);

  //graphe_example->positions est un tableau de long unsigned int
  //qui code en premier element 0/1 (id du premier joueur)
  //et en deuxieme (dernier)element 1-l'autre (id de l'autre joueur).
  graph_example->positions[0]=1;//Le 1er joueur va etre celui avec id=1.
  graph_example->positions[1]=0;//Le 2ieme joueur va etre celui avec id=0.
  

  //le graphe a 64 points connexes lies avec une matrice t .
  //step2: generate now new values of matrix elements

  double val;
  for(int i=0;i<8;i++){
    for(int j=0;j<8;j++){
      enum color_t random_color=rand()%(MAX_COLOR+1);
      //renvoie une valeur aleatoire entre:
      //0 et 8.
      val=(double)random_color;
      //pas recommandee:
      //val=color_to_string(random_color);;
      gsl_spmatrix_uint_set(graph_example->t,i,j,val);
    }
  }

  //petit affichage pour verification:
  for (int k=0;k<8;k++){
    for(int h=0;h<8;h++){
      val=gsl_spmatrix_uint_get(graph_example->t,k,h);
      //printf("graph_example->t(%d,%d) = %5.2f ",k,h,val);
      printf("%5.2f",val);
    }
    printf("\n");
  }

  //autres verifications:
  printf("************************************\n");
  printf("ordre des joueurs:\n---------------:\t");
  printf("(%ld,%ld)",graph_example->positions[0],graph_example->positions[1]);
  printf("\nLe premier joueur est celui dont l'ID est ==  %ld\n",graph_example->positions[0]);
  printf("Le deuxieme joueur est celui dont l'ID est ==  %ld\n\n",graph_example->positions[1]);
  printf("\n_____________________NOMBRE DE POINTS (sommets du graphe(via *t)):");
  printf("%ld\n\n\n",graph_example->num_vertices);

  //faire un free pour la structure graph_example->t PUIS struct graph_t*.
  gsl_spmatrix_uint_free(graph_example->t);
  free(graph_example);
  
  return 0;
}
