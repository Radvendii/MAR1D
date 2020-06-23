#ifndef _WINDOWING_H
#define _WINDOWING_H
#include <SDL.h>
#include "helpers.h"
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

SDL_Window* dimWindow;
SDL_Window* perspWindow;

// Activate different windows. No guarantee these are actually different windows.
void wn_menuWindow();
void wn_hudWindow();
void wn_perspWindow();
void wn_dimWindow();

// Event processing
void wn_click(int, int, int);
void wn_keypress(SDL_Keycode, int, int);
void wn_mousemove(double, double);
void wn_processEvents();

void wn_update();

bool wn_shouldClose();

void wn_disable_mouse(bool);

void wn_init();
void wn_deinit();

#endif
