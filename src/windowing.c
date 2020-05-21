#include "windowing.h"

void wn_menuWindow(){
  glfwMakeContextCurrent(perspWindow);
}

void wn_hudWindow(){
  glfwMakeContextCurrent(perspWindow);
}

void wn_perspWindow(){
  glfwMakeContextCurrent(perspWindow);
}

void wn_dimWindow(){
  glfwMakeContextCurrent(dimWindow);
}

void wn_update(){
  if(debug && !glfwGetWindowAttrib(dimWindow, GLFW_VISIBLE)){
    glfwShowWindow(dimWindow);
  }
  if(!debug && glfwGetWindowAttrib(dimWindow, GLFW_VISIBLE)){
    glfwHideWindow(dimWindow);
  }
  glfwSwapBuffers(perspWindow);
  glfwSwapBuffers(dimWindow);
  glfwPollEvents();
}

bool wn_shouldClose(){
  return glfwWindowShouldClose(dimWindow) || glfwWindowShouldClose(perspWindow);
}

void error_callback(int error, const char* description){
  fputs(description, stderr);
}

void click_callback(GLFWwindow* w, int button, int action, int mods){
  cl_click(button, action, mods);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
    glfwSetWindowShouldClose(window, GL_TRUE);
  }

  if (key == GLFW_KEY_J && action == GLFW_PRESS){
    debug = !debug;
  }
  cl_keypress(key, scancode, action, mods);
  gr_keypress(key, scancode, action, mods);
}

void cursor_callback(GLFWwindow *window, double xPos, double yPos){
  gr_cursormove(xPos, yPos);
}

void wn_init(){
  if(!glfwInit()){
    exit(EXIT_FAILURE);
  }
  glfwSetErrorCallback(error_callback);
  glfwWindowHint(GLFW_RESIZABLE, false);
  perspWindow = glfwCreateWindow(k_perspWindowW, k_perspWindowH, k_perspWindowName, NULL, NULL);
  dimWindow = glfwCreateWindow(k_dimWindowW, k_dimWindowH, k_dimWindowName, NULL, NULL);
  if (!perspWindow)
    {
      glfwTerminate();
      exit(EXIT_FAILURE);
    }
  if (!dimWindow)
    {
      glfwTerminate();
      exit(EXIT_FAILURE);
    }
  glfwSetWindowPos(dimWindow, k_dimWindowX, 50);
  glfwSetWindowPos(perspWindow, k_perspWindowX, 50);

  glfwSetWindowAttrib(dimWindow, GLFW_FOCUS_ON_SHOW, false); // when the debug window shows, it shouldn't move focus

  wn_disable_mouse(true);
  glLineWidth(1.5f);

  glfwSetKeyCallback(perspWindow, key_callback);
  glfwSetMouseButtonCallback(perspWindow, click_callback);
  glfwSetKeyCallback(dimWindow, key_callback);
  glfwSetCursorPosCallback(perspWindow, cursor_callback);
}

void wn_disable_mouse(bool disable) {
  glfwSetInputMode(perspWindow, GLFW_CURSOR, (disable ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL));
}

void wn_deinit(){
  glfwDestroyWindow(dimWindow);
  glfwDestroyWindow(perspWindow);
  glfwTerminate();
}
