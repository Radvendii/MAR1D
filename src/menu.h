#ifndef _MENU_H
#define _MENU_H

#include <SDL_opengl.h>
#include <GL/glu.h>
#include <SDL.h>
#include "helpers.h"
#include "graphics.h"
#include "parsing.h"

// position of the menu on the screen
#define k_menuX 90
#define k_menuY (k_menuWindowH - 250)

// constants for menu formatting and spacing
#define k_headingSpace (k_fontSpaceY(false) * k_fontSize * 1.5)
#define k_selSpace (k_fontCharX + 10)

// constants for menu widget properties

// dimensions
//TODO: make k_sliderW dependent on the w.max in the case of line width
//      (at the moment they just happen to be the same)
#define k_sliderW 100
#define k_sliderH 10
#define k_switchW 40
#define k_switchH 14
#define k_keybindW 150
#define k_keybindH 14
#define k_switchButtonW (k_switchW * 0.5)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// constants to define the animation from menu to the actual game
#define k_menuAnimTime 200
#define k_menuAnimStartRotation (k_menuAnimTime / 2)

// location of mario on the main menu background
#define k_menuAnimMarioX 310.0
#define k_menuAnimMarioY 82.0

// we shift forward this amount as we rotate
#define k_menuAnimMarioXShift 10

// how much to zoom in on mario
#define k_menuAnimScale 10.0

/*
 * define a menu from a list of widgets. This is useful for initializing nested
 * menus. the MENU() macro adds a back button to the menu, _MENU() does not.
 * - .ws will need to be freed.
 * - .nWs is set appropriately.
 * - .sel is initialized to 0
 * - .p is not set.
 * - .heading is not set
 *
 * invoked as (notice the lack of (widget[]) in the argument)
 * MENU(
 *   {
 *     (widget) { ... },
 *     (widget) { ... }
 *   }
 * )
 */

#define MENU(...) MENU_EXPAND(__VA_ARGS__)
#define MENU_EXPAND(...) MENU_EXPANDED(__VA_ARGS__)
#define MENU_EXPANDED(...)                      \
  _MENU(                                        \
    APPEND(                                     \
      WIDGET(                                   \
        .label = "BACK",                        \
        .kind = WK_ACTION,                      \
        .action = &mu_goParent                  \
      ),                                        \
      ##__VA_ARGS__                             \
    )                                           \
  )

#define _MENU(...)                                                      \
  (menu) {                                                              \
    .ws = INIT_ARR_ON_HEAP(widget, { __VA_ARGS__ }),                    \
    .nWs = sizeof((widget[]) { __VA_ARGS__ }) / sizeof(widget),         \
    .sel = 0                                                            \
  }

#define WIDGET(...)                                                     \
  (                                                                     \
    (widget) {                                                          \
      __VA_ARGS__                                                       \
    }                                                                   \
  )

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
  char *heading;
  menu *p;    // the parent menu containing this one (null if this is the highest level)
  widget *ws; // array of widgets
  int nWs;    // number of widgets in `ws` array
  int sel;    // the index of the selected widget
};

enum widgetKind { WK_MENU,      // menu
                  WK_SLIDER,    // int value slider
                  WK_SWITCH,    // bool value switch
                  WK_KEYBIND,   // attaches a key to an in-game action
                  WK_ACTION,    // perform an action when selected
                  WK_TEXT };    // display text

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
    struct { // WK_KEYBIND
      SDL_Keycode *keyVal;
      bool active; // whether we are waiting for a keypress to overwrite the key
    };
    struct { // WK_ACTION
      void (*action)(void);
    };
    struct { // WK_TEXT
      char *text;
      float size;
    };
    // WK_TEXT just uses .label
  };
};

void mu_setParents(menu *m, menu *p);
void mu_setHeadings(menu *m, char *heading);

void mu_main();
void mu_update();

void mu_init();
void mu_deinit();

void mu_deleteMenu(menu *);

void mu_startGame();
void mu_quit();
void mu_goParent();
void mu_resetKeys();
void mu_saveConfig();

void mu_drawSelected(float, float);

int mu_labelSpace(menu);

void mu_drawMenu(menu, float, float);
void mu_keypressMenu(menu *, int, int, int);
void mu_mouseclickMenu(menu *, int, int, int, int);
void mu_mousemoveMenu(menu *, int, int, int);

float mu_widgetH(widget);
void mu_drawWidget(int labelSpace, bool selected, widget, float, float);
bool mu_keypressWidget(widget *, int, int, int);
void mu_mouseclickWidget(widget *, int, int, int, int, int);
void mu_mousemoveWidget(widget *, int, int, int, int);

void mu_keypress(SDL_KeyboardEvent);
void mu_mouseclick(SDL_MouseButtonEvent);
void mu_mousemove(SDL_MouseMotionEvent);

void mu_drawBackground();

#endif
