#include <unistd.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "objects.h"
#include "rendering.h"
#include "helpers.h"
#include "mechanics.h"
#include "controls.h"
#include "graphics.h"

bool debug;

void gr_update(){
    cl_gravity(&s);
    bool colis = !cl_go(&s.world, 'y', s.velY);
    cl_go(&s.world, 'x', s.velX);
    if(colis == true && s.velY <= 0) {s.onGround = true;}
    if(colis == false && (int) s.velY != 0){s.onGround = false;}

    rn_dimFworld(&lineArr, s.world);
    rn_perspFworld_v(screen, s.world, &lineArr);
    if(!debug){lineArr[0] = (line) {.x1 = 0, .x2 = 0, .y1 = 0, .y2 = 0, .r = 0, .g = 0, .b = 0};}
    if(s.camFlip){
        unsigned char swap_r;
        unsigned char swap_g;
        unsigned char swap_b;
        for(int i=0;i<k_nPixels/2;i++){
            swap_r = screen[i*3];
            swap_g = screen[i*3+1];
            swap_b = screen[i*3+2];
            screen[i*3] = screen[3*k_nPixels-(1+i)*3];
            screen[i*3+1] = screen[3*k_nPixels-(1+i)*3+1];
            screen[i*3+2] = screen[3*k_nPixels-(1+i)*3+2];
            screen[3*k_nPixels-(1+i)*3] = swap_r;
            screen[3*k_nPixels-(1+i)*3+1] = swap_g;
            screen[3*k_nPixels-(1+i)*3+2] = swap_b;
        }
    }
    //rn_perspFworld(screen, s.world);
}

static void error_callback(int error, const char* description){
    fputs(description, stderr);
}

void gr_listCollisions(){
    for(int i1=0;;i1++){
        if(s.world.scene[i1*3] == terminator) {break;}
        for(int i2=0;;i2++){
            if(s.world.scene[i2*3] == terminator) {break;}
            printf("%d collides with %d: %d\n", i1, i2, mh_isCollision(s.world, i1*3, i2*3));
        }
        printf("\n");
    }
    printf("\n");
}

void cursor_callback(GLFWwindow *window, double xPos, double yPos){
    cl_cursormove(&s, xPos, yPos);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_N && action == GLFW_PRESS){
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        int w,h;
        glfwGetFramebufferSize(window, &w, &h);
        int i=0;
        do{i+=3;}while(s.world.scene[i]!=terminator);
        s.world.scene[i] = objBrick;
        s.world.scene[i+1] = (int)((x-w/2)/w*k_drawD);
        s.world.scene[i+2] = (int)((h/2-y)/h*k_drawD);
        s.world.scene[i+3] = terminator;
    }
    if (key == GLFW_KEY_J && action == GLFW_PRESS){
        debug = !debug;
    }
    cl_keypress(&s, key, scancode, action, mods);
}

void gr_pixel(int y, unsigned char r, unsigned char g, unsigned char b){
    glBegin(GL_QUADS);
    glColor3f( (GLfloat) r/255.0, (GLfloat) g/255.0, (GLfloat) b/255.0);
    glVertex2f(-1.f, (GLfloat) -1 +y*2.0/k_nPixels);
    glVertex2f(-1.f, (GLfloat) -1+(y+1)*2.0/k_nPixels);
    glVertex2f(1.f, (GLfloat) -1+(y+1)*2.0/k_nPixels);
    glVertex2f(1.f, (GLfloat) -1 +y*2.0/k_nPixels);
    glEnd();
}

void gr_pixels(unsigned char *renderArr){
    for(int y=0;y<k_nPixels;y++){
        gr_pixel(y, renderArr[y*3+0], renderArr[y*3+1], renderArr[y*3+2]);
    }
}

void gr_lines(line *ls){
    glBegin(GL_LINES);
    for(int i=0;;i++){
        if(ob_isTerminating(ls[i])){break;}
        glColor3f( (GLfloat) ls[i].r/255.0, (GLfloat) ls[i].g/255.0, (GLfloat) ls[i].b/255.0 );
        glVertex2f((GLfloat) ls[i].x1/k_drawD*2, (GLfloat) ls[i].y1/k_drawD*2);
        glVertex2f((GLfloat) ls[i].x2/k_drawD*2, (GLfloat) ls[i].y2/k_drawD*2);
    }
    glEnd();
    return;
}

void gr_draw(GLFWwindow *window, int renderType){
    glfwMakeContextCurrent(window);
    float ratio;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    if(renderType == 0){
        glBegin(GL_QUADS);
        glColor3f( (GLfloat) 107/255.0, (GLfloat) 136/255.0, (GLfloat) 255/255.0);
        glVertex2f(-1.f, -1.f);
        glVertex2f(-1.f, 1.f);
        glVertex2f(1.f, 1.f);
        glVertex2f(1.f, -1.f);
        glEnd();
        gr_lines(lineArr);}
    else{gr_pixels(screen);}

    glfwSwapBuffers(window);
    glfwPollEvents();
}


void gr_init(){
    debug = true;
    screen = salloc(sizeof(unsigned char)*k_nPixels*3);
    lineArr = salloc(sizeof(line)*k_nMaxLinesPerObj * k_nMaxObj);
    s.world.scene = salloc(sizeof(int) * k_nMaxObj*3);
    for(int i=0;!((s.world.scene[i] = ob_levelTest[i]) == terminator && (i%3 == 0));i++);
    s.velY = 0;
    s.velX = 0;
    s.onGround = true;
    s.paused = false;
    s.world.camX = ob_levelTest[1];
    s.world.camY = ob_levelTest[2];
    s.world.camT = 0;
    s.camFlip = false;
    gr_update();
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

void gr_deinit(){
    free(screen);
    free(s.world.scene);
    glfwDestroyWindow(dimWindow);
    glfwDestroyWindow(perspWindow);
    glfwTerminate();
}

int main(void){
    ob_init();
    gr_init();
    while (!(glfwWindowShouldClose(dimWindow) || glfwWindowShouldClose(perspWindow)))
    //while (!(glfwWindowShouldClose(perspWindow)))
    {
        //sleep(1);
        if(!s.paused){
        gr_update();
        }
        gr_draw(perspWindow, 2);
        gr_draw(dimWindow, 0);
    }
    gr_deinit();
    ob_deinit();
    exit(EXIT_SUCCESS);
}
