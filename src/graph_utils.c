
#include <stdlib.h>
#include <stdio.h>
#include "graph_utils.h"

enum direction{ NORTH = 0, SOUTH = 1, WEST = 2, EAST = 3 };

const int dx[4] = {0, 0, -1, 1};

const int dy[4] = {-1, 1, 0, 0};

struct point{ 
  int x; 
  int y; 
};

int coords_to_index(struct point coords, int n){ 
  return coords.x + coords.y * n; 
}

struct point index_to_coords(int index, int n){ 
  return ((struct point) {index % n, index / n}); 
}
struct point coords_of_neighbor(struct point coords, enum direction dir){ 
  return ((struct point){coords.x + dx[dir], coords.y + dy[dir]}); 
}

struct graph * create_square_graph(int n){
    struct graph * g = graph__empty(n * n);

    graph__set_positions(g, 0, n * n - 1);

    //Central square
    for (size_t x = 1; x < n - 1; x++){
        for (size_t y = 1; y < n - 1; y++){
            struct point coords = {x, y};
            graph__add_edge(g, coords_to_index(coords_of_neighbor(coords, NORTH), n), coords_to_index(coords, n));
            graph__add_edge(g, coords_to_index(coords_of_neighbor(coords, SOUTH), n), coords_to_index(coords, n));
            graph__add_edge(g, coords_to_index(coords_of_neighbor(coords, WEST), n), coords_to_index(coords, n));
            graph__add_edge(g, coords_to_index(coords_of_neighbor(coords, EAST), n), coords_to_index(coords, n));
        }
    }
    //North border
    for (size_t x = 0; x < n - 1; x++){ 
      graph__add_edge(g, coords_to_index(coords_of_neighbor(((struct point){x, 0}), EAST), n), coords_to_index(((struct point){x, 0}), n)); 
    }
    //East border
    for (size_t y = 0; y < n - 1; y++){ 
      graph__add_edge(g, coords_to_index(coords_of_neighbor(((struct point){n - 1, y}), SOUTH), n), coords_to_index(((struct point){n - 1, y}), n)); 
    }
    //South border
    for (size_t x = 1; x < n; x++){ 
      graph__add_edge(g, coords_to_index(coords_of_neighbor(((struct point){x, n - 1}), WEST), n), coords_to_index(((struct point){x, n - 1}), n)); 
    }
    //West border
    for (size_t y = 1; y < n; y++){ 
      graph__add_edge(g, coords_to_index(coords_of_neighbor(((struct point){0, y}), NORTH), n), coords_to_index(((struct point){0, y}), n)); 
    }

    graph__compress(g);
    return g;
}

struct graph * create_torus_graph(int n){
    struct graph * g = graph__empty(n * n);

    graph__set_positions(g, 0, (n / 2) * n + n / 2);
    //the right coefficient for displaying the graph position players after.//

    /*ca reste la meme chose*/
    //Central square
    for (size_t x = 1; x < n - 1; x++){
        for (size_t y = 1 ; y < n - 1; y++){
            struct point coords = {x, y};
            graph__add_edge(g, coords_to_index(coords_of_neighbor(coords, NORTH), n), coords_to_index(coords, n));
            graph__add_edge(g, coords_to_index(coords_of_neighbor(coords, SOUTH), n), coords_to_index(coords, n));
            graph__add_edge(g, coords_to_index(coords_of_neighbor(coords, WEST), n), coords_to_index(coords, n));
            graph__add_edge(g, coords_to_index(coords_of_neighbor(coords, EAST), n), coords_to_index(coords, n));
        }
    }
    //gestion des bords comme pour le graphe carre en utilisant que
    //des appels de fonctions.(modularite)


    //part1:OK.
    //North border
    for (size_t x = 0; x < n - 1; x++){ 
      graph__add_edge(g, coords_to_index(coords_of_neighbor(((struct point){x, 0}), EAST), n), coords_to_index(((struct point){x, 0}), n)); 
    }
    //East border
    for (size_t y = 0; y < n - 1; y++){ 
      graph__add_edge(g, coords_to_index(coords_of_neighbor(((struct point){n - 1, y}), SOUTH), n), coords_to_index(((struct point){n-1, y}), n)); 
    }
    //South border
    for (size_t x = 1; x < n; x++){ 
      graph__add_edge(g, coords_to_index(coords_of_neighbor(((struct point){x, n - 1}), WEST), n), coords_to_index(((struct point){x, n - 1}), n)); 
    }
    //West border
    for (size_t y = 1; y < n; y++){ 
      graph__add_edge(g, coords_to_index(coords_of_neighbor(((struct point){0, y}), NORTH), n), coords_to_index(((struct point){0, y}), n)); 
    }
    //avec tout cela on a un graphe carre
    //pour obtenir un graphe torique, il suffit d'y ajouter des trucs:
    //toute la bordure nord est reliee a la sud
    //et l'ouest avec l'est symetriquement on peut acceder par un depassement
    //d'indices-- du nord au sud sur le point.y's coords
    //et de l'ouest à l'est par un depassement d'indices-- depuis points.x's
    //coords. Avec la symetrie deja tenue en compte tout au long du projet
    //l'acces a l'est depuis l'ouest avec un depassement d'indice
    //sur y ++ est possible et meme chose pour le nord depuis le sud avec un
    //depassement d'indice sur x en ++.

