#include "graph_basic.h"
#include "graph_struct.h"
#include <stdio.h>

/*creation d'un graph empty
de n sommets ; suivant les tests deja ecrits 
si n<=0 on doit retourner le pointeur NULL sinon
on alloue la structure graph (sizeof(*graph) c'est prefere que sizeof(struct
graph*) comme on a vu en C avance)
apres on doit remplir les champs ; a savoir:
graph_t,colors,is_compressed.
au debut , is_compressed est mise a 0.
le colors est alloue car il s'agit d'un tableau de couleurs avec:
sizeof((enum color_t)*((int)n)), reste graph de type graph_t.
Pour celui la il faut commencer par l'allouer pour pouvoir acceder aux champs
qui devront etre modifies.
Pour cela, on commence par l'allouer normalement ; apres cela:
on graph->graph->num_vertices=n 
puis graph->graph->t=gt avec gt une matrice de format coo allouee
avec gsl_uint_graph_alloc(n,n) car c'est matrice d'adjacense donc de 
taille $1*$1 puis les positions a 0 au debut ; a savoir les 2 instructions:
graph->graph->positions[0||1]=0.
(sans oublier de remplir le graph->colors par des NO_COLOR au debut)
*/

struct graph* graph__empty(size_t n){

  if(n <= 0){
    return NULL;
  }

  struct graph* graph = (struct graph*)malloc(sizeof(*graph));
  struct graph_t* gt = (struct graph_t*)malloc(sizeof(*gt));
  gt->num_vertices = n;
  gt->t = gsl_spmatrix_uint_alloc(n, n); //remplie par des zeros des l'allocation de la matrice d'adjacense gsl.
  gt->positions[0] = 0;
  gt->positions[1] = 0;
  graph->colors=(enum color_t*)malloc(n*sizeof(enum color_t));

  for(int j = 0; j < n; j++){
    graph->colors[j] = NO_COLOR;
  }

  graph->is_compressed = 0;
  graph->graph = gt;
  return graph;
}

/*
faire un free pour le graph;procedont par profondeur:
le pointeur alloue tout au fond est g->graph->t => free avec gsl_spmatrix_uint
_free's function puis g->graph (aucune dans la structure g->graph reste a free)
puis reste 2 choses: le tableau couleurs g->colors et g en entier tout a la fin
(voir la fonction graph_empty pour plus de details)
*/


void graph__free(struct graph* g){
  gsl_spmatrix_uint_free(g->graph->t);
  free(g->graph);
  free(g->colors);
  free(g);
}

/*
ajouter une arete entre a et b
via la matrice d'adjacense g->graph->t qui definit le graphe bien sur.
on utilise la fonction gsl_spmatrix_uint_set(g->graph->t,a,b,1)
et inversement(matrice symetrique car graphe nn oriente) 
PUIS ajouts des code erreurs comme on a deja fait en ateliers de programmation.
*/

int graph__add_edge(struct graph *g, size_t a, size_t b){

  if(a == b){
    return -1;
  }else{
    if(a >= g->graph->num_vertices || b >= g->graph->num_vertices){
      return -2;
    }
  }

  gsl_spmatrix_uint_set(g->graph->t, a, b,1);
  gsl_spmatrix_uint_set(g->graph->t, b, a,1); 
  return 0;//code erreur.($?)
}


