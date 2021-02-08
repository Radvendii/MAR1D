#ifndef _WINDOWING_H
#define _WINDOWING_H
#include <SDL.h>
#include "helpers.h"
#include "menu.h"
#include "objects.h"
#include "gamelogic.h"
#include "graphics.h"

#define k_dimWindowH 512
#define k_dimWindowW 512
#define k_perspWindowH 512
#define k_perspWindowW 512
#define k_hudWindowH 512
#define k_hudWindowW 512
#define k_menuWindowH 512
#define k_menuWindowW 512
#define k_dimWindowX 0
#define k_perspWindowX (k_dimWindowX+k_dimWindowW)
#define k_perspWindowName "MAR1D"
#define k_dimWindowName "Debug"

// Activate different windows. No guarantee these are actually different windows.
void wn_menuWindow();
void wn_hudWindow();
void wn_perspWindow();
void wn_dimWindow();

// Event processing
void wn_eventCallbacks(void (*keypress)(SDL_KeyboardEvent),
                       void (*mouseclick)(SDL_MouseButtonEvent),
                       void (*mousemove)(SDL_MouseMotionEvent));
void wn_keypressCallback(void (*callback)(SDL_KeyboardEvent));
void wn_mouseclickCallback(void (*callback)(SDL_MouseButtonEvent));
void wn_mousemoveCallback(void (*callback)(SDL_MouseMotionEvent));
void wn_keypress(SDL_KeyboardEvent);
void wn_mouseclick(SDL_MouseButtonEvent);
void wn_mousemove(SDL_MouseMotionEvent);
void wn_processEvents();

void wn_update();

void wn_disable_mouse(bool);

void wn_init();
void wn_deinit();

#endif
