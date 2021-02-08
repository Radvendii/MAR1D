#include "menu.h"

image imBg; // menu background
image imSel; // image that appears to the left of selected item

menu main_menu;
menu *active_menu;

void mu_init() {

  // initialize menu
  // TODO: read this in from a (json?) resource file
  main_menu = _MENU(
    WIDGET(
      .label = "START GAME",
      .kind = WK_ACTION,
      .action = &mu_startGame
    ),
    WIDGET(
      .label = "EXPLANATION",
      .kind = WK_MENU,
      .m = MENU(
        WIDGET(
          .label = "",
          .kind = WK_TEXT,
          .text =
            "FIRST PERSON MARIO. THE WORLD IS\n"
            "STILL 2D. YOU SEE THAT WORLD AS\n"
            "MARIO DOES: A 1D LINE.\n"
            "\n"
            "MORE AT MAR1D.COM\n",
          .size = 1.5
        )
      )
    ),
    WIDGET(
      .label = "OPTIONS",
      .kind = WK_MENU,
      .m = MENU(
        WIDGET(
          .label = "CONTROLS",
          .kind = WK_MENU,
          .m = MENU(
            WIDGET(
              .label = "KEYBOARD",
              .kind = WK_MENU,
              .m = MENU(
                WIDGET(
                  .label = "FORWARD",
                  .kind = WK_KEYBIND,
                  .keyVal = &conf.keys.forward,
                  .active = false
                ),
                WIDGET(
                  .label = "BACKWARD",
                  .kind = WK_KEYBIND,
                  .keyVal = &conf.keys.backward,
                  .active = false
                ),
                WIDGET(
                  .label = "TURN AROUND",
                  .kind = WK_KEYBIND,
                  .keyVal = &conf.keys.turn,
                  .active = false
                ),
                WIDGET(
                  .label = "JUMP",
                  .kind = WK_KEYBIND,
                  .keyVal = &conf.keys.jump,
                  .active = false
                ),
                WIDGET(
                  .label = "RUN",
                  .kind = WK_KEYBIND,
                  .keyVal = &conf.keys.run,
                  .active = false
                ),
                WIDGET(
                  .label = "CROUCH",
                  .kind = WK_KEYBIND,
                  .keyVal = &conf.keys.crouch,
                  .active = false
                ),
                WIDGET(
                  .label = "RESET",
                  .kind = WK_ACTION,
                  .action = &mu_resetKeys
                )
              )
            ),
            WIDGET(
              .label = "MOUSE",
              .kind = WK_MENU,
              .m = MENU(
                WIDGET(
                  .label = "SENSITIVITY",
                  .kind = WK_SLIDER,
                  .sliderVal = &conf.sensitivity,
                  .inc = 1,
                  .min = 1,
                  .max = 20
                ),
                WIDGET(
                  .label = "INVERT Y",
                  .kind = WK_SWITCH,
                  .switchVal = &conf.invertMouseY
                )
              )
            )
          )
        ),
        WIDGET(
          .label = "GRAPHICS",
          .kind = WK_MENU,
          .m = MENU(
            WIDGET(
              .label = "LINE WIDTH",
              .kind = WK_SLIDER,
              .sliderVal = &conf.lineSize,
              .inc = 5,
              .min = 1,
              .max = 100
            ),
            WIDGET(
              .label = "",
              .kind = WK_TEXT,
              .text =
                "CHANGE THE WIDTH OF THE DISPLAY\n"
                "LINE. THIS DOES NOT MAKE IT 2D.\n"
                "THERE IS ONLY 1D OF INFORMATION.\n",
              .size = 1.5
            )
          )
        ),
        WIDGET(
          .label = "SOUND",
          .kind = WK_MENU,
          .m = MENU(
            WIDGET(
              .label = "MUSIC",
              .kind = WK_SLIDER,
              .sliderVal = &conf.music,
              .min = 0,
              .max = MIX_MAX_VOLUME,
              .inc = 8
            ),
            WIDGET(
              .label = "EFFECTS",
              .kind = WK_SLIDER,
              .sliderVal = &conf.effects,
              .min = 0,
              .max = MIX_MAX_VOLUME,
              .inc = 8
            )
          )
        ),
        WIDGET(
          .label = "SAVE",
          .kind = WK_ACTION,
          .action = &mu_saveConfig
        )
      )
    ),
    WIDGET(
      .label = "SOURCE CODE",
      .kind = WK_MENU,
      .m = MENU(
        WIDGET(
          .label = "",
          .kind = WK_TEXT,
          .text =
            "MAR1D IS FREE SOFTWARE LICENSED\n"
            "UNDER AGPL. THE SOURCE CAN BE\n"
            "FOUND AT MAR1D.COM/SOURCE\n",
          .size = 1.5
        )
      )
    ),
    WIDGET(
      .label = "QUIT",
      .kind = WK_ACTION,
      .action = &mu_quit
    )
  );

  // TODO: I really, really want this to be incorporated in the macros above,
  //       and not it's own function. But I can't figure out an elegant way to do it.
  //       I could do it if .m were a pointer to a menu, and not a menu itself
  mu_setParents(&main_menu, NULL);
  //       this one could easily be moved into the macros. just add a set_heading
  //       function call to the WIDGET() macro that checks if it's a menu and if so sets the heading
  mu_setHeadings(&main_menu, "MAIN MENU");

  active_menu = &main_menu;

  // initialize images
  imBg = io_getImage("menuscreen_bg.bmp");
  imSel = io_getImage("selected.bmp");
}

