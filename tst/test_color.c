#include "../src/color.h"
#include <stdio.h>

int main(int argc, char* argv[]){
  printf("%s--%s--%s\n\n",color_to_string(BLUE),color_to_string(GREEN),color_to_string(ORANGE));
  printf("%s--%s--%s\n\n",color_to_string(RED),color_to_string(YELLOW),color_to_string(VIOLET));
  return 0;
}
