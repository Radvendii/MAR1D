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

extern struct state s;

void mh_init();
bool mh_isCollision(int, int);
bool mh_playerCollision(int);
void mh_update();
void mh_listCollisions();
/*
 *bool mh_isCollisionRight(struct world, int);
 *bool mh_isCollisionLeft(struct world, int);
 */
#endif
