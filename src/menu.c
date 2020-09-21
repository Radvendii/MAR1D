#include "menu.h"

menu main_menu;
menu *active_menu;

void mu_init() {

  // initialize menu
  main_menu = (menu) { .sel = 0,
                       .nWs = 3,
                       .ws = salloc(sizeof(widget) * 3) };

  main_menu.ws[0] = (widget) { .label = "START GAME",
                               .kind = WK_ACTION,
                               .action = &mu_startGame };

  main_menu.ws[1] = (widget) { .label = "OPTIONS",
                               .kind = WK_MENU,
                               .m = (menu) { .p = &main_menu,
                                             .sel = 0,
                                             .nWs = 5,
                                             .ws = salloc(sizeof(widget) * 5) }};

  main_menu.ws[1].m.ws[0] = (widget) { .label = "MUTE",
                                       .kind = WK_SWITCH,
                                       .switchVal = &conf.mute };

  main_menu.ws[1].m.ws[1] = (widget) { .label = "EFFECTS",
                                       .kind = WK_SWITCH,
                                       .switchVal = &conf.effects };

  main_menu.ws[1].m.ws[2] = (widget) { .label = "LINE WIDTH",
                                       .kind = WK_SLIDER,
                                       .sliderVal = &conf.lineSize,
                                       .inc = 5,
                                       .min = 1,
                                       .max = 100 };

  main_menu.ws[1].m.ws[3] = (widget) { .label = "SENSITIVITY",
                                       .kind = WK_SLIDER,
                                       .sliderVal = &conf.sensitivity,
                                       .inc = 1,
                                       .min = 1,
                                       .max = 20 };

  main_menu.ws[1].m.ws[4] = (widget) { .label = "INVERT Y",
                                       .kind = WK_SWITCH,
                                       .switchVal = &conf.invertMouseY };

  main_menu.ws[2] = (widget) { .label = "QUIT",
                               .kind = WK_ACTION,
                               .action = &mu_quit };
  active_menu = &main_menu;
}

void mu_quit() {
  quit = true;
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
  free(main_menu.ws[0].m.ws);
  free(main_menu.ws);
  active_menu = NULL;
}

void mu_main() {
  wn_eventCallbacks(&mu_keypress, &mu_mouseclick, &mu_mousemove);

  while (!quit) {
    wn_menuWindow();
    gr_clear();

    mu_backgroundMatrix();
    mu_drawBackground();

    mu_menuMatrix();
    mu_drawMenu(*active_menu, 100, k_menuWindowH - 260);
    wn_update();
  }
}

void mu_drawMenu(menu m, float x, float y) {

  // gr_text(false, w.label, x, y);

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

  for (int i=0; i<m.nWs; i++) {
    widget wi = m.ws[i];

    float yi = y - i * k_fontSpaceY(false) - k_headingSpace;

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
  float ymid = y - k_fontHeight * k_fontSize / 2;
  color textColor = selected ? k_colorTextLit : k_colorTextDim;
  // All widgets (so far) have their label displayed in front.
  // If you add a widget for which this is not the case, this will have to be added to all of the cases
  gr_text(textColor, false, w.label, x, y);
  switch (w.kind) {
    case WK_MENU:
      gr_text(textColor, false, w.label, x, y);
      break;
    case WK_SLIDER:
      gr_text(textColor, false, w.label, x, y);
      float dist = linInterp(0, k_sliderW,
                             0, w.max, // TODO: unsure whether to use w.min or 0 here
                             *(w.sliderVal));

      gr_rectLCWH(k_colorWidgetBGDim, x + labelSpace, ymid, k_sliderW, k_sliderH);
      gr_rectLCWH(k_colorWidgetFGLit, x + labelSpace, ymid, dist, k_sliderH);
      break;
    case WK_SWITCH:
      gr_text(textColor, false, w.label, x, y);

      // Switch socket rectangle
      gr_rectLCWH(*(w.switchVal) ? k_colorWidgetBGLit : k_colorWidgetBGDim, x + labelSpace, ymid, k_switchW, k_switchH);
      // Switch internal rectangle (1 pixel border)
      gr_rectLCWH(*(w.switchVal) ? k_colorWidgetFGLit : k_colorWidgetFGDim, x + labelSpace + (*(w.switchVal) ? k_switchW - k_switchButtonW - 1  : 1), ymid, k_switchButtonW, k_switchH - 2);
      break;
    case WK_ACTION:
      gr_text(textColor, false, w.label, x, y);
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

void mu_mouseclickMenu(menu *m, int button, int action) {
  mu_mouseclickWidget(&(m->ws[m->sel]), button, action);
}

void mu_mouseclickWidget(widget *w, int button, int action) {
  switch (w->kind) {
    case WK_MENU:
      break;
    case WK_SLIDER:
      break;
    case WK_SWITCH:
      break;
    case WK_ACTION:
      break;
    default:
      fprintf(stderr, "%s not fully specified", __FUNCTION__);
      exit(EXIT_FAILURE);
      break;
  }
}

void mu_mousemoveMenu(menu *m, double xPos, double yPos) {
}

void mu_mousemoveWidget(widget *w, double xPos, double yPos) {
}

void mu_keypress(SDL_KeyboardEvent ev){
  mu_keypressMenu(active_menu, ev.keysym.sym, ev.state, ev.keysym.mod);
}

void mu_mouseclick(SDL_MouseButtonEvent ev){
  mu_mouseclickMenu(active_menu, ev.button, ev.state);
}

void mu_mousemove(SDL_MouseMotionEvent ev){
  mu_mousemoveMenu(active_menu, ev.x, ev.y);
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
