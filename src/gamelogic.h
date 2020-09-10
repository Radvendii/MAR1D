#ifndef _GAMELOGIC_H
#define _GAMELOGIC_H
#include "objects.h"
#include "controls.h"
#include "mechanics.h"
#include "enemies.h"
#include "parsing.h"

// Constants to define the bouncing animation for blocks
#define act_nothing 0
#define act_bounce 16
#define act_bounceU 1
#define act_bounceD (act_bounce/2 + act_bounceU*2)

// Constants to define the animation for mario's growth
#define act_startGrow 1
#define k_growthRate 7 // How many frames per change

// Constants to define mario's death animation
#define k_dieStart 170
#define k_dieStartMoving 150
#define k_dieStartBlack 100

#define k_corpseLife 80 // How long until enemy corpses are removed from the game.
#define k_shellLife 160 // How long koopas remain shells until they transform back

#define k_timeTick 30 // How many frames in a time tick
#define k_time 400 // How much time the player has to complete the stage
#define k_timeLow 101 // The time after which time is 'low' and the music will change.

// Constants to define the animation from menu to the actual game, and the winning screen.
#define k_menuTime 200
#define k_menuStatic 201
#define k_menuWin k_menuStatic+500

struct state {
  char level; // The level the player is on
  char loc; //
  char pipeTo; // For the time in transition inside a pipe
  char check; //
  level scene; // The level data. This will change as the player plays
  int pli; // The index of the player in the list of objects
  bool onGround; // Whether the player is on the ground
  bool paused; // Is the game paused (by the user or because of some action in the game)
  bool userPaused; // Is the game paused by the user
  double gravity; // How strong is gravity
  int upcount; // 
  bool forward; // Is the forward button pressed
  bool backward; // Is the backward button pressed
  int leftMost; // The leftmost part of the world still in existence. This isn't used for anything at the moment, but could be used to de-generate the world as mario runs through it.
  bool flip; // Is mario facing backwards?
  bool bigMario; // Is mario big (after having eaten a mushroom)
  bool fire; // Has mario consumed a fire flower
  bool crouch; // Is mario crouching
  bool won; // Has the player won
  int dead; // Is mario dead
  int nFBalls; // The number of fire balls that are in the world
  int invincible; // Is mario invincible (after just having been demoted from big mario, for example)
  bool run; // Is the run button pressed
  int runWarp; // The screen warps while running to increase the visual effect. This measures haw far along that process we are
  int star; // Does mario have star power
  int lives;
  int coins;
  int score;
  int time; // Time left in the game
  int multibounce; // Bouncing accross multiple enemies results in a score multiplier. This keeps track of that.
  // The moveFrame variables are bookkeeping variables for when mario is moving slower than one square per frame, so that he can move one square every two frames for example.
  int moveFrameY;
  int moveFrameX;
  bool lowTime; // Is the player low on time

  /* int menu; // This variable keeps track of which part of the game we are in (menu, main game, win screen) */
};

struct state s;

int gl_playerIndex();

void gl_main(); // main game loop

void gl_keypress(SDL_KeyboardEvent ev);
void gl_mouseclick(SDL_MouseButtonEvent ev);
void gl_mousemove(SDL_MouseMotionEvent ev);

void gl_resetLevel();
void gl_loadLevel(char, char);
void gl_load();
void gl_win();

void gl_init();
void gl_update();
void gl_deinit();

void gl_killed();
void gl_die();

#endif
