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
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_N && action == GLFW_PRESS){
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        int w,h;
        glfwGetFramebufferSize(window, &w, &h);
        int i=0;
        do{i+=3;}while(s.world.scene[i]!=terminator);
        s.world.scene[i] = objGround;
        s.world.scene[i+1] = (int)((x-w/2)/w*k_drawD*2);
        s.world.scene[i+2] = (int)((h/2-y)/h*k_drawD*2);
        s.world.scene[i+3] = terminator;
    }
    if (key == GLFW_KEY_J && action == GLFW_PRESS){
        debug = !debug;
    }
    cl_keypress(key, scancode, action, mods);
}

void cursor_callback(GLFWwindow *window, double xPos, double yPos){
    cl_cursormove(xPos, yPos);
}

void wn_init(){
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    dimWindow = glfwCreateWindow(800, 800, "2d view. For debugging only.", NULL, NULL);
    perspWindow = glfwCreateWindow(50, 800, "Perspective view", NULL, NULL);
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
