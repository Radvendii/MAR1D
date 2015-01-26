#ifndef _OBJECT_H
#define _OBJECT_H
#include <math.h>
#include "helpers.h"

#define k_nPixels 1000 //resolution of screen. Never make it odd.
#define k_FOV 60 //Field of vision (degrees)
#define k_nMaxLinesPerObj 100 //maximum number of lines per object
#define k_drawD 300
#define k_nMaxObj 200 //maximum number of objects in a world
#define k_termLine  ((line){ .x1=0, .y1=0, .x2=0, .y2=0, .r=0, .g=0, .b=0 })

//labeling all the types of objects (enum)
enum objType {
    terminator=0,
    objPlayer,
    nothing,
    objGround,
    objBrick,
    objCoin
};

typedef struct {
    int x1;
    int x2;
    int y1;
    int y2;
    unsigned char r;
    unsigned char g;
    unsigned char b;
} line;

struct world {
    int camX;
    int camY;
    double camT;
    int *scene;
};

//objects possible in the scene
line *ob_ground;
line *ob_nothing;
line *ob_playerBox;
line *ob_brick;
line *ob_coin;
int *ob_levelTest;

//object from type
line* objFtype(enum objType);

void ob_init();
void ob_deinit();
bool ob_intersect(line, line, float*, float*);
bool ob_intersectIn(line, line);
float ob_intersectD(line, line);

void ob_printLine(line);
void ob_realifyLine(line*, int*);
void ob_objectifyCam(int*, int*);
float ob_slope(line);

bool ob_isTerminating(line);

#endif
