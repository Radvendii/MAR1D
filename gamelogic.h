#ifndef _GAMELOGIC_H
#define _GAMELOGIC_H
#include "objects.h"
#include "controls.h"
#include "mechanics.h"
struct state {
    struct world world;
    bool onGround;
    bool paused;
    bool forward;
    double gravity;
    int upcount;
    bool backward;
    double velX;
    double velY;
    bool camFlip;
    int coins;
};

extern struct state s;

void gl_init();
void gl_update();
void gl_deinit();

#endif
