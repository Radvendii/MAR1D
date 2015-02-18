#include "windowing.h"

void wn_perspWindow(){
    glfwMakeContextCurrent(perspWindow);
}

void wn_dimWindow(){
    glfwMakeContextCurrent(dimWindow);
}

void wn_update(){
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
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    dimWindow = glfwCreateWindow(k_dimWindowW, k_dimWindowH, "dimWindow", NULL, NULL);
    perspWindow = glfwCreateWindow(k_perspWindowW, k_perspWindowH, "perspWindow", NULL, NULL);
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
    glfwSetWindowPos(dimWindow, 100, 50);
    glfwSetWindowPos(perspWindow, 900, 50);

    glfwSetInputMode(perspWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    glLineWidth(1.5f);

    glfwSetKeyCallback(perspWindow, key_callback);
    glfwSetKeyCallback(dimWindow, key_callback);
    glfwSetCursorPosCallback(perspWindow, cursor_callback);
}

void wn_deinit(){
    glfwDestroyWindow(dimWindow);
    glfwDestroyWindow(perspWindow);
    glfwTerminate();
}