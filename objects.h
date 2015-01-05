#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#ifndef _OBJECT_H
#define _OBJECT_H

#define k_nPixels 500 //resolution of screen
#define k_FOV 60 //Field of vision (degrees)
#define k_nMaxLinesPerObj 30 //maximum number of lines per object
#define k_drawD 500
#define k_nMaxObj 50 //maximum number of objects in a world

//labeling all the types of objects (enum)
enum objType {
    nothing=0,
    objGround
};

typedef struct {
    int x1;
    int x2;
    int y1;
    int y2;
    char r;
    char g;
    char b;
} line;

//objects possible in the scene
line *g_ground;
line *g_nothing;

//object from type
line* objFtype(enum objType);

void g_init();

void g_printLine(line*);

bool g_isNotTerminating(line*);

int g_levelTest[k_nMaxObj*3];

#endif
