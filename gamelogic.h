#ifndef _GAMELOGIC_H
#define _GAMELOGIC_H
#include "objects.h"
#include "controls.h"
#include "mechanics.h"
#include "enemies.h"
#include "parsing.h"
#define act_nothing 0
#define act_bounce 16
#define act_bounceU 1
#define act_bounceD (act_bounce/2 + act_bounceU*2)

#define act_startGrow 1
#define k_growthRate 7

#define k_dieStart 170
#define k_dieStartMoving 150
#define k_dieStartBlack 100

#define k_corpseLife 80
#define k_shellLife 160

#define k_timeTick 30
#define k_time 400

#define k_menuTime 200
#define k_menuStatic 201
#define k_menuWin k_menuStatic+250

struct state {
    char level;
    char loc;
    char pipeTo;
    char check;
    level scene;
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
    bool fire;
    bool crouch;
    int dead;
    int nFBalls;
    int invincible;
    int run;
    int star;
    int lives;
    int coins;
    int score;
    int time;
    int multibounce;
    int moveFrameY;
    int moveFrameX;

    int menu;
};

struct state s;

int gl_playerIndex();

void gl_resetLevel();
void gl_loadLevel(char, char);
void gl_win();

void gl_init();
void gl_update();
void gl_deinit();

void gl_killed();
void gl_die();

#endif
