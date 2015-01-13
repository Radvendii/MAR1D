#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "objects.h"
#include "rendering.h"
#include "helpers.h"
#include "mechanics.h"
#include "controls.h"

GLFWwindow* window;
unsigned char *screen;
line *lineArr;
int nLines;
struct world world;
int numObjs;
int renderType;

void gr_update(){
    if(renderType==0){
        nLines = rn_dimFworld(&lineArr, world);
        lineArr[nLines] = (line) {.x1 = -k_drawD, .x2 = k_drawD, .y1=0, .y2=0, .r=255, .g=255, .b=255};
        lineArr[nLines+1] = (line) {.y1 = -k_drawD, .y2 = k_drawD, .x1=0, .x2=0, .r=255, .g=255, .b=255};
        nLines += 2;
    }
    else if(renderType == 1){rn_orthoTest(screen);}
    else if(renderType == 2){rn_perspFworld(screen, world);}
}

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

void gr_listcollisions(){
    for(int i1=0;i1<numObjs;i1++){
        for(int i2=0;i2<numObjs;i2++){
            printf("%d collides with %d: %d\n", i1, i2, mh_isCollision(world, i1*3, i2*3));
        }
        printf("\n");
    }
    printf("\n");
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_N && action == GLFW_PRESS){
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        int w,h;
        glfwGetFramebufferSize(window, &w, &h);
        world.scene[numObjs*3+0] = objGround;
        world.scene[numObjs*3+1] = (int)((x-w/2)/w*k_drawD);
        world.scene[numObjs*3+2] = (int)((w/2-y)/h*k_drawD);
        world.scene[numObjs*3+3] = terminator;
        numObjs++;
        gr_update();
        gr_listcollisions();
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

void gr_lines(line *ls, int nLines){
    glBegin(GL_LINES);
    for(int i=0;i<nLines;i++){
        glColor3f( (GLfloat) ls[i].r/255.0, (GLfloat) ls[i].g/255.0, (GLfloat) ls[i].b/255.0 );
        glVertex2f((GLfloat) ls[i].x1/k_drawD*2, (GLfloat) ls[i].y1/k_drawD*2);
        glVertex2f((GLfloat) ls[i].x2/k_drawD*2, (GLfloat) ls[i].y2/k_drawD*2);
    }
    glEnd();
    return;
}

static void gr_draw(GLFWwindow *window){
    float ratio;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    if(renderType == 0){
        gr_lines(lineArr, nLines);}
    else{gr_pixels(screen);}

    glfwSwapBuffers(window);
    glfwPollEvents();
}


void gr_init(){
    screen = salloc(sizeof(unsigned char)*k_nPixels*3);
    world.scene = salloc(sizeof(int) * k_nMaxObj*3);
    for(int i=0;!((world.scene[i] = ob_levelTest[i]) == terminator && (i%3 == 0) && (numObjs = i/3));i++){}
    world.camX = -20;
    world.camY = 20;
    world.camT = -20;
    int h, w;
    renderType = 0; //0 for 2d, 1 for ortho, 2 for persp
    if(renderType == 0){h=800;w=800;
        lineArr = salloc(sizeof(line)*k_nMaxLinesPerObj * k_nMaxObj);
    }
    else if(renderType == 1){h=800;w=20;}
    else if(renderType == 2){h=800;w=20;}
    gr_update();
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(w, h, "This is Epic", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowRefreshCallback(window, gr_draw);
    glfwSetKeyCallback(window, key_callback);
}

void gr_deinit(){
    free(screen);
    free(world.scene);
    glfwDestroyWindow(window);
    glfwTerminate();
}

int main(void){
    ob_init();
    gr_init();
    while (!glfwWindowShouldClose(window))
    {
        gr_draw(window);
    }
    gr_deinit();
    ob_deinit();
    exit(EXIT_SUCCESS);
}
