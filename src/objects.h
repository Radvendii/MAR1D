#ifndef _OBJECTS_H
#define _OBJECTS_H
#include "helpers.h"

//TODO: remove need for k_nMax___

#define k_nMaxObj 1000 //maximum number of objects in a world
// Special points. Won't conflict with actual points because '\0' is not a valid colour.
#define p_termPoint ((point){ .x=124214, .y=143512, .c = '\0'}) // Point which terminates the array of points.
#define p_skipPoint ((point){ .x=124214, .y=141312, .c = '\0'}) // Special point that is simply skipped. This is useful so that all frames of an animation can be made to have the same number of points.

// Sometimes it's more convenient to define a color by RGB(0xRRGGBB)
#define RGB(x) (color) {                                                \
    .r = ((x) & 0xFF0000) >> 16,                                        \
    .g = ((x) & 0x00FF00) >> 8,                                         \
    .b = ((x) & 0x0000FF) >> 0                                          \
  }
#define k_colorTextLit RGB(0xF0F0F0)
#define k_colorTextDim RGB(0x404040)
#define k_colorTextMed RGB(0x888888)

typedef struct {
  unsigned char r;
  unsigned char g;
  unsigned char b;
} color;

typedef struct {
  int x;
  int y;
  char c; // Colour
} point;

typedef struct {
  int x;
  int y;
  int w;
  int h;
} box;

typedef struct{
  // Position
  int x;
  int y;

  // Velocity
  double vx;
  double vy;

  bool gravity; // Affected by gravity
  bool physical; // Should collisions register
  bool active; // Whether they should act / move.
  bool hidden; // Whether to render
  bool onScreen; // Is it visible to the player
  bool flip; // Whether to flip the rendering of the object. Useful for e.g. enemies which turn around when they hit a wall.

  // Counters for various things. Depends on the object.
  int i;
  int j;

  char c;

  int animFrame; // Each object needs it's own animation counter so that the animation starts at the beginning when the object appears.

  char type; // Each object type is associated with a character.
  point** ps; // ps[i] is the ith frame of animation.
  int nps; // Number of points
  box bb; // bounding box
  box* cols; // The areas of collision detection.
  int nCols; // Number of collision detection boxes.
} obj;

obj ob_objFchar(char);

//TODO: Why didn't I do this?
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
