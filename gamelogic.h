#ifndef _GAMELOGIC_H
#define _GAMELOGIC_H
#include "objects.h"
#include "controls.h"
#include "mechanics.h"
struct state {
    struct world world;
    bool onGround;
    bool paused;
    double velX;
    double velY;
    bool camFlip;
    int coins;
};

extern struct state s;

#endif
