#include "objects.h"
#ifndef _RENDERING_H
#define _RENDERING_H

void rn_orthoTest(unsigned char*);
void rn_perspTest(int camX, int camY, int camT, unsigned char*);
int rn_dimTest(line**);
void rn_screenFworld(unsigned char*, struct world);
#endif
