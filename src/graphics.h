#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <SDL.h>
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
#define k_camT -pi/10 //initial camera angle
// dimensions of the font
// TODO: read this from the font file
#define k_fontWidth 7
#define k_fontHeight 7
// scale the font up
#define k_fontSize 2
#define k_fontPadChar (k_fontSize)
#define k_fontPadLine (k_fontSize * 2)
// TODO: define the space a character takes up
// TODO: Also, this feels kind of ugly
#define k_fontSpaceX(vert) ((k_fontWidth) * (k_fontSize) + (vert ? k_fontPadLine : k_fontPadChar))
#define k_fontSpaceY(vert) ((k_fontHeight) * (k_fontSize) + (vert ? k_fontPadChar : k_fontPadLine))
#define k_camSpeed (conf.sensitivity * (conf.invertMouseY ? -1 : 1) / 10000.0)

int lineSize;
bool debug;
bool *font;
int fontSize;

extern struct state s;
struct camera cam;
// TODO: perhaps this should be an array of `color`s instead?
unsigned char *perspScreen;
int imageWidth;
point *dimScreen;

void gr_color(color);

void gr_text(color, bool, char *, GLfloat, GLfloat);
// Basic rectangle drawing
void gr_rectLTRB(color, float left, float top, float right, float bottom);
void gr_rectLTWH(color, float left, float top, float width, float height);
void gr_rectCCWH(color, float centerX, float centerY, float width, float height);
void gr_rectLCWH(color, float left, float centerY, float width, float height);

void gr_image(image, GLfloat, GLfloat, GLfloat, GLfloat);

void gr_init();
void gr_deinit();
void gr_update();
void gr_drawPersp();
void gr_drawDim();
void gr_drawHud();
void gr_clear();

void gr_keypress(int, int, int);
void gr_mousemove(double, double);

void gr_pixels(unsigned char*);

#endif
