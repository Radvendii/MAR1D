#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#include <GLFW/glfw3.h> //for openGL functions
#include <unistd.h>
#include "helpers.h"
#include "objects.h"
#include "gamelogic.h"
#include "rendering.h"
#include "parsing.h"
#include "windowing.h"
#define k_tBeforeFOVChange 40
#define k_durationFOVChange 20
#define k_FOV (70*pi/180)
#define k_FOVrun (65*pi/180)
#define k_fontSize 2
#define k_camT -pi/10 //initial camera angle

float camSpeed;
int lineSize;
bool debug;
bool *font;
int fontSize;
GLuint texture;

extern struct state s;
struct camera cam;
unsigned char *perspScreen;
point *dimScreen;

void gr_text(bool, char *, GLfloat, GLfloat);

void gr_init(int, int);
void gr_deinit();
void gr_update();
void gr_drawPersp();
void gr_drawDim();
void gr_drawHud();
void gr_drawMenu();
void gr_clear();

void gr_keypress(int, int, int, int);
void gr_cursormove(double, double);

void gr_pixels(unsigned char*);

#endif
