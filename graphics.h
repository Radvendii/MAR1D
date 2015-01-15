#include "objects.h"
#ifndef _GRAPHICS_H
#define _GRAPHICS_H

struct state {
    struct world world;
    bool forward;
    bool backward;
    int upward; //stores for how many frames player should travel upwards
    bool onGround;
    bool paused;
    double velX;
    double velY;
};

void gr_init();
void gr_deinit();
void gr_update();

void gr_pixels(unsigned char*);
void gr_lines(line *ls);

#endif
