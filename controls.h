#include "objects.h"
#ifndef _CONTROLS_H
#define _CONTROLS_H

struct state {
    struct world world;
    bool forward;
    bool backward;
    int upward; //stores for how many frames player should travel upwards
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
