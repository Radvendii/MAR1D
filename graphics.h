#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#include <GLFW/glfw3.h>
#include <unistd.h>
#include "helpers.h"
#include "objects.h"
#include "gamelogic.h"
#include "rendering.h"

bool debug;
bool *font;

struct state s;
unsigned char *perspScreen;
line *dimScreen_old;
point *dimScreen;

void gr_text(char *, GLfloat, GLfloat);

void gr_init();
void gr_deinit();
void gr_update();
void gr_draw();

void gr_pixels(unsigned char*);
void gr_lines(line *ls);

#endif
