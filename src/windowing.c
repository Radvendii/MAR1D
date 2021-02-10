#include "windowing.h"

SDL_Window* dimWindow;
SDL_Window* perspWindow;

void (*keypressCallback)(SDL_KeyboardEvent);
void (*mouseclickCallback)(SDL_MouseButtonEvent);
void (*mousemoveCallback)(SDL_MouseMotionEvent);

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
  SDL_GL_MakeCurrent(dimWindow, dimContext);
}

void wn_eventCallbacks(void (*keypress)(SDL_KeyboardEvent),
                       void (*mouseclick)(SDL_MouseButtonEvent),
                       void (*mousemove)(SDL_MouseMotionEvent)) {
  wn_keypressCallback(keypress);
  wn_mouseclickCallback(mouseclick);
  wn_mousemoveCallback(mousemove);
}

void wn_keypressCallback(void (*callback)(SDL_KeyboardEvent)) {
  keypressCallback = callback;
}

void wn_mouseclickCallback(void (*callback)(SDL_MouseButtonEvent)) {
  mouseclickCallback = callback;
}

void wn_mousemoveCallback(void (*callback)(SDL_MouseMotionEvent)) {
  mousemoveCallback = callback;
}

void wn_processEvents(){
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_KEYDOWN:
      case SDL_KEYUP:
        wn_keypress(event.key);
        break;
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
        wn_mouseclick(event.button);
        break;
      case SDL_MOUSEMOTION:
        wn_mousemove(event.motion);
        break;
    case SDL_WINDOWEVENT:
      if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
        if (event.window.windowID == SDL_GetWindowID(perspWindow)) {
          // TODO: should this just exit()?
          quit = true;
        }
        else if (event.window.windowID == SDL_GetWindowID(dimWindow)) {
          SDL_HideWindow(dimWindow);
        }
      }
      default:
        break;
    }
  }
}

void wn_keypress(SDL_KeyboardEvent ev){
  if (keypressCallback) {
    (*keypressCallback)(ev);
  }
  else {
    if (ev.keysym.sym == SDLK_ESCAPE && ev.state == SDL_PRESSED){
      quit = true;
    }
  }
}

void wn_mouseclick(SDL_MouseButtonEvent ev){
  if (mouseclickCallback) {
    (*mouseclickCallback)(ev);
  }
}

void wn_mousemove(SDL_MouseMotionEvent ev){
  if (mousemoveCallback) {
    (*mousemoveCallback)(ev);
  }
}

void wn_init(){
  if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS)){
    printf("Unable to initialize SDL: %s", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  perspWindow = SDL_CreateWindow(k_perspWindowName, k_perspWindowX, 100, k_perspWindowW, k_perspWindowH, SDL_WINDOW_OPENGL);
  dimWindow = SDL_CreateWindow(k_dimWindowName, k_dimWindowX, 100, k_dimWindowW, k_dimWindowH, SDL_WINDOW_OPENGL);
  if (!perspWindow || !dimWindow) {
    printf("Unable to create SDL windows: %s\n", SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
  }

  SDL_SetWindowResizable(perspWindow, SDL_FALSE);
  SDL_SetWindowResizable(dimWindow, SDL_FALSE);

  if (!conf.debug) {
    SDL_HideWindow(dimWindow);
  }

  glLineWidth(1.5f);
  perspContext = SDL_GL_CreateContext(perspWindow);
  dimContext = SDL_GL_CreateContext(dimWindow);

  if (!perspContext || !dimContext) {
    printf("Unable to create OpenGL contexts: %s\n", SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
  }

  wn_keypressCallback(&wn_keypress);
  wn_mouseclickCallback(&wn_mouseclick);
  wn_mousemoveCallback(&wn_mousemove);
}

void wn_update(){
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
