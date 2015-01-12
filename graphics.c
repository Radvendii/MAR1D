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
int renderType;

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    /*
     *if (key == GLFW_KEY_W && action == GLFW_PRESS
     */
}

void gr_init(){
    screen = salloc(sizeof(unsigned char)*k_nPixels*3);
    world.scene = salloc(sizeof(int) * k_nMaxObj*3);
    for(int i=0;!((world.scene[i] = ob_levelTest[i]) == terminator && (i%3 == 0));i++){}
    world.camX = -20;
    world.camY = 20;
    world.camT = -20;
    int h, w;
    renderType = 0; //0 for 2d, 1 for ortho, 2 for persp
    if(renderType == 0){h=800;w=800;
        lineArr = salloc(sizeof(line)*k_nMaxLinesPerObj * k_nMaxObj);
        nLines = rn_dimTest(&lineArr);
        lineArr[nLines] = (line) {.x1 = -k_screenX, .x2 = k_screenX, .y1=0, .y2=0, .r=255, .g=255, .b=255};
        lineArr[nLines+1] = (line) {.y1 = -k_screenX, .y2 = k_screenX, .x1=0, .x2=0, .r=255, .g=255, .b=255};
        nLines += 2;
    }
    else if(renderType == 1){h=800;w=20;rn_orthoTest(screen);}
    else if(renderType == 2){h=800;w=20;rn_screenFworld(screen, world);}
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
    glfwSetKeyCallback(window, key_callback);
}

void gr_deinit(){
    free(screen);
    free(world.scene);
    glfwDestroyWindow(window);
    glfwTerminate();
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
        glVertex2f((GLfloat) ls[i].x1/k_screenX*2, (GLfloat) ls[i].y1/k_screenX*2);
        glVertex2f((GLfloat) ls[i].x2/k_screenX*2, (GLfloat) ls[i].y2/k_screenX*2);
    }
    glEnd();
    return;
}

/*
 *void gr_lines(){
 *    line l;
 *    glBegin(GL_LINES);
 *    for(int obj=0;;obj++) {
 *        if(ob_levelTest[obj*3] == terminator) {break;} //Check for termination
 *        for(int line=0;;line++) {
 *            if(ob_isTerminating(&(objFtype(ob_levelTest[obj*3])[line]))){break;}
 *            l = objFtype(ob_levelTest[obj*3])[line];
 *            realifyLine(&l, (ob_levelTest + obj*3 +1));
 *            glColor3f( (GLfloat) l.r/255.0, (GLfloat) l.g/255.0, (GLfloat) l.b/255.0 );
 *            glVertex2f((GLfloat) l.x1/k_screenX*2, (GLfloat) l.y1/k_screenX*2);
 *            glVertex2f((GLfloat) l.x2/k_screenX*2, (GLfloat) l.y2/k_screenX*2);
 *
 *        }
 *    }
 *    glColor3f( 1.f, 1.f, 1.f);
 *    glVertex2f( -1.f, 0.f );
 *    glVertex2f( 1.f, 0.f );
 *    glVertex2f( 0.f, -1.f );
 *    glVertex2f( 0.f, 1.f );
 *    glEnd();
 *    return;
 *}
 */


int main(void){
    ob_init();
    gr_init();
    printf("%d\n", mh_isCollision(world, 0, 3));
    while (!glfwWindowShouldClose(window))
    {
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
    gr_deinit();
    ob_deinit();
    exit(EXIT_SUCCESS);
}
