#include <stdio.h>
#include <stdlib.h>
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

typdef struct {
    int x1;
    int x2;
    int y1;
    int y2;
    char r;
    char g;
    char b;
} line;

//objects possible in the scene
static line *g_ground;
static line *g_nothing;

//object from type
int objFtype(enum objType);

static int g_levelTest[k_nMaxObj*3];

#endif