// recursively set all menu.p to the menu that contains it
void mu_setParents(menu *m, menu *p) {
  for(int i=0; i < m->nWs; i++) {
    if (m->ws[i].kind == WK_MENU) {
      mu_setParents(&m->ws[i].m, m);
    }
  }
  m->p = p;
}

// recursively set all menu.heading to the label of the widget containing it
// and the initial one to the input
void mu_setHeadings(menu *m, char *heading) {
  for(int i=0; i < m->nWs; i++) {
    if (m->ws[i].kind == WK_MENU) {
      mu_setHeadings(&m->ws[i].m, m->ws[i].label);
    }
  }
  m->heading = heading;
}

void mu_quit() {
  quit = true;
}

// TODO: better name?
void mu_goParent() {
  active_menu->sel = 0;
  active_menu = active_menu->p;
}

void mu_resetKeys() {
  conf.keys = k_defaultKeys;
}

void mu_saveConfig() {
  io_writeConfig(conf);
  // TODO: goParent() shouldn't really be in the saveConfig() function. really there
  // should be a separate function for the menu item.
  mu_goParent();
}

void mu_menuMatrix() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glOrtho(0, k_menuWindowW, 0, k_menuWindowH, -1, 1);
}

void mu_backgroundMatrix() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void mu_startGame() {
  wn_menuWindow();
  mu_backgroundMatrix();
  float s, rY, tX, tY;

  for (int i=0; i<k_menuAnimTime && !quit; i++) {
    wn_menuWindow();
    gr_clear();

    // translations
    // zoom in a little bit more on mario (by shifting more towards his front) as we rotate
    tX = linInterp(k_menuAnimMarioX, k_menuAnimMarioX+k_menuAnimMarioXShift,
                   k_menuAnimStartRotation, k_menuAnimTime, // interpolate during rotation
                   MAX(i, k_menuAnimStartRotation)); // start as we rotate
    tY = k_menuAnimMarioY;

    // convert pixels to screen units
    tX = linInterp(-1, 1, 0, k_menuWindowW, tX);
    tY = linInterp(-1, 1, 0, k_menuWindowH, tY);

    // scale
    s  = smoothInterp(1.0, k_menuAnimScale, 0, k_menuAnimTime, i);

    // rotation about the vertical screen axis (putting us in mario's view)
    // starts at k_menuAnimStartRotation
    rY = smoothInterp(0.0, 90.0, k_menuAnimStartRotation, k_menuAnimTime, MAX(i, k_menuAnimStartRotation));

    glPushMatrix();

    // Mostly figured this out using trial and error
    // translate, scale, translate back seems to be a way to zoom in on a point though
    glRotatef(rY, 0.0, 1.0, 0.0);
    glTranslatef(tX, tY, 0);
    glScalef(s, s, s);
    glTranslatef(-tX, -tY, 0);

    // draw rotated, scaled background
    mu_drawBackground();

    glPopMatrix();
    wn_update();
  }
  gl_main();

  // take callbacks back
  // TODO: this is janky. perhaps there should be a callback stack
  wn_eventCallbacks(&mu_keypress, &mu_mouseclick, &mu_mousemove);
}

