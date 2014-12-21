#include <stdio.h>
#include <stdlib.h>
#ifndef _OBJECT_H
#define _OBJECT_H

#define k_nPixels 100 //resolution of screen
#define k_FOV 60 //Field of vision (degrees)
#define k_nMaxLinesPerObj 30 //maximum number of lines per object
#define k_nMaxObj 50 //maximum number of objects in a world

//labeling all the types of objects (enum)
enum objType {
    nothing=0,
    objGround
};


//objects possible in the scene
int g_ground[k_nMaxLinesPerObj*7] = { //two coordinates define a line (first four) and color (last three)
    0,  16, 16, 16, 231, 95, 19,
    16, 16, 16, 0,  231, 95, 19,
    16, 0, 0, 0,    231, 95, 19,
    0,  0, 0, 16,   231, 95, 19
};

int g_nothing[k_nMaxLinesPerObj*7] = {};

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

const int g_levelTest[k_nMaxObj*3] = { //which object and coordinates
    objGround, 20, -8
};

#endif