    //La bordure nord.
    for (size_t x = 0; x < n; x++){ 
      graph__add_edge(g, coords_to_index(((struct point){x, n - 1}), n), coords_to_index(((struct point){x, 0}), n)); 
    }
    
    //convention de la matrice GSL:
    //y vers le dessous et x vers la droite pour reperer les coordonnees
    //des points en question.
    //donc avec cela, chaque element repere par un {x,0} doit etre relie
    //en plus(deja fait avant) de coords_of_neighbor((struct point){x,0},EAST,n)
    //à {x,n-1} tout simplement.
    
     //La bordure ouest.
    for (size_t y = 0; y < n; y++){ 
      graph__add_edge(g, coords_to_index(((struct point){n - 1, y}), n), coords_to_index(((struct point){0, y}), n)); 
    }
    //ici chaque (0,y) va etre relie de l'autre cote avec (n-1,y) y reste
    //le meme comme x ci-dessus.
    
    graph__compress(g);
    return g;
}
/*preparating for generating torus graph example.(tests)*/

void coloring_graph_randomly(struct graph * g){

  for(size_t i = 0; i < graph__get_number_of_vertices(g); i++){
    graph__set_color(g, i, rand() % MAX_COLOR);
  }
}

int RBG[MAX_COLOR] = {0xFF0000, 0x0000FF, 0x00FF00, 0x00FFFF, 0x007FFF, 0xFF007F, 0xFFFF00, 0xFF00FF};

void display_graph_as_square(struct graph * g, int width, int is_sdl){
    printf("#\n");
    
    for(int line = 0; line < width; line++){
        for(int col = 0; col < width; col++){
	  if(is_sdl){ 
	    printf("%d ", RBG[graph__get_color(g,line*width+col)]);
	  }else{ 
	    printf("%d ", graph__get_color(g,line*width+col));
	  }
        }
        printf("\n");
    }
}

void display_graph_as_torus(struct graph * g, int width, int is_sdl){
    printf("#\n");

    for(int line = 0; line < width; line++){
        for(int col = 0; col < width; col++){
            if(is_sdl) printf("%d ", RBG[graph__get_color(g, line * width + col)]);
            else printf("%d ", graph__get_color(g, line * width + col));
        }
        printf("\n");
    }
}

//les deux premieres parties sont finies de l'implementation(documentation
//pour les autres partie graphe)
//a savoir pour les fonctions non elementaires:
//create_square/torus_graph from n=$1 num_vertices.sqrt() si 64 par exemple
//ou 16 -m 4 dans les tests de la forge.
//et displaying_graph pour une visualisation sdl
//a savoir: display_graph_as_square and display_graph_as_torus.
//le coloriage inclus aussi entre les deux bien sur(1ere frame)
//via: coloring_graph_randomly.
//Autres algos de coloriage de graphe apres comme d'habitude.(APRES la documen-
//tation)


int * construct_list(int size, int value){
    int *L = malloc(sizeof(int)*size);
    
    for (int i = 0; i < size; i++){
      L[i] = value;
    }
    
    return L;
}
/*
construit un simple tableau sous forme de pointeur comme type de retour sur un entier avec toutes les valeurs mises a value.
*/

void display_size_t_list(struct size_t_list input, int is_sdl){
    printf("[");
    
    if(input.size == 0){ 
      printf("]\n"); 
      return; 
    }

    for(size_t i = 0; i < input.size - 1; i++){
      printf("%u ", input.list[i]);
    }
    
    printf("%u", input.list[input.size - 1]);
    printf("]\n");
}
/*
un simple display de la composante connexe(de type struct size_t_list) si on utilise un affichage sdl (plus pousse)
*/


//Cette fonction retourne le tableau des indices dans bool_list qui sont differents de 0 ainsi par exemple le retour.list contiendrait les indices de tous les elements dont la taille fait size qui dans bool_list sont differents de 0
struct size_t_list compress_bool_list_with_size(int * bool_list, size_t length, size_t size){
    struct size_t_list compressed = {size, construct_list(size, 0)};
    size_t index = 0;
    
    for(size_t i = 0; i < length; i++){
        if(bool_list[i]) compressed.list[index++] = i;
    }
    
    return compressed;
}

//meme chose mais ayant que le length relatif a bool_list uniquement. Utilise la fonction precedente.(c'est comme une generalisation via une specification de la fonction precedente avec le parametre size qui fait reference en nombre de 1 dans le tableau bool_list bien sur(indiquee avant comme remarque.))
struct size_t_list compress_bool_list(int * bool_list, size_t length){
    size_t size = 0;
    for(size_t i = 0; i < length; i++) if(bool_list[i]){
	size++;
      }

    return compress_bool_list_with_size(bool_list, length, size);
}

//Les 3 premieres fonctions liees entre elles preliminaires pour la suite.











