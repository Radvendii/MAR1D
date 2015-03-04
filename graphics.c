#include "graphics.h"

void gr_keypress(int key, int scancode, int action, int mods){
    if ((key == GLFW_KEY_A || key == GLFW_KEY_D) && action == GLFW_PRESS){
        cam.flip = !cam.flip;
    }
}

void gr_cursormove(double xPos, double yPos){
    cam.T = fmod(-yPos*0.000436, 2*pi);
}

void gr_update(){
    cam.animFrame++;
    for(int i=0; cam.scene[i].type != '\0'; i++){
        cam.scene[i].animFrame++;
    }
    cam.x = s.scene[s.pli].x+14;
    cam.y = s.scene[s.pli].y-2;
    cam.flashD = s.invincible && !s.star;
    cam.flashB = s.star;
    cam.redTint = s.fire;
    cam.flip = s.flip;

    if(s.run < k_tBeforeFOVChange || cam.flip){
        cam.FOV = k_FOV;
    }
    else{
        cam.FOV = (k_FOVrun - k_FOV)/(k_durationFOVChange)*(s.run - k_tBeforeFOVChange)+k_FOV;
    }
    if(s.run>k_tBeforeFOVChange+k_durationFOVChange){s.run = k_tBeforeFOVChange+k_durationFOVChange;}

    rn_dimFcamera(dimScreen, cam);
    rn_perspFcamera(perspScreen, cam, NULL);
}

void gr_char(char c, GLfloat* x, GLfloat* y){
    glPointSize(k_fontSize);
    glBegin(GL_POINTS);
    for(int i=0;i<fontSize;i++){
        if(font[(c-48)*fontSize+i]){
        glVertex2f(*x+(i%7)*k_fontSize, *y-(i/7)*k_fontSize);
        }
    }
    glEnd();
    *x += 8*k_fontSize;
}

void gr_text(char *s, GLfloat x, GLfloat y){
    float x_orig = x;
    while(*s != '\0'){
        if(*s == '\n'){
            y-=8*k_fontSize;
            x=x_orig;
        }
        else{
            gr_char(*s, &x, &y);
        }
        s++;
    }
}

void gr_pixel(int y, unsigned char r, unsigned char g, unsigned char b){
    glBegin(GL_QUADS);
    glColor3ub(r, g, b);
    glVertex2f(-k_lineSize, y);
    glVertex2f(-k_lineSize, y+1);
    glVertex2f(k_lineSize, y+1);
    glVertex2f(k_lineSize, y);
    glEnd();
}

void gr_pixels(unsigned char *renderArr){
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-k_perspWindowW, k_perspWindowW, 0, k_nPixels, -1, 1);

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

void gr_drawPersp(){
    gr_pixels(perspScreen);
}

void gr_drawDim(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_QUADS);
    glColor3ub(k_bgr, k_bgg, k_bgb);
    glVertex2f(-1.f, -1.f);
    glVertex2f(-1.f, 1.f);
    glVertex2f(1.f, 1.f);
    glVertex2f(1.f, -1.f);
    glEnd();
    gr_points(dimScreen);
}

void gr_clear(){
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void gr_drawGui(){
    char score[20];
    sprintf(score, "MAR1D\n%d", s.coins);
    glColor3f(1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, k_guiWindowW, 0, k_guiWindowH, -1, 1);
    gr_text(score, 5, k_guiWindowH-5);
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
