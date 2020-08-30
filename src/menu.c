#include "menu.h"

widget active_menu;

bool quit;

void mu_init(config *conf) {
  quit = false;

  // initialize menu
  active_menu = (widget) { .label = "",
                           .kind = WK_MENU,
                           .selected = 0,
                           .nWidgets = 2,
                           .subWidgets = salloc(sizeof(widget) * 2) };

  active_menu.subWidgets[0] = (widget) { .label = "LINE WIDTH",
                                         .kind = WK_SLIDER,
                                         .sliderVal = &conf->lineSize,
                                         .action = &quit };

  active_menu.subWidgets[1] = (widget) { .label = "QUIT",
                                         .kind = WK_ACTION,
                                         .action = &mu_quit };
}

void mu_quit() {
  quit = true;
}

void mu_deinit() {
  free(active_menu.subWidgets);
}

void mu_main() {
  while (!quit) {
    wn_menuWindow();
    gr_clear();

    /* mu_drawBackground(); */
    mu_drawWidget(active_menu);
    wn_update();
  }
}

void mu_drawSelected() {
  glPointSize(5.0);
  glBegin(GL_POINTS);
  glVertex2f(0, k_hudWindowH);
  glEnd();
}

int mu_widgetHeight(widget w) {
  return 0;
}

int mu_widgetWidth(widget w) {
  return 0;
}

void mu_drawWidget(widget w) {
  int x = 0;
  int y = k_hudWindowH;

  switch (w.kind) {
    case WK_MENU:
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glOrtho(0, k_hudWindowW, 0, k_hudWindowH, -1, 1);

      gr_text(false, w.label, x, y);
      for (int i=0; i<w.nWidgets; i++) {
        widget wi = w.subWidgets[i];

        glTranslatef(0, -k_charSpaceY, 0);

        if (i == w.selected) {
          mu_drawSelected();
        }

        glTranslatef(k_charSpaceX, 0, 0);

        switch (wi.kind) {
          case WK_MENU:
            // Don't recurse into sub-menus
            // just print their labels
            gr_text(false, wi.label, x, y);
            break;
          default:
            mu_drawWidget(wi);
            break;
        }

        glTranslatef(-k_charSpaceX, 0, 0);
      }
      break;
    case WK_SLIDER:
      gr_text(false, w.label, x, y);
      gr_text(false, "SLIDER", x, y);
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

void mu_keypressWidget(widget *w, int key, int state, int mods) {
  int dir = 0;
  switch (w->kind) {
    case WK_MENU:
      if ((key == SDLK_UP || key == SDLK_w) && state == SDL_PRESSED) {
        dir = -1;
      }
      if ((key == SDLK_DOWN || key == SDLK_s) && state == SDL_PRESSED) {
        dir = 1;
      }

      if (dir) { // no need to run this on every keypress (though I suppose we could)
        w->selected += dir;
        BOUND(w->selected, 0, w->nWidgets-1);
      }

      mu_keypressWidget(&(w->subWidgets[w->selected]), key, state, mods);
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

void mu_mouseclickWidget(widget *w, int button, int action, int mods) {
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

void mu_mousemoveWidget(widget *w, double xPos, double yPos) {
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

void mu_keypress(int key, int state, int mods){
  mu_keypressWidget(&active_menu, key, state, mods);
}

void mu_mouseclick(int button, int state, int mods){
  mu_mouseclickWidget(&active_menu, button, state, mods);
}

void mu_mousemove(double x , double y){
  mu_mousemoveWidget(&active_menu, x, y);
}

void mu_drawBackground() {
  glColor3f(1.f, 1.f, 1.f);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  if(s.menu > k_menuStatic){
    char hud[100];
    sprintf(hud, "CONGRATULATIONS!\n\nYOUVE WON LEVEL 1-1!\n\nSCORE %06d", s.score);
    glOrtho(-k_menuWindowW/2, k_menuWindowW/2, 0, k_menuWindowH, -1, 1);
    gr_text(false, hud, -k_menuWindowW/2+20, k_menuWindowH-20);
    s.menu--;
  }
  else{ // The beginning menu

    // Rotate the image when the user starts the game
    // TODO: zoom in on mario, either before rotating, or during
    glPushMatrix();
    float rot = linInterp(0.0, 90.0, k_menuTime, 0.0, s.menu);
    glRotatef(rot, 0.f, 1.f, 0.f);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.f, 0.f); glVertex2f(-1.f, -1.f);
    glTexCoord2f(1.f, 0.f); glVertex2f(1.f, -1.f);
    glTexCoord2f(1.f, 1.f); glVertex2f(1.f, 1.f);
    glTexCoord2f(0.f, 1.f); glVertex2f(-1.f, 1.f);
    glEnd();
    glPopMatrix();
    if(s.menu != k_menuStatic){
      s.menu--;
      if(!s.menu){ // End of menu animation
        gl_load();
      }
    }
    glDisable(GL_TEXTURE_2D);
  }
}
