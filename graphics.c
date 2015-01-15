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

GLFWwindow* dimWindow;
GLFWwindow* perspWindow;
struct state s;
unsigned char *screen;
line *lineArr;

void gr_update(){
    if(s.forward && s.backward){;}
    else if(s.forward){cl_forward(&s.world);}
    else if(s.backward){cl_backward(&s.world);}
    //if(s.upward){cl_upward(&s.world); s.upward--;}
    //s.onGround = !cl_gravity(&s.world);
    s.velY -= .2;
    bool colis = !cl_go(&s.world, 'y', s.velY);
    if(colis == true && s.velY <= 0) {s.onGround = true; while(!cl_go(&s.world,'y', s.velY+=.1));}
    if(colis == true && s.velY > 0) {while(!cl_go(&s.world,'y', s.velY-=.1));}
    else if((int) s.velY != 0){s.onGround = false;}

    rn_dimFworld(&lineArr, s.world);
    rn_perspFworld_v(screen, s.world, &lineArr);
}

static void error_callback(int error, const char* description)
{
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
    s.world.camT = -yPos/80.0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_W && action == GLFW_PRESS){
        s.forward = true;
    }
    if (key == GLFW_KEY_W && action == GLFW_RELEASE){
        s.forward = false;
    }

    if (key == GLFW_KEY_S && action == GLFW_PRESS){
        s.backward = true;
    }
    if (key == GLFW_KEY_S && action == GLFW_RELEASE){
        s.backward = false;
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
        if(s.onGround) {s.upward = 40;}
        if(s.onGround) {s.velY = 7;}
        s.onGround = false;
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE){
        s.upward -= 20;
        s.velY -= 0;
        if(s.upward<0){s.upward = 0;}
    }
    if (key == GLFW_KEY_Q && action == GLFW_PRESS){
        s.paused = !s.paused;
    }

    if (key == GLFW_KEY_N && action == GLFW_PRESS){
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        int w,h;
        glfwGetFramebufferSize(window, &w, &h);
        int i=0;
        do{i+=3;}while(s.world.scene[i]!=terminator);
        s.world.scene[i] = objGround;
        s.world.scene[i+1] = (int)((x-w/2)/w*k_drawD);
        s.world.scene[i+2] = (int)((h/2-y)/h*k_drawD);
        s.world.scene[i+3] = terminator;
    }

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

static void gr_draw(GLFWwindow *window, int renderType){
    glfwMakeContextCurrent(window);
    float ratio;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    if(renderType == 0){
        gr_lines(lineArr);}
    else{gr_pixels(screen);}

    glfwSwapBuffers(window);
    glfwPollEvents();
}


void gr_init(){
    screen = salloc(sizeof(unsigned char)*k_nPixels*3);
    lineArr = salloc(sizeof(line)*k_nMaxLinesPerObj * k_nMaxObj);
    s.world.scene = salloc(sizeof(int) * k_nMaxObj*3);
    for(int i=0;!((s.world.scene[i] = ob_levelTest[i]) == terminator && (i%3 == 0));i++){}
    s.forward = false;
    s.backward = false;
    s.onGround = true;
    s.upward = 0;
    s.paused = false;
    s.world.camX = ob_levelTest[1];
    s.world.camY = ob_levelTest[2];
    s.world.camT = 0;
    gr_update();
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    dimWindow = glfwCreateWindow(800, 800, "2d view. For debugging only.", NULL, NULL);
    perspWindow = glfwCreateWindow(20, 800, "Perspective view", NULL, NULL);
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
