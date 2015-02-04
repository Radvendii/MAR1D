#ifndef _MECHANICS_H
#define _MECHANICS_H
#include "helpers.h"
#include "objects.h"
#include "controls.h"
#include "gamelogic.h"

struct state s;

bool mh_isCollision(struct world, int, int);
bool mh_playerCollision(int);
void mh_update();
void mh_listCollisions();
/*
 *bool mh_isCollisionRight(struct world, int);
 *bool mh_isCollisionLeft(struct world, int);
 */
#endif
