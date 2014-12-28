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

void g_init() {
    g_ground = malloc(sizeof(line) * k_nMaxLinesPerObj);
    g_ground[0] = (line) { .x1=0, .y1=16, .x2=16, .y2=16, .r=231, .g=95, .b=19 }
    g_ground[1] = (line) { .x1=16, .y1=16, .x2=16, .y2=16, .r=231, .g=95, .b=19 }
    g_ground[2] = (line) { .x1=16, .y1=0, .x2=0, .y2=0, .r=231, .g=95, .b=19 }
    g_ground[3] = (line) { .x1=0, .y1=0, .x2=0, .y2=16, .r=231, .g=95, .b=19 }

    g_nothing = malloc(sizeof(line) * k_nMaxLinesPerObj);
}

int *objFtype(enum objType type) {
    switch(type) {
        case objGround:
            return g_ground;
            break;
        case nothing:
            return g_nothing;
            break;
        default:
            printf("No object data found for object type %d", type);
            exit(-1);
            break;
    }
}

const int g_levelTest[k_nMaxObj*3] = {
    objGround, 20, -8
};

#endif
