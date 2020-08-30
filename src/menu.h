#ifndef _MENU_H
#define _MENU_H

#include <SDL_opengl.h>
#include <GL/glu.h>
#include <SDL.h>
#include "helpers.h"
#include "graphics.h"
#include "parsing.h"

/*
 * Definition of datatypes for the menu
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Widget is a general structure for menus, things that appear in menus, and
 * things that are used in menus for spacing, or anything else.
 *
 * The structure is that of a doubly-linked multi-tree. That is, a menu can
 * contain an arbitrary number of widgets inside it, which can be sub-menus, or
 * they can be end-nodes. Each widget also has a pointer up to its parent widget.
 *
 * mu_drawWidget() defines how a given widget gets displayed. It functions
 * recursively on menus.
 *
 * mu_keypressWidget(), mu_mouseclickWidget(), mu_mousemoveWidget() defines
 * what happens when the user interacts with a widget.
 *
 * All of these items must be updated in tandem:
 * - the types available in widgetKind
 * - the widget struct (fields added for it)
 * - the mu_*Widget() functions, (a case added for it)
 */

enum widgetKind { WK_MENU,     // menu
                  WK_SLIDER,   // int value slider
                  WK_SWITCH,   // bool value switch
                  WK_ACTION }; // perform an action when selected

typedef struct widget widget;

struct widget {
  char *label;
  widget *parent;

  enum widgetKind kind;

  union {
    struct { // WK_MENU
      widget *subWidgets;
      int nWidgets;
      int selected;
    };
    struct { // WK_SLIDER
      int *sliderVal;
      int inc;
      int min;
      int max;
    };
    struct { // WK_SWITCH
      bool *switchVal;
    };
    struct { // WK_ACTION
      void (*action)(void);
    };
  };
};

void mu_main();

void mu_init();
void mu_deinit();

void mu_quit();

void mu_drawSelected();

int mu_widgetHeight(widget);
int mu_widgetWidth(widget);

void mu_drawWidget(widget);
void mu_keypressWidget(widget *, int, int, int);
void mu_mouseclickWidget(widget *, int, int, int);
void mu_mousemoveWidget(widget *, double, double);

void mu_keypress(int, int, int);
void mu_mouseclick(int, int, int);
void mu_mousemove(double, double);

void mu_drawBackground();

#endif
