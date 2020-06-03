#include "windowing.h"

bool quit = false;
SDL_GLContext perspContext;
SDL_GLContext dimContext;

void wn_menuWindow(){
  wn_perspWindow();
}

void wn_hudWindow(){
  wn_perspWindow();
}

void wn_perspWindow(){
  SDL_GL_MakeCurrent(perspWindow, perspContext);
}

void wn_dimWindow(){
  SDL_GL_MakeCurrent(dimWindow, perspContext);
}

bool wn_shouldClose(){
  return quit; // glfwWindowShouldClose(dimWindow) || glfwWindowShouldClose(perspWindow);
}

void wn_processEvents(){
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_KEYDOWN:
      case SDL_KEYUP:
        if(!event.key.repeat){
          wn_keypress(event.key.keysym.sym, event.key.state, event.key.keysym.mod);
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
        wn_click(event.button.button, event.button.state, 0);
        break;
      case SDL_MOUSEMOTION:
        wn_mousemove(event.motion.xrel, event.motion.yrel);
        break;
      default:
        break;
    }
  }
}

void wn_click(int button, int action, int mods){
  cl_click(button, action, mods);
}

void wn_keypress(SDL_Keycode key, int state, int mods){
  if (key == SDLK_ESCAPE && state == SDL_PRESSED){
    quit = true;
  }

  if (key == SDLK_j && state == SDL_PRESSED){
    debug = !debug;
  }
  cl_keypress(key, state, mods);
  gr_keypress(key, state, mods);
}

void wn_mousemove(double xMove, double yMove){
  gr_mousemove(xMove, yMove);
}

void wn_init(){
  if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS)){
    printf("Unable to initialize SDL: %s", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  perspWindow = SDL_CreateWindow(k_perspWindowName, k_perspWindowX, 100, k_perspWindowW, k_perspWindowH, SDL_WINDOW_OPENGL);
  dimWindow = SDL_CreateWindow(k_dimWindowName, k_dimWindowX, 100, k_dimWindowW, k_dimWindowH, SDL_WINDOW_OPENGL);
  if (!perspWindow)
    {
      SDL_Quit();
      exit(EXIT_FAILURE);
    }
  if (!dimWindow)
    {
      SDL_Quit();
      exit(EXIT_FAILURE);
    }

  SDL_SetWindowResizable(perspWindow, SDL_FALSE);
  SDL_SetWindowResizable(dimWindow, SDL_FALSE);

  glLineWidth(1.5f);
  perspContext = SDL_GL_CreateContext(perspWindow);
  dimContext = SDL_GL_CreateContext(dimWindow);
}

void wn_update(){
  if(debug && SDL_GetWindowFlags(dimWindow) & SDL_WINDOW_HIDDEN){
    SDL_ShowWindow(dimWindow);
  }
  if(!debug && SDL_GetWindowFlags(dimWindow) & SDL_WINDOW_SHOWN){
    SDL_HideWindow(dimWindow);
  }
  wn_perspWindow();
  SDL_GL_SwapWindow(perspWindow);
  wn_dimWindow();
  SDL_GL_SwapWindow(dimWindow);
  wn_processEvents();
}

void wn_disable_mouse(bool disable) {
  SDL_SetRelativeMouseMode(disable);
  SDL_ShowCursor(!disable);
  SDL_CaptureMouse(disable);
}

void wn_deinit(){
  SDL_DestroyWindow(perspWindow);
  SDL_DestroyWindow(dimWindow);
  SDL_Quit();
}
