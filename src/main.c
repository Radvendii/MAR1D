#include <SDL.h>
#include <libconfig.h>
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
#include <stdio.h>
#include <stdlib.h>
// Windows users don't deserve command line options.
#ifndef _WIN32
#  include <getopt.h>
#endif

struct state s;

// for when I come back to this project:
// http://8bithorse.blogspot.com/2010/11/super-mario-bros-101.html
// detailed mechanics that I definitely got wrong:
// https://www.speedrun.com/smb1/guide/pbl9d

// TODO: Should I use "atexit" and separate out a mn_init() and mn_deinit() functions?

int main(int argc, char **argv){

  // resources must be initialized before io_readConfig()
  rs_init();

  // default values
  config conf = {
    .mute = false,
    .effects = false,
    .lineSize = 30,
    .sensitivity = 10,
    .reverseMouseY = false
  };

  // Pull config from file
  io_readConfig(&conf);

#ifndef _WIN32
  /* Look, I'm not going to bother porting this to windows.
   * If you want command line options, you can get a real OS.
   * Or you can port it yourself and submit a pull request. */

  // Overwrite default / conf file configurations with command line options.
  int c;
  while ((c = getopt(argc, argv, "mfw:s:r")) != -1){
    switch (c){
    case 'm':
      conf.mute = true;
      break;
    case 'f':
      conf.effects = true;
      break;
    case 'w':
      conf.lineSize = atoi(optarg);
      break;
    case 's':
      conf.sensitivity = atoi(optarg);
      break;
    case 'r':
      conf.reverseMouseY = true;
      break;
    }
  }
#endif

  // Order matters for intializations
  ob_init();
  gl_init();
  wn_init();
  au_init(conf.mute, conf.effects);
  gr_init(conf.lineSize, conf.sensitivity * (conf.reverseMouseY ? -1 : 1));

  while(!wn_shouldClose()) {
    au_update();

    //TODO: Do this when the pause button is pressed so that it doesn't have to happen every time through the loop.
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
      // TODO: pausing while in the main menu bugs out
      s.userPaused = true;
      s.paused = true;
      wn_menuWindow();
      gr_drawMenu();
      wn_update();
    }
    else{
      au_update();
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

  // deinit in reverse order
  gr_deinit();
  au_deinit();
  wn_deinit();
  gl_deinit();
  ob_deinit();
  rs_deinit();
  exit(EXIT_SUCCESS);

  return 0;
}
