#ifndef _GAMELOGIC_H
#define _GAMELOGIC_H
#include "objects.h"
#include "controls.h"
#include "mechanics.h"
struct state { //TODO: split state into game state and graphics state
    int* scene;
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