/*
on recuperer les voisins de vertex qui est un sommet dans le graphe:
pour faire cela, on procede de la maniere suivante avec la fonction:
gsl_spmatrix_uint_get entre 2 vertexs pour savoir si il y a un lien
SI: vertex depasse g->graph->num_vertices donc impossible car vertex
n'appaartient pas dans ce cas aux sommets du graphe en question (code erreur ; 
voir avant) sinon on fera la procedure suivante:
on a deux cas:
graphe compresse NN (<=>g->is_compressed mis a 0<=>test: !g->is_compressed juste)
=>dans ce cas:
on fait un petit warning sur la sortie standard ; sinon:
on itere sur g->graph->num_vertices et a chaque fois
que: il y a 1 dans la ligne vertex c'est a dire: 
i le compteur entre 0 et g->graph->num_vertices-1 telque:
gsl_spmatrix_uint_get(g->graph->t,j,i)==1 (=>existe lien)
dans ce cas la list qui a ete allouee avec sizeof(*list) doit grandir
=>realloc avec (k+1)*(sizeof(unsigned int)) pour acceuillir les nouveaux voisins
et cette liste en k va recevoir la valeur i. k est incrementee et initialisee a
0 tout au debut pour gerer ces problemes de reallocation pour que le tableau
contient plus que les donnees prevues a savoir une (*list typeof c'est unsigned int ) tout au debut apres avoir termine de la verification du cas de base)).
on retourne (struct size_t_list){k,list} ou {0,NULL} dans le cas de base
la 1ere via un typecasting avec k l'element qui permet de parcourir dans 
le tableau list les voisins de meme couleur que vertex de depart ($2 de cette fonction (argv[2])) ; on aurait pu faire cela facilement car si g->is_compressed==0 ; cela voudrait dire que: g->graph->t est de format C00 et donc on dispose dans g->graph->t nommee tab d'une variable tab->nz mais on va detailler cela apres.
=>DANS LE CAS CONTRAIRE:
on dispose dans les graphes de matrices creuses d'adjacense bien sur
de plusieurs informations. Par exemple, p si tab=g->graph->t alors tab->p[vertex] represente le nombre de non zeros en parcourant toutes les lignes de la matrice d'adjacense jusqu'a vertex non inclus alors si on veut le nombre de non zeros dans la ligne vertex , il suffit de faire ce qui suit:
tab->p[vertex+1]-tab->p[vertex] qui represente le nombre de nn zeros a savoir le nombre de 1 dans la ligne vertex de la matrice d'adjacense:g->graph->t dans ce modele la, apres pour recupere ces voisins a savoir tous les j telque en (vertex,j) on a un 1, on utilise la variable i qui est pointe vers le debut des 1 a chaque fois pour cela dans le contexte de vertex, on recuperera la liste des voisins avec tab->i[start_index]<=>tab->i+start_index qui va indiquer le tableau des 1 pour le vertex dont la liste des 1 est suivant start_index=tab->p[vertex]. C'est dispose dans la construction gsl par passage au format compresse donc pas la peine de fournir toutes les fonctions deja faites meme si cela se teste evidemment.
*/

struct size_t_list graph__get_neighbors(struct graph* g, size_t vertex){
  //cette fonction doit recuperer les voisins de vertex.
  //vertex est un numero pour la numerotation de sommets.
  //on peut pour cela travailler avec: 
  //on inspecte la ligne dans la matrice d'adjacense numero vertex

  int k = 0;
  struct size_t_list list_error = {0, NULL};

  if(vertex >= g->graph->num_vertices){
    return list_error;
  }

  if(!g->is_compressed){ //Alors il faut libérer la liste des voisins + calcul plus long
    fprintf(stderr, "[WARNING] graph not compressed\n");
    unsigned int* list = (unsigned int*)malloc(sizeof(*list));

    for(size_t j = 0; j < g->graph->num_vertices ; j++){
      if(j != vertex && gsl_spmatrix_uint_get(g->graph->t,vertex,j) == 1){
        list = realloc(list,(k + 1)*sizeof(unsigned int));
        *(list+k) = j;
        k++;
      }
    }

    return (struct size_t_list) {k, list };
  }//else

  int start_index = g->graph->t->p[vertex];
  int length = g->graph->t->p[vertex + 1] - start_index;

  return (struct size_t_list) {length, (unsigned int*) (g->graph->t->i) + start_index };
}

/*
changer le coloriage de vertex:
facile: on doit changer g->colors en la position (int)vertex qui va recevoir
color.
avec des cas particuliers comme ceux documentes tout avant.
*/


int graph__set_color(struct graph *g, size_t vertex, enum color_t color){
  //if(color==NO_COLOR) return -3;
  if(color >= MAX_COLOR || color < 0 ){
    return -3;  //-3 pour depassement d'indices dans le tableau g->colors.
  }else{
    if(vertex >= g->graph->num_vertices){
      return -4;
    }else{
      g->colors[(int)vertex]=color;
      return 0;
    }
  }
}

/*
recuperer la couleur du vertex, partiellement faite toute avant.
*/

