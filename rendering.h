#ifndef _RENDERING_H
#define _RENDERING_H
#include "objects.h"
#include "helpers.h"
#define k_nPixels 1000 //resolution of screen. Never make it odd.
#define k_drawD (16*16)

struct camera {
    double FOV;
    int x;
    int y;
    double T;
    obj* scene;
    bool flip;
    bool paused;
    bool debug;
    bool flashD;
    bool flashB;
    int drawD;
    int animFrame;
    int yshift;
    int xshift;
};

void rn_dimFcamera(point*, struct camera);

void rn_dimFcamera(point*, struct camera);
void rn_perspFcamera(unsigned char*, struct camera, point*);
#endif
