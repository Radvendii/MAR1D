#ifndef _GAMELOGIC_H
#define _GAMELOGIC_H
#include "objects.h"
#include "controls.h"
#include "mechanics.h"
#include "enemies.h"
#define act_nothing 0
#define act_bounce 16
#define act_bounceU 1
#define act_bounceD (act_bounce/2 + act_bounceU*2)
#define act_startGrow 1
#define k_growthRate 7


struct state {
    obj* scene;
    int pli;
    bool onGround;
    bool paused;
    double gravity;
    int upcount;
    bool forward;
    bool backward;
    int leftMost;
    bool flip;
    bool bigMario;
    int invincible;
    int star;
    int lives;
    int coins;
    int moveFrameY;
    int moveFrameX;
};

typedef struct {
    obj o;
    int x;
    int y;
    double vx;
    double vy;
    bool gravity;
    bool physical;
    int i;
} gameObj;

struct state s;

int gl_playerIndex();

void gl_init();
void gl_update();
void gl_deinit();

void gl_killed();
void gl_die();

#endif
