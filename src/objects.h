#ifndef _OBJECTS_H
#define _OBJECTS_H
#include "helpers.h"

//TODO: remove need for k_nMax___

#define k_nMaxObj 1000 //maximum number of objects in a world
#define p_termPoint ((point){ .x=124214, .y=143512, .c = '\0'}) //Hope this point never actually comes up... :P
#define p_skipPoint ((point){ .x=124214, .y=141312, .c = '\0'}) //Hope this point never actually comes up... :P

typedef struct {
  unsigned char r;
  unsigned char g;
  unsigned char b;
} color;

typedef struct {
  int x;
  int y;
  char c;
} point;

typedef struct {
  int x;
  int y;
  int w;
  int h;
} box;

typedef struct{
  int x;
  int y;
  double vx;
  double vy;
  bool gravity;
  bool physical;
  int i;
  int j;
  char c;
  bool active;

  bool hidden;
  bool flip;
  int animFrame;
  bool onScreen;

  char type;
  point** ps;
  int nps;
  box bb;
  box* cols;
  int nCols;
} obj;

obj ob_objFchar(char);

//typedef struct {
//obj* os;
//color* cs;
//int* checks;
//} level;

typedef obj* level;
level* ob_levels;

void ob_init();
void ob_deinit();

void ob_printPoint(point);
void ob_realifyPoint(point*, int, int);
void ob_realifyBox(box*, int, int);
void ob_objectifyCam(int*, int*);

bool ob_p_isTerm(point);
bool ob_p_isSkip(point);

#endif

