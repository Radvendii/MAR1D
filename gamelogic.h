#ifndef _GAMELOGIC_H
#define _GAMELOGIC_H
#include "objects.h"
#include "controls.h"
#include "mechanics.h"
#define act_nothing 0
#define act_bounce 16
#define act_bounceU 1
#define act_bounceD (act_bounce/2 + act_bounceU*2)


struct state {
    int* scene;
    int* action;
    int x;
    int y;
    bool onGround;
    bool paused;
    bool forward;
    double gravity;
    int upcount;
    bool backward;
    double velX;
    double velY;
    bool flip;
    int coins;
    int moveFrameY;
    int moveFrameX;
};

struct state s;

void gl_init();
void gl_update();
void gl_deinit();

#endif
