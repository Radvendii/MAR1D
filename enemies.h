#ifndef _ENEMIES_H
#define _ENEMIES_H
#include "helpers.h"
#include "objects.h"
#include "controls.h"
#include "mechanics.h"
#include "gamelogic.h"

extern struct state s;

void ai_init();
void ai_update();
void ai_killAt(int);
void ai_kill(obj*);
void ai_activate(int, int);
#endif
