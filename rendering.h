#ifndef _RENDERING_H
#define _RENDERING_H
#include "objects.h"
#include "helpers.h"
#include <math.h>

//void rn_orthoTest(unsigned char*);
//void rn_perspTest(int camX, int camY, double camT, unsigned char*);
//int rn_dimTest(line**);
int rn_dimFworld(line**, struct world);
void rn_perspFworld(unsigned char*, struct world);
void rn_perspFworld_v(unsigned char*, struct world, line**);
#endif