void mu_deinit() {
  mu_deleteMenu(&main_menu);
  active_menu = NULL;
}

void mu_deleteMenu(menu *m) {
  if (m) {
    for (int i=0; i < m->nWs; i++) {
      if (m->ws[i].kind == WK_MENU) {
        mu_deleteMenu(&m->ws[i].m);
      }
    }
    free(m->ws);
  }
}

void mu_main() {
  wn_eventCallbacks(&mu_keypress, &mu_mouseclick, &mu_mousemove);

  while (!quit) {
    wn_menuWindow();
    gr_clear();

    mu_backgroundMatrix();
    mu_drawBackground();

    mu_menuMatrix();
    mu_drawMenu(*active_menu, k_menuX, k_menuY);
    wn_update();
    mu_update();
  }
}

void mu_update() {
  // can't select text widgets
  while (active_menu->ws[active_menu->sel].kind == WK_TEXT) {
    active_menu->sel++;
  }
}

int mu_labelSpace(menu m) {

  // this value will help align the widgets to start in the same place
  // after their respective labels
  int labelSpace = 0;

  for (int i=0; i<m.nWs; i++) {
    widget wi = m.ws[i];
    BOUND_BELOW(labelSpace, strlen(wi.label) * k_fontSpaceX(false) * k_fontSize);
  }

  // if there's no labels, the labels don't need padding
  // otherwise, pad by a few character widths
  if (labelSpace > 0) {
    labelSpace += k_fontSpaceX(false) * 2 * k_fontSize;
  }

  return labelSpace;
}

void mu_drawMenu(menu m, float x, float y) {

  gr_text(k_colorTextLit, false, m.heading, k_fontSize, x, y);

  float yi = y - k_headingSpace;

  for (int i=0; i<m.nWs; i++) {
    widget wi = m.ws[i];

    mu_drawWidget(mu_labelSpace(m), i == m.sel, wi, x, yi);

    yi -= mu_widgetH(wi);
  }
}

float mu_widgetH(widget w) {
  float size = k_fontSize;
  char *str = w.label;

  // for now this doesn't depend much on the type of widget, so we'll just special-case it
  if (w.kind == WK_TEXT) {
    size = w.size;
    str = w.text;
  }

  int nLines = 1;

  for (int i=0; str[i] != '\0'; i++) {
    if (str[i] == '\n') {
      nLines++;
    }
  }

  return k_fontSpaceY(false) * size * nLines;
}

// draw the mushroom symbol that indicates the selected widget
void mu_drawSelected(float x, float ymid) {
  gr_image(&imSel, RECT_LCWH(x-2, ymid, k_fontCharX, k_fontCharX));
}

