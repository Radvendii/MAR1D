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
                                             .nWs = 1,
                                             .ws = salloc(sizeof(widget) * 1) }};

  main_menu.ws[1].m.ws[0] = (widget) { .label = "LINE WIDTH",
                                       .kind = WK_SLIDER,
                                       .sliderVal = &conf.lineSize,
                                       .inc = 5,
                                       .min = 1,
                                       .max = 100 };

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

#define k_zoomUntil ((k_menuTime) / 2)
#define k_marioX 310.0
#define k_marioY 82.0

  for (int i=0; i<k_menuTime; i++) {
    wn_menuWindow();
    gr_clear();
    // Rotate the image when the user starts the game
    // TODO: zoom in on mario, either before rotating, or during
    tX = linInterp(-1, 1, 0, k_menuWindowW, k_marioX+20);
    tY = linInterp(-1, 1, 0, k_menuWindowH, k_marioY);
    s  = linInterp(1.0, 5.0, 0, k_zoomUntil, MIN(i, k_zoomUntil));
    rY = linInterp(0.0, 90.0, k_zoomUntil, k_menuTime, MAX(i, k_zoomUntil));
    glPushMatrix();

    glRotatef(rY, 0.0, 1.0, 0.0);
    glTranslatef(tX, tY, 0);
    glScalef(s, s, s);
    glTranslatef(-tX, -tY, 0);

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
  for (int i=0; i<m.nWs; i++) {
    widget wi = m.ws[i];

    float yi = y - i * k_fontSpaceY - k_headingSpace;

    if (i == m.sel) {
      mu_drawSelected(x, yi);
    }

    mu_drawWidget(wi, x + k_selSpace, yi);
  }
}

void mu_drawSelected(float x, float y) {
  glPointSize(5.0);
  glBegin(GL_POINTS);
  glVertex2f(x, y);
  glEnd();
}

void mu_drawWidget(widget w, float x, float y) {
  switch (w.kind) {
    case WK_MENU:
      gr_text(false, w.label, x, y);
      break;
    case WK_SLIDER:
      gr_text(false, w.label, x, y);
      //TODO: make k_sliderWidth dependent on the w.max in the case of line width
      //TODO: move these elsewhere
#define k_sliderWidth 100
#define k_labelSpace 170
#define k_colorWidgetBG ((color) {.r = 0x4C, .g = 0x84, .b = 0xDC})
#define k_colorWidgetFG ((color) {.r = 0xFF, .g = 0xFF, .b = 0xFF})
      float ymid = y - k_fontHeight * k_fontSize / 2;
      float dist = linInterp(0, k_sliderWidth,
                             0, w.max, // unsure whether to use w.min here
                             *(w.sliderVal));

      gr_rectLCWH(k_colorWidgetBG, x + k_labelSpace, ymid, k_sliderWidth, 10);
      gr_rectLCWH(k_colorWidgetFG, x + k_labelSpace, ymid, dist, 10);
      break;
    case WK_SWITCH:
      gr_text(false, w.label, x, y);
      gr_text(false, "SWITCH", x, y);
      break;
    case WK_ACTION:
      gr_text(false, w.label, x, y);
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
