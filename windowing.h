#ifndef _WINDOWING_H
#define _WINDOWING_H
#include <GLFW/glfw3.h>
#include "helpers.h"
#include "objects.h"
#include "gameLogic.h"
#include "graphics.h"

GLFWwindow* dimWindow;
GLFWwindow* perspWindow;

void wn_perspWindow();
void wn_dimWindow();

void wn_update();

bool wn_shouldClose();
void wn_init();
void wn_deinit();

#endif
