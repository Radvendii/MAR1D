#ifndef _RENDERING_H
#define _RENDERING_H
#include "objects.h"
#include "helpers.h"
#include "parsing.h"
#define k_nPixels 512 // resolution of screen. Never make it odd.
#define k_drawD1 (16*7) // Start fadeout (along x-axis)
#define k_drawD2 (16*8) // End fadeout (along x-axis)
#define k_drawD (16*12) // Absolute draw distance (along radial axis)

// Background color
#define k_bgr 107
#define k_bgg 136
#define k_bgb 255
#define k_flashG 1000

#define k_animFreq 10 // How fast to step the animations

struct camera {
  double FOV;
  int x;
  int y;
  double T; // Angle from x-axis
  level scene;
  bool flip; // Camera facing backwards
  bool paused;
  bool flashD;
  bool flashB;
  int flashG;
  bool redTint;
  color bg;
  int drawD;
  int animFrame;
};

void rn_dimFcamera(point*, struct camera);

void rn_dimFcamera(point*, struct camera);
void rn_perspFcamera(unsigned char*, struct camera);
#endif
