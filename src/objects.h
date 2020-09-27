#ifndef _OBJECTS_H
#define _OBJECTS_H
#include "helpers.h"

//TODO: remove need for k_nMax___

#define k_nMaxObj 1000 //maximum number of objects in a world
// Special points. Won't conflict with actual points because '\0' is not a valid colour.
#define p_termPoint ((point){ .x=124214, .y=143512, .c = '\0'}) // Point which terminates the array of points.
#define p_skipPoint ((point){ .x=124214, .y=141312, .c = '\0'}) // Special point that is simply skipped. This is useful so that all frames of an animation can be made to have the same number of points.

// Sometimes it's more convenient to define a color by RGB(0xRRGGBB)
#define RGB0x(hex) (color) {                    \
    .r = ((0x##hex) & 0xFF0000) >> 16,          \
    .g = ((0x##hex) & 0x00FF00) >> 8,           \
    .b = ((0x##hex) & 0x0000FF) >> 0            \
  }

#define k_colorBrown RGB0x(C84C0C)
#define k_colorPink  RGB0x(FCBCB0)
#define k_colorWhite RGB0x(F0F0F0)
#define k_colorDark  RGB0x(404040)
#define k_colorLight RGB0x(CCCCCC)
#define k_colorGray  RGB0x(888888)
#define k_colorBlue  RGB0x(5c94fc)
#define k_colorGreen RGB0x(00a800)
#define k_colorF     RGB0x(FFFFFF)
#define k_color0     RGB0x(000000)

#define k_colorTextLit k_colorWhite
#define k_colorTextDim k_colorDark
#define k_colorTextMed k_colorGray

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
