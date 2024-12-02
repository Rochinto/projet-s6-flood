#include "player.h"
//#include "graph_impl.h"
#include "graph_basic.h"
#include "graph_utils.h"

typedef struct player{
  size_t identifiant;
  const char* nom;
  struct color_set_t couleurs_interdites;
  struct graph* g;
}player;

player player_en_cours={};//vide pour l'instant.
//reprenons les fonctions fondamentales de jeu;a savoir: ceux de player.h.

char const* get_player_name(){
  char* player_name="PLAYER_RANDOM_MOVE";
  return (char const*)player_name;
}

void initialize(size_t id, struct graph_t* graph,
                enum color_t* colors,
                struct color_set_t forbidden[NUM_PLAYERS]){
  player_en_cours.identifiant=id;
  player_en_cours.g=graph__merge_from(graph,colors);
  //rappel: la fonction elementaire graph__merge_from a la fin
  //du fichier src/graph_basic.c permet de construire un struct graph
  //a partir d'un struct graph_t et d'une coloration possible dans ce dernier
  //tableau.
  player_en_cours.couleurs_interdites=forbidden[(int)id];
}


unsigned int recuperation_coups_valides(int valides_coups[], int couleur_adversaire){
  //on va utiliser la fonction number_vertices_winnable_from_player qui permet
  //de modifier par effet de bord le tableau rslt_by_color qui repertorie
  //en indice=(int)COLOR_GIVEN le nombre de sommets qu'on peut gagner avec
  //(le nombre de sommets dans la plus grande composante connexe des bords
  //en terme de chaque sommet).

  int n=0;
  int index_colors_got_component[MAX_COLOR];
  number_vertices_winnable_from_player(player_en_cours.g,player_en_cours.identifiant,index_colors_got_component);
  //modifie maintenant.
  int j=0;
  while(j<(int)MAX_COLOR){
    int is_ok=100;
    if(player_en_cours.couleurs_interdites.t[j]!=0) is_ok=0;
    //dans ce cas, dans le tableau des couleurs interdites de type char*
    //en l'indice j on a une valeur differente de 0
    //reprenons le tableau est une case a booleens de grandeur MAX_COLOR
    //evidemment si en j qui represente une couleur on a une valeur!==0
    //=>elle est egale a 1=>j qui represente une couleur par (type casting
    //enum color_t) est interdite a ce joueur.VOILA./////

    //on rappelle que par effet de bord cette fonction
    //repertorie en l'indice (int)color_J le nombre de sommets
    //pouvant etres gagnes en jouant ce sommet (valider le coup avec
    //cette couleur pas interdite) dont l'appartenance a la bordure
    //est OK et a la plus grande couverture en terme de composante
    //connexe a partir de ce sommet la bien sur.
    else if(index_colors_got_component[j]<=0)  is_ok=0;
    //plutot =0 cad. que cette couleur ne fait gagner rien.. meme si
    //elle est permise,d'apres les regles ; on LA JOUE PAS.

    else if(is_touching_opponent(player_en_cours.g) && (j==couleur_adversaire)) is_ok=0;
    //si la couleur que ce joueur veut jouer en cours est permise
    //et fait gagner des points depuis un sommet de meme couleur adjacent dont
    //la couverture est la plus grande mais malheureusement :
    //cette couleur en question est la couleur de l'adversaire qui peut etre
    //dans la bordure de l'espace gagnant ET EN plus elle touche la composante connexe en cours de l'autre (condition pas suffisante car il se peut que la couleur se joue et fait gagner totalement ce dernier avec le r alors qu'au debut ce r etait adjacent a la composante connexe de l'adversaire perdant dont la couleur etait bleue.(on ajoute les 2 conditions pour que ca soit clair et nette)

    if(is_ok) valides_coups[n++]=j;
    j++;
  }
  return n;
}

    
//on reprend les toutes 2 dernieres fonctions:
struct move_t play(struct move_t previous_move){
  //on va jouer sur le player_courant...
  //au debut on fait un {NO_COLOR} comme ca c'est le deuxieme qui va commencer
  //la bataille..
  //on utilisera pour cela la fonction coloring_graph_from_player
  //qui va colorier le graph (updating) en tenant compte de la couverture
  //du joueur oppose (pas de contraintes tout AU DEBUT).
  
  coloring_graph_from_player(player_en_cours.g,(int)(1-player_en_cours.identifiant),previous_move.c);//$3 fait reference a la couleur apparemment le choix de l'autre par cette fonction retournera avec des petites modifs les autres choix permis dans un espace large(.g) mis a jour...
  //RQ: forbidden={} dans le serveur pour ne pas developper trop cette fonction.

  int eventuels_coups[MAX_COLOR];
  int number_valides_coups=recuperation_coups_valides(eventuels_coups,previous_move.c);//va renvoyer a la fin apres avoir modifier eventuels_coups qui stockera dans chaque indice'' les gains en jouant {(enum color_t)$cet_indice$} la longueur de ce dernier a savoir le nombre de couleurs permises rapportant des gains pas a la frontiere de l'adversaire et pas de meme couleur que l'adversaire dans la partie en cours.=>rand()%n en terme d'indice choisira la couleur a jouer
  //parmi les indices possibles .
  struct move_t mise_0={NO_COLOR};
  if(number_valides_coups>0) mise_0.c=eventuels_coups[rand()%number_valides_coups];
  coloring_graph_from_player(player_en_cours.g,(size_t)player_en_cours.identifiant,mise_0.c);//le coup joue par le player_en_cours suivant ce contexte de CETTE FONCTION avec le coup representee par la couleur jouee qui sera move.c qu'on vient de modifier si n le nombre de couleurs 'PERMISES'>=0&&!=0.
  return mise_0;//on retourne le coup joue ainsi avec le serveur_impl.c ou bien le serveur.c enrichie on pourrait via le Makefile tester cette 3ieme strategie disons la 2ieme car le min et max est en cours FAIRE EN sorte que cette fonction retourne le coup joue par l'adversaire lors du prochain sous-tour.
}


void finalize(){
  graph__free(player_en_cours.g);
  //la seule ressource d'adresses a free suivant la fonction elementaire
  //dans src/graph_basic.c: graph__free qui:
  //fait un free pour :
  //1->gsl_spmatrix_uint_free(g->graph->t)
  //2: free(g->graph)
  //3&&4: free(g->colors);free(g). (g de type struct graph* dans graph_struct.h)
}




//execution:
//gcc  -std=c99 -I/net/ens/renault/save/gsl-2.6/install/include -L/net/ens/renault/save/gsl-2.6/install/lib ./src/player_random.c ./src/graph_basic.c ./src/graph_utils.c ./src/main_testing.c -lgsl -lgslcblas -lm pour un test de compilation rapide & efficace.
