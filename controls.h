#ifndef _CONTROLS_H
#define _CONTROLS_H
#include <GLFW/glfw3.h> //for keypress keycodes (e.g. GLFW_KEY_W)
#include "helpers.h"
#include "objects.h"
#include "gamelogic.h"
#include "mechanics.h"

#define k_xVel 0.15
#define k_yVel 5.5
#define k_xVelMax 3.0
#define k_yVelMin -7.0
#define k_gravity -0.45
#define k_nJumpFrames 30

struct state s;

void cl_update();
void cl_keypress(int, int, int, int);
void cl_cursormove(double, double);
bool cl_go(struct world*, char, int);
bool cl_forward(struct world*);
bool cl_backward(struct world*);
bool cl_upward(struct world*);
void cl_gravity();
void cl_jumpStart();
void cl_jumpEnd();

#endif
