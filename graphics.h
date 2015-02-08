#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#include <GLFW/glfw3.h>
#include <unistd.h>
#include "helpers.h"
#include "objects.h"
#include "gamelogic.h"
#include "rendering.h"
#include "parsing.h"

bool debug;
bool *font;
int fontSize;

extern struct state s;
struct camera cam;
unsigned char *perspScreen;
point *dimScreen;

void gr_text(char *, GLfloat, GLfloat);

void gr_init();
void gr_deinit();
void gr_update();
void gr_draw();

void gr_keypress(int, int, int, int);
void gr_cursormove(double, double);

void gr_pixels(unsigned char*);

#endif