void mu_drawWidget(int labelSpace, bool selected, widget w, float x, float y) {
  float ymid = y - k_fontCharY / 2; // middle of the text line

  if (selected) {
    mu_drawSelected(x, ymid);
  }

  x += k_selSpace;

  // All widgets (so far) have their label displayed in front.
  // If you add a widget for which this is not the case, this will have to be added to all of the cases
  gr_text(k_colorTextLit, false, w.label, k_fontSize, x, y);

  x += labelSpace;

  switch (w.kind) {
    case WK_SLIDER: ; // sacrifice an empty statement to appease the C gods
      float dist = linInterp(0, k_sliderW,
                             0, w.max, // TODO: unsure whether to use w.min or 0 here
                             *(w.sliderVal));

      rect slider = RECT_LCWH(x, ymid, k_sliderW, k_sliderH);
      rect fill = RECT_LCWH(x, ymid, dist, k_sliderH);
      gr_drawRect(k_colorBlue, slider);
      gr_drawBezelIn(slider);
      gr_drawRect(k_colorWhite, fill);
      break;
    case WK_SWITCH: ; // sacrifice an empty statement to appease the C gods
      rect socket = RECT_LCWH(x, ymid, k_switchW, k_switchH);
      rect button = *(w.switchVal)
        ? RECT_RCWH(x + k_switchW - 2, ymid, k_switchButtonW - 4, k_switchH - 4)
        : RECT_LCWH(x + 2, ymid, k_switchButtonW - 4, k_switchH - 4);
      gr_drawRect(*(w.switchVal) ? k_colorWhite : k_colorBlue, socket);
      gr_drawBezelIn(socket);
      gr_drawRect(k_colorBlue, button);
      gr_drawBezelOut(button);
      break;
    case WK_KEYBIND: ; // sacrifice an empty statement to appease the C gods
      const char *str = SDL_GetKeyName(*w.keyVal);
      // TODO: I don't know why that -1 has to be there ;-;
      gr_text(k_colorTextLit, false, str, 1.6, x + (k_keybindW - strlen(str) * k_fontW * 1.6) / 2, y - 1 - (k_keybindH - k_fontH * 1.6) / 2);
      (w.active ? &gr_drawBezelIn : &gr_drawBezelOut)(RECT_LTWH(x, y, k_keybindW, k_keybindH));
      break;
    case WK_TEXT:
      gr_text(k_colorTextLit, false, w.text, w.size, x - labelSpace - k_selSpace, y);
      break;
    case WK_MENU:
    case WK_ACTION:
      /* already printed label */
      break;
    default:
      fprintf(stderr, "%s not fully specified", __FUNCTION__);
      exit(EXIT_FAILURE);
      break;
  }
}

void mu_keypressMenu(menu *m, int key, int state, int mods) {
  // first see if the active widget wants the keypress
  if (!mu_keypressWidget(&(m->ws[m->sel]), key, state, mods)) {
    int dir = 0;
    if ((key == SDLK_UP || key == SDLK_w) && state == SDL_PRESSED) {
      dir = -1;
    }
    if ((key == SDLK_DOWN || key == SDLK_s) && state == SDL_PRESSED) {
      dir = 1;
    }

    if (dir) { // no need to run this on every keypress (though I suppose we could)

      int sel = m->sel;

      do {
        sel+=dir;
      }
      // skip over WK_TEXT widgets
      while (sel >= 0 && sel < m->nWs && m->ws[sel].kind == WK_TEXT);

      // If we haven't gone past the end, engage!
      if (sel >= 0 && sel < m->nWs) {
        m->sel = sel;
      }
    }

    if (key == SDLK_ESCAPE && state == SDL_PRESSED) {
      if (m->p) {
        active_menu = m->p;
      }
      else {
        mu_quit();
      }
    }
  }
}


// returns whether or not the keypress was captured
bool mu_keypressWidget(widget *w, int key, int state, int mods) {
  int dir = 0;
  bool ret = false;
  switch (w->kind) {
    case WK_MENU:
      if ((key == SDLK_RETURN || key == SDLK_SPACE) && state == SDL_PRESSED) {
        active_menu = &w->m;
        ret = true;
      }
      break;
    case WK_SLIDER:
      if ((key == SDLK_LEFT || key == SDLK_a) && state == SDL_PRESSED) {
        dir = -1;
      }
      if ((key == SDLK_RIGHT || key == SDLK_d) && state == SDL_PRESSED) {
        dir = 1;
      }

      if (dir) { // no need to run this on every keypress (though I suppose we could)
        *(w->sliderVal) += dir * w->inc;
        *(w->sliderVal) -= dir * (*(w->sliderVal) % w->inc); // make it a multiple of w->inc
        BOUND(*(w->sliderVal), w->min, w->max);
        ret = true;
      }
      break;
    case WK_SWITCH:
      if ((key == SDLK_RETURN || key == SDLK_SPACE) && state == SDL_PRESSED) {
        *(w->switchVal) ^= true;
        ret = true;
      }

      if ((key == SDLK_LEFT || key == SDLK_a) && state == SDL_PRESSED) {
        *(w->switchVal) = false;
        ret = true;
      }

      if ((key == SDLK_RIGHT || key == SDLK_d) && state == SDL_PRESSED) {
        *(w->switchVal) = true;
        ret = true;
      }
      break;
    case WK_KEYBIND:
      if (w->active) {
        if (state == SDL_PRESSED) {
          if (key != SDLK_ESCAPE && key != SDLK_q) { // reserved keys TODO: this is jank.
            *w->keyVal = key;
          }
          w->active = false;
          ret = true;
        }
      }
      else if ((key == SDLK_RETURN || key == SDLK_SPACE) && state == SDL_PRESSED) {
        w->active = true;
        ret = true;
      }
      break;
    case WK_ACTION:
      if ((key == SDLK_RETURN || key == SDLK_SPACE) && state == SDL_PRESSED) {
        w->action();
        ret = true;
      }
      break;
    case WK_TEXT:
      break;
    default:
      fprintf(stderr, "%s not fully specified", __FUNCTION__);
      exit(EXIT_FAILURE);
      break;
  }
  return ret;
}

