#ifndef _MECHANICS_H
#define _MECHANICS_H
#include "helpers.h"
#include "objects.h"
#include "controls.h"
#include "gamelogic.h"
#define k_gravity -0.42
#define k_moveFrames 2
#define k_leftSide(x1, x2, x3, x4) (x1 > x3 && x1 > x4 && x2 > x3 && x2 > x4)
#define k_oneSide(x1, x2, x3, x4) (k_leftSide(x1, x2, x3, x4) || k_leftSide(x3, x4, x1, x2))
#define k_boxInter(b1, b2) !(k_oneSide(b1.x, b1.x+b1.w, b2.x, b2.x+b2.w) || k_oneSide(b1.y, b1.y+b1.h, b2.y, b2.y+b2.h))

extern struct state s;

void mh_init();
int mh_isCollision(int, int);
bool mh_playerCollision(int);
void mh_update();
void mh_listCollisions();
bool mh_collision(int, int);
void mh_doCollision(obj*, obj*, int);
#endif
