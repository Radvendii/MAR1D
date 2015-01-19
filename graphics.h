#include "objects.h"
#ifndef _GRAPHICS_H
#define _GRAPHICS_H

GLFWwindow* dimWindow;
GLFWwindow* perspWindow;
struct state s;
unsigned char *screen;
line *lineArr;

void gr_init();
void gr_deinit();
void gr_update();

void gr_pixels(unsigned char*);
void gr_lines(line *ls);

#endif
