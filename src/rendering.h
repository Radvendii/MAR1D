#ifndef _RENDERING_H
#define _RENDERING_H
#include "objects.h"
#include "helpers.h"
#include "parsing.h"
#define k_nPixels 512 //resolution of screen. Never make it odd.
#define k_drawD1 (16*7)
#define k_drawD2 (16*8)
#define k_drawD (16*12)
#define k_bgr 107
#define k_bgg 136
#define k_bgb 255
#define k_flashG 1000

#define k_animFreq 10

struct camera {
  double FOV;
  int x;
  int y;
  double T;
  level scene;
  bool flip;
  bool paused;
  bool debug;
  bool flashD;
  bool flashB;
  color bg;
  int flashG;
  bool redTint;
  int drawD;
  int animFrame;
  int yshift;
  int xshift;
};

void rn_dimFcamera(point*, struct camera);

void rn_dimFcamera(point*, struct camera);
void rn_perspFcamera(unsigned char*, struct camera, point*);
#endif
