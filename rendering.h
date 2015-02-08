#ifndef _RENDERING_H
#define _RENDERING_H
#include "objects.h"
#include "helpers.h"
#define k_nPixels 1000 //resolution of screen. Never make it odd.
#define k_drawD 150

void rn_dimFworld(point*, struct world);
void rn_perspFworld_v(unsigned char*, struct world, point*);
#endif
