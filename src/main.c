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
#include "menu.h"
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

int main(int argc, char **argv){

  quit = false;

  // resources must be initialized before io_readConfig()
  // but settings must be loaded before we can initialize the rest
  rs_init();

  // default values

  conf = k_defaultConf;

  // pull config from file
  io_readConfig(&conf);

#ifndef _WIN32
  /* Look, I'm not going to bother porting this to windows.
   * If you want command line options, you can get a real OS.
   * Or you can port it yourself and submit a pull request. */

  // overwrite default / conf file settings with command line options.
  int c;
  while ((c = getopt(argc, argv, "m:f:w:s:y")) != -1){
    switch (c){
    case 'm':
      conf.music = atoi(optarg);
      break;
    case 'f':
      conf.effects = atoi(optarg);
      break;
    case 'w':
      conf.lineSize = atoi(optarg);
      break;
    case 's':
      conf.sensitivity = atoi(optarg);
      break;
    case 'y':
      conf.invertMouseY = true;
      break;
    }
  }
#endif

  // Order matters for intializations
  mu_init();
  ob_init();
  gl_init();
  wn_init();
  au_init();
  gr_init();

  mu_main(); // start the program with the menu

  // deinit in reverse order
  gr_deinit();
  au_deinit();
  wn_deinit();
  gl_deinit();
  ob_deinit();
  mu_deinit();
  rs_deinit();
  exit(EXIT_SUCCESS);

  return 0;
}
