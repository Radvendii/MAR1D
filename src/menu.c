#include "menu.h"

menu main_menu;
menu *active_menu;

void mu_init() {

  // initialize menu
  // TODO: read this in from a (json?) resource file
  main_menu = WS_MENU(
    {
      (widget) {
        .label = "START GAME",
        .kind = WK_ACTION,
        .action = &mu_startGame
      },
      (widget) {
        .label = "OPTIONS",
        .kind = WK_MENU,
        .m = WS_MENU(
          {
            (widget) {
              .label = "MUTE",
              .kind = WK_SWITCH,
              .switchVal = &conf.mute
            },
            (widget) {
              .label = "EFFECTS",
              .kind = WK_SWITCH,
              .switchVal = &conf.effects
            },
            (widget) {
              .label = "LINE WIDTH",
              .kind = WK_SLIDER,
              .sliderVal = &conf.lineSize,
              .inc = 5,
              .min = 1,
              .max = 100
            },
            (widget) {
              .label = "SENSITIVITY",
              .kind = WK_SLIDER,
              .sliderVal = &conf.sensitivity,
              .inc = 1,
              .min = 1,
              .max = 20
            },
            (widget) {
              .label = "INVERT Y",
              .kind = WK_SWITCH,
              .switchVal = &conf.invertMouseY
            },
            // TODO: automatically add this to all submenus somehow
            (widget) {
              .label = "BACK",
              .kind = WK_ACTION,
              .action = &mu_goParent
            }
          }
        )
      },
      (widget) {
        .label = "QUIT",
        .kind = WK_ACTION,
        .action = &mu_quit
      }
    }
  );

  // TODO: I really, really want this to be incorporated in the macros above,
  // and not it's own function. But I can't figure out an elegant way to do it
  // right now, and I need to move on.
  mu_setParents(&main_menu, NULL);

  active_menu = &main_menu;
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

void mu_quit() {
  quit = true;
}

// TODO: better name?
void mu_goParent() {
  active_menu = active_menu->p;
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

  for (int i=0; i<k_menuAnimTime; i++) {
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
    s  = linInterp(1.0, k_menuAnimScale, 0, k_menuAnimTime, i);

    // rotation about the vertical screen axis (putting us in mario's view)
    // starts at k_menuAnimStartRotation
    // subLinInterp gives us a nice slowed rotatoin near the end
    rY = subLinInterp(0.0, 90.0, k_menuAnimStartRotation, k_menuAnimTime, MAX(i, k_menuAnimStartRotation));

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
  }
}

int mu_labelSpace(menu m) {

  // this value will help align the widgets to start in the same place
  // after their respective labels
  int labelSpace = 0;

  for (int i=0; i<m.nWs; i++) {
    widget wi = m.ws[i];
    BOUND_BELOW(labelSpace, strlen(wi.label) * k_fontSpaceX(false));
  }

  // if there's no labels, the labels don't need padding
  // otherwise, pad by a few character widths
  if (labelSpace > 0) {
    labelSpace += k_fontSpaceX(false) * 2;
  }

  return labelSpace;
}

void mu_drawMenu(menu m, float x, float y) {

  gr_text(k_colorTextLit, false, "FOO", x, y);

  int labelSpace = mu_labelSpace(m);

  for (int i=0; i<m.nWs; i++) {
    widget wi = m.ws[i];

    float yi = y - k_headingSpace - i * k_fontSpaceY(false);

    if (i == m.sel) {
      mu_drawSelected(x, yi);
    }

    mu_drawWidget(labelSpace, i == m.sel, wi, x + k_selSpace, yi);
  }
}

// draw the mushroom symbol that indicates the selected widget
void mu_drawSelected(float x, float y) {
  glPointSize(5.0);
  glBegin(GL_POINTS);
  glVertex2f(x, y);
  glEnd();
}

void mu_drawWidget(int labelSpace, bool selected, widget w, float x, float y) {
  float ymid = y - k_fontHeight * k_fontSize / 2; // middle of the text line
  color textColor = selected ? k_colorTextLit : k_colorTextDim;
  // All widgets (so far) have their label displayed in front.
  // If you add a widget for which this is not the case, this will have to be added to all of the cases
  gr_text(textColor, false, w.label, x, y);
  x += labelSpace;

  switch (w.kind) {
    case WK_MENU:
      /* already printed label */
      break;
    case WK_SLIDER:
      ; // sacrifice an empty statement to appease the C gods
      float dist = linInterp(0, k_sliderW,
                             0, w.max, // TODO: unsure whether to use w.min or 0 here
                             *(w.sliderVal));

      gr_rectLCWH(k_colorWidgetBGDim, x, ymid, k_sliderW, k_sliderH);
      gr_rectLCWH(k_colorWidgetFGLit, x, ymid, dist, k_sliderH);
      break;
    case WK_SWITCH:
      // Switch socket rectangle
      gr_rectLCWH(*(w.switchVal) ? k_colorWidgetBGLit : k_colorWidgetBGDim, x, ymid, k_switchW, k_switchH);
      // Switch internal rectangle (1 pixel border)
      gr_rectLCWH(*(w.switchVal) ? k_colorWidgetFGLit : k_colorWidgetFGDim, x + (*(w.switchVal) ? k_switchW - k_switchButtonW - 1  : 1), ymid, k_switchButtonW, k_switchH - 2);
      break;
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
  int dir = 0;
  if ((key == SDLK_UP || key == SDLK_w) && state == SDL_PRESSED) {
    dir = -1;
  }
  if ((key == SDLK_DOWN || key == SDLK_s) && state == SDL_PRESSED) {
    dir = 1;
  }

  if (dir) { // no need to run this on every keypress (though I suppose we could)
    m->sel += dir;
    BOUND(m->sel, 0, m->nWs-1);
  }

  if (key == SDLK_ESCAPE && state == SDL_PRESSED) {
    if (m->p) {
      active_menu = m->p;
    }
    else {
      mu_quit();
    }
  }

  mu_keypressWidget(&(m->ws[m->sel]), key, state, mods);
}


void mu_keypressWidget(widget *w, int key, int state, int mods) {
  int dir = 0;
  switch (w->kind) {
    case WK_MENU:
      if ((key == SDLK_RETURN || key == SDLK_SPACE) && state == SDL_PRESSED) {
        active_menu = &w->m;
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
      }
      break;
    case WK_SWITCH:
      if ((key == SDLK_RETURN || key == SDLK_SPACE) && state == SDL_PRESSED) {
        *(w->switchVal) ^= true;
      }

      if ((key == SDLK_LEFT || key == SDLK_a) && state == SDL_PRESSED) {
        *(w->switchVal) = false;
      }

      if ((key == SDLK_RIGHT || key == SDLK_d) && state == SDL_PRESSED) {
        *(w->switchVal) = true;
      }
      break;
    case WK_ACTION:
      if ((key == SDLK_RETURN || key == SDLK_SPACE) && state == SDL_PRESSED) {
        w->action();
      }
      break;
    default:
      fprintf(stderr, "%s not fully specified", __FUNCTION__);
      exit(EXIT_FAILURE);
      break;
  }
}

void mu_mouseclickMenu(menu *m, int button, int state, int xPos, int yPos) {
  mu_mouseclickWidget(&(m->ws[m->sel]), button, state, xPos - k_selSpace, yPos - k_headingSpace - m->sel * k_fontSpaceY(false), mu_labelSpace(*m));
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
    case WK_ACTION:
      w->action();
      break;
    default:
      fprintf(stderr, "%s not fully specified", __FUNCTION__);
      exit(EXIT_FAILURE);
      break;
  }
}

void mu_mousemoveMenu(menu *m, int xPos, int yPos, int state) {
  if (yPos > k_headingSpace &&
      yPos < k_headingSpace + m->nWs * k_fontSpaceY(false) &&
      xPos > 0
    ) {
    if (!state) {
      m->sel = (yPos - k_headingSpace) / k_fontSpaceY(false);
    }
    mu_mousemoveWidget(&(m->ws[m->sel]), xPos - k_selSpace, yPos - k_headingSpace - m->sel * k_fontSpaceY(false), state, mu_labelSpace(*m));
  }
}

void mu_mousemoveWidget(widget *w, int xPos, int yPos, int state, int labelSpace) {
  switch (w->kind) {
    case WK_MENU:
    case WK_SWITCH:
    case WK_ACTION:
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
  glColor3f(1.f, 1.f, 1.f);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  glBegin(GL_QUADS);
  glTexCoord2f(0.f, 0.f); glVertex2f(-1, -1);
  glTexCoord2f(1.f, 0.f); glVertex2f(1, -1);
  glTexCoord2f(1.f, 1.f); glVertex2f(1, 1);
  glTexCoord2f(0.f, 1.f); glVertex2f(-1, 1);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}
