#ifndef _FLOOD_COLOR_H_
#define _FLOOD_COLOR_H_

enum color_t {
  BLUE=0, RED=1, GREEN=2, YELLOW=3,
  ORANGE=4, VIOLET=5, CYAN=6, PINK=7,
  MAX_COLOR=8,
  NO_COLOR=-1,     // Color when no move is possible
};

struct move_t {
  enum color_t c;
};

const char* color_to_string(enum color_t c);

struct color_set_t {
  char t[MAX_COLOR];
};

#endif // _FLOOD_COLOR_H_
