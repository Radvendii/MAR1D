#include <GLFW/glfw3.h>
#include "helpers.h"
#include "resources.h"
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
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
struct state s;
//for when I come back to this project:
//http://8bithorse.blogspot.com/2010/11/super-mario-bros-101.html

int main(int argc, char **argv){
  bool mute = false;
  bool effects = false;
  int lineSize = 30;
  int sensitivity = 5;
  bool reverse = false;
  int c;
  while ((c = getopt(argc, argv, "mfw:s:r")) != -1){
    switch (c){
    case 'm':
      mute = true;
      break;
    case 'f':
      effects = true;
      break;
    case 'w':
      lineSize = atoi(optarg);
      break;
    case 's':
      sensitivity = atoi(optarg);
      break;
    case 'r':
      reverse = true;
      break;
    }
  }
  au_init(mute, effects);
  ob_init();
  gl_init();
  wn_init();
  gr_init(lineSize, sensitivity * (reverse ? -1 : 1));

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
