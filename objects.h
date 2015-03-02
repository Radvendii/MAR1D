#ifndef _OBJECTS_H
#define _OBJECTS_H
#include "helpers.h"

//TODO: remove need for k_nMax___

#define k_nMaxLinesPerObj 100 //maximum number of lines per object
#define k_nMaxObj 500 //maximum number of objects in a world
#define p_termPoint ((point){ .x=124214, .y=143512, .r=100, .g=100, .b=100 }) //Hope this point never actually comes up... :P
#define p_skipPoint ((point){ .x=124214, .y=141312, .r=100, .g=100, .b=100 }) //Hope this point never actually comes up... :P
#define c_brick .r=231, .g=95, .b=19
#define c_lbrick .r=240, .g=208, .b=176
#define c_black .r=0, .g=0, .b=0

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} color;

typedef struct {
    int x;
    int y;
    char r;
    char g;
    char b;
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
    bool hidden;
    char c;
    bool active;
    bool flip;

    char type;
    point** ps;
    int nps;
    box bb;
    box* cols;
    int nCols;
} obj;

obj ob_objFchar(char);

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

