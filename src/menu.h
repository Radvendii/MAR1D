#ifndef _MENU_H
#define _MENU_H

#include <SDL_opengl.h>
#include <GL/glu.h>
#include <SDL.h>
#include "helpers.h"
#include "graphics.h"
#include "parsing.h"

#define k_headingSpace (k_fontSpaceY * 1.5)
#define k_selSpace (k_fontSpaceX)

/*
 * Definition of datatypes for the menu
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Widget is a general structure for things that appear in menus (including
 * sub-menus), and things that are used in menus for spacing, or anything else.
 *
 * The structure is that of a doubly-linked multi-tree. That is, a menu can
 * contain an arbitrary number of widgets inside it, which can be sub-menus, or
 * they can be end-nodes. Each widget also has a pointer up to its parent widget.
 *
 * mu_drawMenu() defines how menus are displayed, laying out its widgets
 * mu_drawWidget() defines how a given widget gets displayed.
 *
 * mu_keypressMenu(), mu_mousclickMenu(), mu_mousemoveMenu() define how a menu
 * responds to user interaction, primarily determining which widget the user
 * means to interact with
 *
 * mu_keypressWidget(), mu_mouseclickWidget(), mu_mousemoveWidget() define
 * what happens when the user interacts with a widget.
 *
 * All of these items must be updated in tandem:
 * - the types available in widgetKind
 * - the widget struct (fields added for it)
 * - the mu_*Widget() functions, (a case added for it)
 */

typedef struct widget widget;
typedef struct menu menu;

struct menu {
  menu *p;    // the parent menu containing this one (null if this is the highest level)
  widget *ws; // array of widgets
  int nWs;    // number of widgets in `ws` array
  int sel;    // the index of the selected widget
};

enum widgetKind { WK_MENU,     // menu
                  WK_SLIDER,   // int value slider
                  WK_SWITCH,   // bool value switch
                  WK_ACTION }; // perform an action when selected

struct widget {
  char *label;

  enum widgetKind kind;

  union {
    menu m; // WK_MENU
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

void mu_startGame();
void mu_quit();

void mu_drawSelected(float, float);

int mu_widgetHeight(widget);
int mu_widgetWidth(widget);

void mu_drawMenu(menu, float, float);
void mu_keypressMenu(menu *, int, int, int);
void mu_mouseclickMenu(menu *, int, int);
void mu_mousemoveMenu(menu *, double, double);

void mu_drawWidget(widget, float, float);
void mu_keypressWidget(widget *, int, int, int);
void mu_mouseclickWidget(widget *, int, int);
void mu_mousemoveWidget(widget *, double, double);

void mu_keypress(SDL_KeyboardEvent);
void mu_mouseclick(SDL_MouseButtonEvent);
void mu_mousemove(SDL_MouseMotionEvent);

void mu_drawBackground();

#endif
