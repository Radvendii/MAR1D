#ifndef _RENDERING_H
#define _RENDERING_H
#include "objects.h"
#include "helpers.h"
#define k_nPixels 500 //resolution of screen. Never make it odd.
#define k_drawD1 (16*7)
#define k_drawD2 (16*8)
#define k_drawD (16*8)
#define k_FOV (70*pi/180)
#define k_FOVrun (65*pi/180)
#define k_bgr 107
#define k_bgg 136
#define k_bgb 255

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
