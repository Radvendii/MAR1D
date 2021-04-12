#ifndef _CONTROLS_H
#define _CONTROLS_H
#include "SDL.h"
#include "helpers.h"
#include "objects.h"
#include "gamelogic.h"
#include "mechanics.h"
#include "audio.h"

// How much to increase the velocity by each timestep. 
// For the y velocity it depends on several factors.
#define k_xVel 0.02
#define k_yVel (3.9+fabs(s.scene[s.pli].vx)*s.bigMario/5)

#define k_xVelMax 1.5
#define k_xVelRunMax (5.0 / 3.0 * k_xVelMax)
#define k_xVelMin 0.8
#define k_yVelMin -4.5

#define k_nJumpFrames 40 // Duration of reduced gravity.

extern struct state s;

void cl_init();
void cl_event();
void cl_update();
void cl_keypress(int, int, int);
void cl_click(int,int);
void cl_cursormove(double, double);
bool cl_move1(int, char, int);
bool cl_move(int, char, int);
void cl_gravity(int);
void cl_jumpStart();
void cl_smallJump();
void cl_jumpEnd();
void cl_bigMario();
void cl_fireMario();
void cl_starman();
void cl_smallMario();
void cl_fire();
void cl_oneUp();
void cl_score(int);
void cl_crouch();
void cl_uncrouch();
void cl_pipe();
void cl_delObjAt(int);
void cl_delObj(obj*);

#endif
