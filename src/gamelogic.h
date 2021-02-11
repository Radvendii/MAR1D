#ifndef _GAMELOGIC_H
#define _GAMELOGIC_H
#include <SDL.h>
#include "objects.h"
#include "controls.h"
#include "mechanics.h"
#include "enemies.h"
#include "parsing.h"

/*
 * Time keeping is a little confusing because there are three different time
 * measurements.
 * 1. Real-World Time
 *    This is reported by SDL_GetTicks() and is measured in milliseconds. It is
 *    used to make sure the game runs at a constant rate regardless of how fast
 *    the processor is. It is found only in gl_main().
 * 2. Time Ticks
 *    This is the number displayed in the HUD that tells the player how much
 *    time they have left. Each one is 0.4 seconds. (in other words, 2.5 time
 *    ticks per second)
 * 3. Game Ticks
 *    This keeps track of how many times the game logic has updated. There are
 *    24 game ticks in a time tick.
 *
 * Unless otherwise stated, constants / variables are measured in game ticks.
 *
 * I found the values from the original game here:
 * https://www.mariowiki.com/Time_Limit#Super_Mario_Bros._/_Super_Mario_Bros.:_The_Lost_Levels
 */

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

#define k_time 400 // How much time the player has to complete the stage (in timeTicks)
#define k_timeLow 101 // The time after which time is 'low' and the music will change (in timeTicks)

#define k_gameTicksPerTimeTick 24
#define k_timeTicksPerSec 2.5
#define k_msPerGameTick (1000 / (k_gameTicksPerTimeTick * k_timeTicksPerSec))
// ms  / gt   tt    ms
// -- /  -- * --  = --
// s /   tt   s     gt

#define k_winScreenTime 6000

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
  int time; // Time left in the game (in gameTicks)
  int multibounce; // Bouncing accross multiple enemies results in a score multiplier. This keeps track of that.
  bool lowTime; // Is the player low on time

  /* int menu; // This variable keeps track of which part of the game we are in (menu, main game, win screen) */
};

extern struct state s;

int gl_playerIndex();

void gl_main(); // main game loop

void gl_winScreen(); // screen that displays when the player wins the game

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