enum color_t graph__get_color(struct graph *g, size_t vertex){
  return g->colors[vertex];
}


/* 
important pour le serveur tout apres:
c'est faire des copies en particulier du graphe tout dessous;commencerait
par allouer un tableau de taille g->graph->num_vertices car chaque sommet
dispose d'une couleur forcement
;
puis ce tableau de couleurs allouee r en la position vertex 
avec vertex variant de 0 à g->graph->num_vertices va recevoir la valeur
g->colors[vertex] ainsi on a une copie parfaite de g->colors
en retour de fonction.
L'utilisation pour le serveur et les interactions avec le joueur seront dits 
apres.
*/

enum color_t* graph__get_colors_copy(struct graph *g){
  enum color_t* r = malloc(g->graph->num_vertices * sizeof(*r));

  for(size_t vertex = 0; vertex < g->graph->num_vertices; vertex++){
    r[vertex] = g->colors[vertex];
  }

  return r;
}

/*
recupere la position
de type struct doublon
des deux joueurs: g->graph->positions[0] et g->graph->positions[1]
avec un typecast pour que le retour soit de type struct doublon
(struct doublon) qui contient un tableau de size_t (respectees toutes les conditions pour CELA
*/

struct doublon graph__get_positions(struct graph *g){
  return (struct doublon) {{g->graph->positions[0], g->graph->positions[1]}};
}

/*
changer la position (voir l'utilisation ou au moins les tests fonctionnels/
structurels apres cela)
g->graph->positions est un tableau a 2 alors 
la 1ere case va etre p0 la deuxieme p1
pour changer la position des joueurs apres l'initialsiation d'un graph_empty dans l'utilisation des fonctions elementaires de graph_utils.c 
dans LE GRAPHE ainsi obtenue(generation d'un graphe carre par exemple).
*/

int graph__set_positions(struct graph *g, size_t p0, size_t p1){
  //changer les positions des joueurs.
  //à savoir: g->graph->positions[0]/g->graph->positions[1].
  if(p0 >= g->graph->num_vertices || p1 >= g->graph->num_vertices){
    return -5;
  }

  g->graph->positions[0] = p0;
  g->graph->positions[1] = p1;
  return 0;//code erreur($?)
}

/*
depuis g recupere l'information qu'on dispose pas dans la structure : le nombre de sommets a savoir dans graph.h:>> g->graph->num_vertices.
*/

size_t graph__get_number_of_vertices(struct graph *g){
  return g->graph->num_vertices;
}

/*
compresse le graphe:
pour ne pas avoir de probleme avec valgrind;on commence par maitriser les cas particuliers traites par les tests;a sa voir si le graph est deja compresse cad. g->graph->t->sptype=!=GSL_SPMATRIX_C00 on faire un return;
sinon le graphe n'est pas compresse=>traitements suivants:
mise a 1 de la variable g->is_compressed
&&
allocation g_compressed via une compression (gsl_spmatrix_uint_compress) d'argument le graph en question g-->graph->t avec $2 le flag habituel GSL_SPMATRIX_CSR puis on se debarasse de la variable g->graph->t pour eviter le undefinitely lost apres et on affecte g->graph->t a g_compressed deja traitee ci-dessus.
voila.///
*/



void graph__compress(struct graph *g){
  //creer une matrice creuse au format CSR.(compressed graph from graph format)
  //g doit etre de type COO.
  if (g->graph->t->sptype != GSL_SPMATRIX_COO){
    return;
  }

  gsl_spmatrix_uint* g_compressed = gsl_spmatrix_uint_compress(g->graph->t,GSL_SPMATRIX_CSR);
  gsl_spmatrix_uint_free(g->graph->t);
  g->graph->t = g_compressed;
  g->is_compressed = 1;
}

/*
recupere la structure graph_t de graph pour acceder eventuellement a la matrice d'adjacense comme modele d'exemple(voir tests pour plus de details).
*/


struct graph_t* graph__get_graph_t(struct graph* g){
  return g->graph;
}

/*
de meme mais l'information manquante du nombre de sommets via un gt de type struct graph_t et non pas struct graph.
*/

