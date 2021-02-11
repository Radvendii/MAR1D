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
#define k_fontW 7
#define k_fontH 7
// scale the font up
// TODO: make this an argument you can pass into gr_text()
#define k_fontSize 2
#define k_fontPadChar 1 // padding betweeen characters
#define k_fontPadLine 3 // padding between lines
/* // the amount of actual space a character takes up */
#define k_fontCharX (k_fontW * k_fontSize)
#define k_fontCharY (k_fontH * k_fontSize)
// TODO: this feels kind of ugly
#define k_fontSpaceX(vert) (k_fontW + (vert ? k_fontPadLine : k_fontPadChar))
#define k_fontSpaceY(vert) (k_fontH + (vert ? k_fontPadChar : k_fontPadLine))
#define k_camSpeed (conf.sensitivity * (conf.invertMouseY ? -1 : 1) / 10000.0)

#define k_bezelSize 1

#define RECT_LTRB(left, top, right, bottom) (rect) { .l = left, .t = top, .r = right, .b = bottom }
#define RECT_LTWH(left, top, width, height) RECT_LTRB((left), (top), (left) + (width), (top) - (height))
#define RECT_LBWH(left, bottom, width, height) RECT_LTWH((left), (bottom) + (height), (width), (height))
#define RECT_CCWH(centerX, centerY, width, height) RECT_LTWH((centerX) - (width) / 2.0, (centerY) + (height) / 2.0, (width), (height))
#define RECT_LCWH(left, centerY, width, height) RECT_LTWH((left), (centerY) + (height) / 2.0, (width), (height))
#define RECT_RCWH(right, centerY, width, height) RECT_LCWH((right) - (width), (centerY), (width), (height))

typedef struct {
    float l, t, r, b;
} rect;

extern struct state s;

void gr_color(color);

void gr_text(color, bool, char *, float, GLfloat, GLfloat);
// Basic rectangle drawing
void gr_drawRect(color, rect);
void gr_drawBezelOut(rect);
void gr_drawBezelIn(rect);

void gr_image(image *, rect);

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