void mu_mouseclickMenu(menu *m, int button, int state, int xPos, int yPos) {
  int y = yPos - (k_headingSpace - k_fontPadLine * k_fontSize / 2);
  int x = xPos - k_selSpace;

  if (y > 0 && xPos > 0) { // if mouse is pressed, don't change selection
    int i;
    for (i=0; i<m->nWs && y > mu_widgetH(m->ws[i]); i++) {
      y -= mu_widgetH(m->ws[i]);
    }
  }
  mu_mouseclickWidget(&(m->ws[m->sel]), button, state, x, y, mu_labelSpace(*m));
}

void mu_mouseclickWidget(widget *w, int button, int state, int xPos, int yPos, int labelSpace) {
  int x = xPos - labelSpace;
  switch (w->kind) {
    case WK_MENU:
      active_menu = &w->m;
      break;
    case WK_SLIDER:
      *(w->sliderVal) = x * w->max / k_sliderW;
      BOUND(*(w->sliderVal), w->min, w->max);
      break;
    case WK_SWITCH:
      *(w->switchVal) ^= true;
      break;
    case WK_KEYBIND:
      w->active ^= true;
      break;
    case WK_ACTION:
      w->action();
      break;
    case WK_TEXT:
      break;
    default:
      fprintf(stderr, "%s not fully specified", __FUNCTION__);
      exit(EXIT_FAILURE);
      break;
  }
}

void mu_mousemoveMenu(menu *m, int xPos, int yPos, int state) {
  int y = yPos - (k_headingSpace - k_fontPadLine * k_fontSize / 2);
  int x = xPos - k_selSpace;

  if (y > 0 && xPos > 0) { // if mouse is pressed, don't change selection
    int i;
    for (i=0; i<m->nWs && y > mu_widgetH(m->ws[i]); i++) {
      y -= mu_widgetH(m->ws[i]);
    }
    if (i < m->nWs && !state) {
      m->sel = i;
    }
  }
  mu_mousemoveWidget(&(m->ws[m->sel]), x, y, state, mu_labelSpace(*m));
}

void mu_mousemoveWidget(widget *w, int xPos, int yPos, int state, int labelSpace) {
  switch (w->kind) {
    case WK_MENU:
    case WK_SWITCH:
    case WK_KEYBIND:
    case WK_ACTION:
    case WK_TEXT:
        break;
    case WK_SLIDER:
      if (state & SDL_BUTTON_LMASK) {
        mu_mouseclickWidget(w, SDL_BUTTON_LEFT, SDL_PRESSED,  xPos, yPos, labelSpace);
      }
      break;
    default:
      fprintf(stderr, "%s not fully specified", __FUNCTION__);
      exit(EXIT_FAILURE);
      break;
  }
}

void mu_keypress(SDL_KeyboardEvent ev){
  mu_keypressMenu(active_menu, ev.keysym.sym, ev.state, ev.keysym.mod);
}

void mu_mouseclick(SDL_MouseButtonEvent ev){
  if (ev.state == SDL_PRESSED) {
    mu_mouseclickMenu(active_menu, ev.button, ev.state, ev.x - k_menuX, ev.y - (k_menuWindowH - k_menuY));
  }
}

void mu_mousemove(SDL_MouseMotionEvent ev){
  mu_mousemoveMenu(active_menu, ev.x - k_menuX, ev.y - (k_menuWindowH - k_menuY), ev.state);
}

void mu_drawBackground() {
  gr_image(&imBg, RECT_LTRB(-1, 1, 1, -1));
}
