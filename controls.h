#include "objects.h"
#ifndef _CONTROLS_H
#define _CONTROLS_H
bool cl_go(struct world*, char, int);
bool cl_forward(struct world*);
bool cl_backward(struct world*);
bool cl_upward(struct world*);
bool cl_gravity(struct world*);

#endif
