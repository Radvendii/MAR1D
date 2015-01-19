#include "objects.h"
#ifndef _CONTROLS_H
#define _CONTROLS_H

#define k_xVel 2.0

struct state {
    struct world world;
    bool onGround;
    bool paused;
    double velX;
    double velY;
    bool camFlip;
};

void cl_keypress(struct state*, int, int, int, int);
void cl_cursormove(struct state*, double, double);
bool cl_go(struct world*, char, int);
bool cl_forward(struct world*);
bool cl_backward(struct world*);
bool cl_upward(struct world*);
void cl_gravity(struct state*);

#endif
