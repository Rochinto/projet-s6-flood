#include"color.h"
#include"color_impl.h"

Bool is_in_array(enum color_t color, struct color_set_t* colors_set){
  for(int i=0;colors_set->t && *(colors_set->t)!='\0';i++){
    if(color==colors_set->t[i]) return True;
  else return False;
  }}

const char* color_to_string(enum color_t c){
  char* colors[] = {"BLUE","RED","GREEN","YELLOW","ORANGE","VIOLET","CYAN","PINK","MAX_COLORS","NO_COLOR"};
  return colors[(int)c];
}
