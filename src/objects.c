#include "objects.h"
#include "parsing.h"

void ob_init(){
  io_getLevels(&ob_levels, "data");
}

void ob_deinit() {
  for(int i=0;i<127;i++){
    if(io_os[i].ps != NULL){
      for(int j=0;j<io_os[i].nps;j++){
        free(io_os[i].ps[j]);
      }
      free(io_os[i].ps);
      free(io_os[i].cols);
    }
    if(ob_levels[i] != NULL){
      free(ob_levels[i]);
    }
  }
  free(ob_levels);
}

obj ob_objFchar(char c){
  return io_os[c];
}

void ob_printPoint(point p){
  printf("{ x: %d, y: %d, c:%c }\n", p.x, p.y, p.c);
}

// Shift by it's coordinate position in the world
void ob_realifyPoint(point *p, int x, int y){
  (*p).x += x;
  (*p).y += y;
  return;
}

void ob_realifyBox(box *b, int x, int y){
  (*b).x += x;
  (*b).y += y;
  return;
}

// Rounds to the nearest tenth. Not sure why I made this function, and it's not called anywhere.
float f_round(float f){
  return( (((int) (f*10 + ((f > 0) ? 0.5: -0.5))))/10.0);
}

// Need some way to terminate the array of points, so I've made a specific value that is the terminator. This tests if it's that.
// Won't conflict with any actual points, because '\0' is not a valid color
bool ob_p_isTerm(point p){
  return (p.x == 124214 && p.y == 143512 && p.c == '\0');
}

bool ob_p_isSkip(point p){
  return (p.x == 124214 && p.y == 141312 && p.c == '\0');
}
