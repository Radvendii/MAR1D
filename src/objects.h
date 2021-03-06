#ifndef _OBJECTS_H
#define _OBJECTS_H
#include "helpers.h"

//TODO: remove need for k_nMax___

#define k_nMaxObj 1000 //maximum number of objects in a world
// Special points. Won't conflict with actual points because '\0' is not a valid colour.
#define p_termPoint ((point){ .x=124214, .y=143512, .c = '\0'}) // Point which terminates the array of points.
#define p_skipPoint ((point){ .x=124214, .y=141312, .c = '\0'}) // Special point that is simply skipped. This is useful so that all frames of an animation can be made to have the same number of points.

// Sometimes it's more convenient to define a color by RGB0x(RRGGBB)
// can't use RGB() because that's taken by some library
#define RGB0x(hex) (color) {                    \
    .r = ((0x##hex) & 0xFF0000) >> 16,          \
    .g = ((0x##hex) & 0x00FF00) >> 8,           \
    .b = ((0x##hex) & 0x0000FF) >> 0            \
  }

#define RGB_brown RGB0x(C84C0C)
#define RGB_pink  RGB0x(FCBCB0)
#define RGB_white RGB0x(F0F0F0)
#define RGB_dark  RGB0x(404040)
#define RGB_light RGB0x(CCCCCC)
#define RGB_gray  RGB0x(888888)
#define RGB_blue  RGB0x(5c94fc)
#define RGB_green RGB0x(00a800)
#define RGB_F     RGB0x(FFFFFF)
#define RGB_0     RGB0x(000000)

#define RGB_textLit RGB_white
#define RGB_textDim RGB_dark
#define RGB_textMed RGB_gray

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

typedef struct {
  point* ps; // points for 1D rendering
  image im; // sprite for 2D rendering
} objFrame;

typedef struct {
  // Position
  int x;
  int y;
  // part left over from trying to move a non-integral amount
  double xMantissa;
  double yMantissa;

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

  int combo; // used by the player and koopa shell to keep track of the score combo count

  char type; // Each object type is associated with a character.
  objFrame* frames; // Animation frames
  int nFrames; // Number of frames
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
extern level* ob_levels;

void ob_init();
void ob_deinit();

void ob_printPoint(point);
void ob_realifyPoint(point*, int, int);
void ob_realifyBox(box*, int, int);
void ob_objectifyCam(int*, int*);

bool ob_p_isTerm(point);
bool ob_p_isSkip(point);

#endif
