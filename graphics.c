#include "graphics.h"
//TODO: FOV decreases when moving fast

void gr_keypress(int key, int scancode, int action, int mods){
    if ((key == GLFW_KEY_A || key == GLFW_KEY_D) && action == GLFW_PRESS){
        cam.flip = !cam.flip;
        //cam.T = pi - cam.T - cam.FOV;
    }
}

void gr_cursormove(double xPos, double yPos){
    cam.T = fmod(-yPos*0.000436, 2*pi);
    //if(!cam.flip){cam.T = fmod(-yPos*0.000436, 2*pi);}
    //else{cam.T = fmod(pi+yPos*0.000426-cam.FOV, 2*pi);}
}

void gr_update(){
    cam.animFrame++;
    cam.x = s.scene[s.pli].x+14;
    cam.y = s.scene[s.pli].y-2;
    cam.flashD = s.invincible && !s.star;
    cam.flashB = s.star;
    cam.flip = s.flip;
    rn_dimFcamera(dimScreen, cam);
    rn_perspFcamera(perspScreen, cam, NULL);
    if(s.scene[s.pli].vx < 1.6 || cam.flip){
        cam.FOV = k_FOV;
    }
    else{
        cam.FOV = (k_FOVrun - k_FOV)/(2.0-1.6)*(s.scene[s.pli].vx - 1.6)+k_FOV;
    }
}

void gr_char(char c, GLfloat* x, GLfloat* y){
    glPointSize(2.5f);
    glBegin(GL_POINTS);
    for(int i=0;i<fontSize;i++){
        if(font[(c-48)*fontSize+i]){
        glVertex2f(*x+i%7, *y-i/7);
        }
    }
    glEnd();
    *x += 8;
}

void gr_text(char *s, GLfloat x, GLfloat y){
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-k_drawD, k_drawD, -k_drawD, k_drawD, -1, 1);
    while(*s != '\0'){
        gr_char(*s, &x, &y);
        s++;
    }
    glPopMatrix();
}

void gr_pixel(int y, unsigned char r, unsigned char g, unsigned char b){
    glBegin(GL_QUADS);
    glColor3ub(r, g, b);
    glVertex2f(0, y);
    glVertex2f(0, y+1);
    glVertex2f(1, y+1);
    glVertex2f(1, y);
    glEnd();
}

void gr_pixels(unsigned char *renderArr){
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 1, 0, k_nPixels, -1, 1);

    for(int y=0;y<k_nPixels;y++){
        gr_pixel(y, renderArr[y*3+0], renderArr[y*3+1], renderArr[y*3+2]);
    }
    glPopMatrix();
}

void gr_points(point *ps){
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-cam.drawD+cam.x, cam.drawD+cam.x, cam.y-cam.drawD, cam.y+cam.drawD, -1, 1);

    glPointSize(360.0/k_drawD);
    glBegin(GL_POINTS);
    for(int i=0;;i++){
        if(ob_p_isTerm(ps[i])){break;}
        glColor3ub(ps[i].r, ps[i].g, ps[i].b);
        glVertex2f(ps[i].x, ps[i].y);
    }
    glEnd();
    glPopMatrix();
    return;
}

void gr_draw(GLFWwindow *window, int renderType){
    glClear(GL_COLOR_BUFFER_BIT);
    if(renderType == 0){
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glBegin(GL_QUADS);
        glColor3ub(107, 136, 255);
        glVertex2f(-1.f, -1.f);
        glVertex2f(-1.f, 1.f);
        glVertex2f(1.f, 1.f);
        glVertex2f(1.f, -1.f);
        glEnd();
        gr_points(dimScreen);
        glColor3f(1.0, 1.0, 1.0);
        char score[20];
        sprintf(score, "%d", s.coins);
        gr_text(score, -k_drawD+5, k_drawD-5);
        //MAR1D
    }
    else{gr_pixels(perspScreen);}
}

void gr_init(){
    cam.animFrame=0;
    cam.drawD = k_drawD;
    cam.FOV = k_FOV;
    cam.scene = s.scene;
    cam.T = 0;
    debug = false;
    perspScreen = salloc(sizeof(unsigned char)*k_nPixels*3);
    for(int i=0; i<k_nPixels*3; i++){perspScreen[i]=0;}
    dimScreen = salloc(sizeof(point)*500*k_nMaxObj);
    dimScreen[0] = p_termPoint;
    fontSize = io_getFont(&font, "mario.font");
}

void gr_deinit(){
    free(perspScreen);
    free(dimScreen);
    free(font);
}
