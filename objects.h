#ifndef _OBJECT_H
#define _OBJECT_H
#include <math.h>
#include "helpers.h"

//TODO: Put line functions in different file

#define k_nPixels 1000 //resolution of screen. Never make it odd.
#define k_FOV 60 //Field of vision (degrees)
#define k_nMaxLinesPerObj 100 //maximum number of lines per object
#define k_drawD 300
#define k_nMaxObj 200 //maximum number of objects in a world
#define k_termLine  ((line){ .x1=0, .y1=0, .x2=0, .y2=0, .r=0, .g=0, .b=0 })
#define p_termPoint ((point){ .x=124214, .y=143512, .r=100, .g=100, .b=100 })
#define c_brick .r=231, .g=95, .b=19
#define c_lbrick .r=240, .g=208, .b=176
#define c_black .r=0, .g=0, .b=0

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

typedef struct {
    int x;
    int y;
    char r;
    char g;
    char b;
} point;

struct world {
    int camX;
    int camY;
    double camT;
    int *scene;
};

//objects possible in the scene
line *ob_ground_old;
line *ob_nothing_old;
line *ob_playerBox_old;
line *ob_brick_old;
line *ob_coin_old;
int *ob_levelTest_old;

point* ob_ground;
point* ob_nothing;
point* ob_playerBox;
point* ob_brick;
point* ob_coin;
int *ob_levelTest;


//object from type
line* objFtype_old(enum objType);
point* objFtype(enum objType);

void ob_init_old();
void ob_deinit_old();
void ob_init();
void ob_deinit();
bool ob_intersect(line, line, float*, float*);
bool ob_intersectIn(line, line);
float ob_intersectD(line, line);
float ob_len(line);

void ob_printLine(line);
void ob_realifyLine(line*, int*);
void ob_realifyPoint(point*, int*);
void ob_objectifyCam(int*, int*);
float ob_slope(line);

bool ob_isTerminating(line);
bool ob_p_isTerm(point);

#endif
