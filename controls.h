#ifndef _CONTROLS_H
#define _CONTROLS_H
#include <GLFW/glfw3.h> //for keypress keycodes (e.g. GLFW_KEY_W)
#include "helpers.h"
#include "objects.h"
#include "gamelogic.h"
#include "mechanics.h"

#define k_xVel 2.0

struct state s;

void cl_keypress(int, int, int, int);
void cl_cursormove(double, double);
bool cl_go(struct world*, char, int);
bool cl_forward(struct world*);
bool cl_backward(struct world*);
bool cl_upward(struct world*);
void cl_gravity();

#endif