size_t graph_t__get_number_of_vertices(struct graph_t *gt){
  return gt->num_vertices;
}

/*
de meme comme precedemment pour recuperer la position des joueurs avec la meme procedure typecast suiavnt le type struct double et recuperation facile des 2 positions avec: g->positions[0] et g->positions[1].
*/


struct doublon graph_t__get_positions(struct graph_t *gt){
  return (struct doublon) {{gt->positions[0], gt->positions[1]}};
}

/*
et nous voila: une copie du graphe;
on alloue tout au debut un struct graph_t normale.
puis on remplit les champs , pour mieux montrer la flexibilite d'utiliser 
les donnees fournies par le modele GSL de ce projet , on procede de cette facon:
r allouee en r->t est une matrice d'ajacense requested
=>apres allocation,on fait un switch sur le type r->t->sptype;si COO:
=>=>
FAIRE CECI:
tab sera g->graph->t (matrice d'adjacense de format COO ici)
tab->nz le nombre des non zeros
on itere sur tab->nz(compteur de boucle element)
et a chaque fois on doit updater la matrice de format coo en ce tab->nz (i,j) par 1 pour cela on utilise les deux acces tab->i/tab->p via tab->p[element] qui donnera la ligne i et tab->i[element] qui donnera la colonne en question pour positionner le 1.
SINON; FORMAT CSR, procedons de la facon suivante:
on fait une boucle sur line de 0 à g->graph_t->num_vertices 
et pour chaque 'ligne' dans la matrice d'adjacense il y a:
tab->p[line+1]-tab->p[line] non zeros a changer
donc une autre boucle sur cela puis pour faire le changement on utilise les memes fonctions avec comme parametres (i,j) dont la position---valeur va changer est: tab->i[element],line avec element le deuxieme compteur de la boucle QUIVARIE
de tab->p[line] à tab->p[line+1].(principe de remonter a l'information precedente si clair dans el code suiavnt)
*/


struct graph_t* graph__get_graph_t_copy(struct graph *g){
  struct graph_t* r = malloc(sizeof(*r));
  r->num_vertices = g->graph->num_vertices;
  r->t = gsl_spmatrix_uint_alloc(g->graph->num_vertices, g->graph->num_vertices);
  r->positions[0] = g->graph->positions[0];
  r->positions[1] = g->graph->positions[1];
  //r->num_vertices = g->graph->num_vertices;

  gsl_spmatrix_uint* tab = g->graph->t;

  if(tab->sptype == GSL_SPMATRIX_COO){
    for(size_t element=0; element < tab->nz; element++){
      gsl_spmatrix_uint_set(r->t, tab->i[element], tab->p[element], 1);
    }
  }else{
    if(tab->sptype == GSL_SPMATRIX_CSR){
      for(size_t line=0; line < g->graph->num_vertices; line++){
	for(int element=tab->p[line]; element < tab->p[line+1]; element++) gsl_spmatrix_uint_set(r->t, tab->i[element], line, 1);
      }
    }else{
      return NULL;
    }
  }

  gsl_spmatrix_uint* g_compressed=gsl_spmatrix_uint_compress(r->t, GSL_SPMATRIX_CSR);
  gsl_spmatrix_uint_free(r->t);
  r->t = g_compressed;
  return r;
  //ces dernieres instructions ne sont pas importantes ca refait la meme chose mais ca pourrait specifier qu'en cas ou le graphe n'est pas compresse(fait en sorte de retourner un format compresse tjrs).
}

/*
cette fonction permet de construire une structure graphe a partie de la donnee d'une structure graph_t qu'on avait tout au debut avec les headers,il faut bien gerer les couleurs car cela pose un probleme d'effet de bord visualise dans les tests tout apres. VOILA.//
 */

struct graph* graph__merge_from(struct graph_t *gt, enum color_t *colors){
  struct graph *g = malloc(sizeof(*g));
  g->graph = gt;
  g->colors = colors;

  if(gt->t->sptype == GSL_SPMATRIX_COO){
    g->is_compressed = 0;
    graph__compress(g);
    return g;
  }else if(gt->t->sptype == GSL_SPMATRIX_CSR){
    g->is_compressed = 1;
    return g;
  }
  return NULL;
}
