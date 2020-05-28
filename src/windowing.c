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

void click_callback(int window, int button, int action, int mods){
  cl_click(button, action, mods);
}

void key_callback(int window, SDL_Keycode key, SDL_Scancode scancode, int action, int mods){
  if (key == SDLK_ESCAPE && action == SDL_PRESSED){
    quit = true;
  }

  if (key == SDLK_j && action == SDL_PRESSED){
    debug = !debug;
  }
  cl_keypress(key, scancode, action, mods);
  gr_keypress(key, scancode, action, mods);
}

void cursor_callback(int window, double xPos, double yPos){
  gr_cursormove(xPos, yPos);
}

void wn_init(){
  if(SDL_Init(SDL_INIT_VIDEO & SDL_INIT_EVENTS)){
    printf("Unable to initialize SDL: %s", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  /* glfwWindowHint(GLFW_RESIZABLE, false); */
  perspWindow = SDL_CreateWindow(k_perspWindowName, k_perspWindowX, 50, k_perspWindowW, k_perspWindowH, SDL_WINDOW_OPENGL);
  dimWindow = SDL_CreateWindow(k_dimWindowName, k_dimWindowX, 50, k_dimWindowW, k_dimWindowH, SDL_WINDOW_OPENGL);
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

  /* glfwSetWindowAttrib(dimWindow, GLFW_FOCUS_ON_SHOW, false); // when the debug window shows, it shouldn't move focus */
  SDL_SetWindowResizable(perspWindow, SDL_FALSE);
  SDL_SetWindowResizable(dimWindow, SDL_TRUE);

  wn_disable_mouse(true);
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

  /* TODO: all of this is bad. move it, change it, something.
   * It's glfw-style but in sdl
   */
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_KEYDOWN:
      case SDL_KEYUP:
        key_callback(event.key.windowID, event.key.keysym.sym, event.key.keysym.scancode, event.key.state, event.key.keysym.mod);
        break;
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
        click_callback(event.button.windowID, event.button.button, event.button.state, 0);
        break;
      case SDL_MOUSEMOTION:
        cursor_callback(event.motion.windowID, event.motion.x, event.motion.y);
        break;
    }
  }
  /* glfwPollEvents(); */
}

void wn_disable_mouse(bool disable) {
  SDL_CaptureMouse(!disable);
  // glfwSetInputMode(perspWindow, GLFW_CURSOR, (disable ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL));
}

void wn_deinit(){
  SDL_DestroyWindow(perspWindow);
  SDL_DestroyWindow(dimWindow);
  SDL_Quit();
}
