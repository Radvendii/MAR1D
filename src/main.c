#include <GLFW/glfw3.h>
#include "helpers.h"
#include "objects.h"
#include "rendering.h"
#include "gamelogic.h"
#include "controls.h"
#include "mechanics.h"
#include "graphics.h"
#include "windowing.h"
#include "parsing.h"
#include "audio.h"
#include <unistd.h>
#include <time.h>
struct state s;
//for when I come back to this project:
//http://8bithorse.blogspot.com/2010/11/super-mario-bros-101.html

int main(void){
  au_init();
  ob_init();
  gl_init();
  wn_init();
  gr_init();

  glfwSetTime(0.0);
  /* srand(time(NULL)); */

  while(!wn_shouldClose()) {
    if(s.userPaused){wn_disable_mouse(false);}
    else{wn_disable_mouse(true);}
    wn_menuWindow();
    gr_clear();
    wn_perspWindow();
    gr_clear();
    wn_dimWindow();
    gr_clear();
    wn_hudWindow();
    gr_clear();

    if(s.menu){
      s.userPaused = true;
      s.paused = true;
      wn_menuWindow();
      gr_drawMenu();
      wn_update();
    }
    else{
      gl_update();
      gr_update();
      wn_perspWindow();
      gr_drawPersp();
      wn_dimWindow();
      gr_drawDim();
      wn_hudWindow();
      gr_drawHud();
      wn_update();
    }
  }

  gr_deinit();
  wn_deinit();
  gl_deinit();
  ob_deinit();
  au_deinit();
  exit(EXIT_SUCCESS);

  return 0;
}
