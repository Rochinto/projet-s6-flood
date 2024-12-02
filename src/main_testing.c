#include<stdio.h>
#include<stdlib.h>


typedef struct list{
  int data;
  struct list* next;
}list;

typedef struct test__1{
  size_t position;
  struct list* vertex;
}test__1;

enum color{BLUE=0,RED=1,GREEN=2,NO_COLOR=-1};

int main(){
  return 0;
}
/*
gcc  -std=c99 -I/net/ens/renault/save/gsl-2.6/install/include -L/net/ens/renault/save/gsl-2.6/install/lib ./src/player_impl.c ./src/graph_impl.c ./src/main_testing.c ./src/color_impl.c -lgsl -lgslcblas -lm

pour tester les modules entre eux(tests d'integrations (plus que fonctionnels))

*/
