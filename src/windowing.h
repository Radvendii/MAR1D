#ifndef _WINDOWING_H
#define _WINDOWING_H
#include <GLFW/glfw3.h>
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

GLFWwindow* dimWindow;
GLFWwindow* perspWindow;

void wn_menuWindow();
void wn_hudWindow();
void wn_perspWindow();
void wn_dimWindow();


void wn_update();

bool wn_shouldClose();

void wn_disable_mouse(bool);

void wn_debugInit();
void wn_debugDeinit();
void wn_init();
void wn_deinit();

#endif