size_t DFS_color(struct graph * g, enum color_t color, int *visited, size_t from){
    //printf("%lu dfs\n", from);
  if(graph__get_color(g, from) != color || visited[from]){
    return 0;
  }
    //printf("%lu pass\n", from);
  visited[from] = 1; 
  struct size_t_list neighbors = graph__get_neighbors(g, from);
  //display_size_t_list(neighbors);
  size_t counter = 1;

  for (size_t i = 0; i < neighbors.size; i++){
    counter += DFS_color(g, color, visited, neighbors.list[i]);
  }
  return counter;
}

struct size_t_list get_component(struct graph * g, size_t from){
    size_t n = graph__get_number_of_vertices(g);
    int * visited = construct_list(n, 0);
    size_t component_size = DFS_color(g, graph__get_color(g, from), visited, from);
    struct size_t_list component = compress_bool_list_with_size(visited, n, component_size);

    free(visited);
    return component;
}

struct size_t_list get_component_outer_border(struct graph * g, size_t from){
    int n = graph__get_number_of_vertices(g);
    enum color_t color = graph__get_color(g, from);
    struct size_t_list component = get_component(g, from);
    int* border = construct_list(n, 0);
    size_t counter = 0;

    for (size_t i = 0; i < component.size; i++){
        struct size_t_list neighbors = graph__get_neighbors(g, component.list[i]);
        for (int j = 0; j < neighbors.size; j++){
            if (graph__get_color(g, neighbors.list[j]) != color && !border[neighbors.list[j]]){ 
	      border[neighbors.list[j]] = 1; 
	      counter++; 
	    }
        }
    }

    free(component.list);
    struct size_t_list compressed_border = compress_bool_list_with_size(border, n, counter);
    free(border);
    return compressed_border;
}

size_t component_intersection_size(struct size_t_list component1, struct size_t_list component2){
    size_t final_size = 0;
    size_t i1 = 0;
    size_t i2 = 0;

    while(i1 < component1.size && i2 < component2.size){
      if(component1.list[i1] < component2.list[i2]){
	i1++;
      }else{
	if(component1.list[i1] > component2.list[i2]){
	  i2++;
        }else{
	  if(component1.list[i1] == component2.list[i2]){
            i1++; 
	    i2++; 
	    final_size++;
	  }
	}
      }
    }

    return final_size;
}

struct size_t_list component_intersection(struct size_t_list component1, struct size_t_list component2){
    size_t final_size = component_intersection_size(component1, component2);
    struct size_t_list intersection = {final_size, construct_list(final_size,0)};
    size_t i1 = 0;
    size_t i2 = 0;
    size_t index = 0;

    while(i1 < component1.size && i2 < component2.size){
        if(component1.list[i1] < component2.list[i2]) i1++;
        else if(component1.list[i1] > component2.list[i2]) i2++;
        else if(component1.list[i1] == component2.list[i2]){
            intersection.list[index] = component1.list[i1];
            i1++; i2++; index++;
        }
    }
    return intersection;
}












//interactions debut avec les joueurs.(application des precedentes
//fonctions a savoir get_component pour recuperer la composante connexe
//depuis une couleur avec le parametre from qui est de type un vertex(sommet))
//a part dans la documentation.


void coloring_graph_from_player(struct graph * g, int player, enum color_t color){
    struct size_t_list component = get_component(g, graph__get_positions(g).n[player]);
    for (size_t i = 0; i < component.size; i++) graph__set_color(g, component.list[i], color);
    free(component.list);
}

int is_touching_opponent(struct graph * g){
    int n = graph__get_number_of_vertices(g);
    struct size_t_list player0_out_border = get_component_outer_border(g, graph__get_positions(g).n[0]);
    struct size_t_list player1_component = get_component(g, graph__get_positions(g).n[1]);
    size_t inter_size = component_intersection_size(player0_out_border, player1_component);
    free(player0_out_border.list);
    free(player1_component.list);
    return (inter_size > 0);
}

int number_vertices_from_player(struct graph * g, int player){
    struct size_t_list player_component = get_component(g, graph__get_positions(g).n[player]);
    size_t number = player_component.size;
    free(player_component.list);
    return number;
}

void number_vertices_winnable_from_player(struct graph * g, int player, int result_by_color[]){

  for(int i = 0; i < MAX_COLOR; i++){
    result_by_color[i] = 0;
  }

  int n = graph__get_number_of_vertices(g);
  struct size_t_list player_out_border = get_component_outer_border(g, graph__get_positions(g).n[player]);
  int* visited = construct_list(n, 0);

  for(size_t i = 0; i < player_out_border.size; i++){
    size_t vertex = player_out_border.list[i];
    if (!visited[vertex]){
      struct size_t_list temp_component = get_component(g, vertex);
      enum color_t component_color = graph__get_color(g, vertex);
      result_by_color[component_color] += temp_component.size;
      struct size_t_list inter = component_intersection(player_out_border, temp_component);
      for (size_t j = 0; j < inter.size; j++){
	visited[inter.list[j]] = 1;
      }
      free(temp_component.list);
      free(inter.list);
    }
  }

  free(player_out_border.list);
  free(visited);
}
