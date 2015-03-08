#include "graphics.h"

void gr_keypress(int key, int scancode, int action, int mods){
    if ((key == GLFW_KEY_A || key == GLFW_KEY_D) && action == GLFW_PRESS){
        cam.flip = !cam.flip;
    }
    if ((key == GLFW_KEY_ENTER) && action == GLFW_PRESS){
        if(s.menu == k_menuStatic){
            s.menu = k_menuTime;
        }
    }
}

void gr_cursormove(double xPos, double yPos){
    static double last = 0;
    double offset = yPos-last;
    last = yPos;
    if(!s.menu && !s.paused){
        cam.T = fmod(cam.T-offset*0.000436, 2*pi);
        if(cam.T < -pi/2){cam.T = -pi/2;}
        if(cam.T > pi/2-k_FOV){cam.T = pi/2-k_FOV;}
    }
}

void gr_update(){
    cam.animFrame++;
    for(int i=0; cam.scene[i].type != '\0'; i++){
        cam.scene[i].animFrame++;
    }
    //TODO: 3rd person deathcam
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
        if(font[(c)*fontSize+i]){
        glVertex2f(*x+(i%7)*k_fontSize, *y-(i/7)*k_fontSize);
        }
    }
    glEnd();
    *y -= 8*k_fontSize;
}

void gr_text(char *s, GLfloat x, GLfloat y){
    float y_orig = y;
    while(*s != '\0'){
        if(*s == '\n'){
            x+=8*k_fontSize;
            y=y_orig;
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

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

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
        glColor3ub(io_cs[ps[i].c].r, io_cs[ps[i].c].g, io_cs[ps[i].c].b);
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
    char gui[500];

    char score[7];
    sprintf(score, "%d", s.score);
    int i;
    int len = strlen(score);
    for(i=0;i<6-len;i++){
        score[i]='0';
    }
    sprintf(score+i, "%d", s.score);

    char coins[3];
    sprintf(coins, "%d", s.coins);
    len = strlen(coins);
    for(i=0;i<2-len;i++){
        coins[i]='0';
    }
    sprintf(coins+i, "%d", s.coins);

    char time[4];
    sprintf(time, "%d", s.time/k_timeTick);
    len = strlen(time);
    for(i=0;i<3-len;i++){
        time[i]='0';
    }
    sprintf(time+i, "%d", s.time/k_timeTick);
    sprintf(gui, "MAR1D          WORLD  TIME\n%s  @x%s    1-1    %s", score, coins, time);
    glColor3f(1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(0, k_guiWindowW, 0, k_guiWindowH, -1, 1);
    gr_text(gui, 5, k_guiWindowH-5);
}

void gr_drawMenu(){
    glEnable(GL_TEXTURE_2D);
    //glFrustum(-1, 1, -1, 1, -2, -1); //TODO: make this part perspective?
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    float rot = linInterp(0.0, 90.0, k_menuTime, 0.0, s.menu);
    glRotatef(rot, 0.f, 1.f, 0.f);

    glColor3f(1.f, 1.f, 1.f);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.f, 0.f); glVertex2f(-1.f, -1.f);
    glTexCoord2f(1.f, 0.f); glVertex2f(1.f, -1.f);
    glTexCoord2f(1.f, 1.f); glVertex2f(1.f, 1.f);
    glTexCoord2f(0.f, 1.f); glVertex2f(-1.f, 1.f);
    glEnd();
    glPopMatrix();
    if(s.menu != k_menuStatic){
        s.menu--;
    }
    glDisable(GL_TEXTURE_2D);
}

void gr_init(){
    cam.animFrame=0;
    cam.drawD = k_drawD;
    cam.FOV = k_FOV;
    cam.scene = s.scene;
    cam.T = k_camT;
    debug = false;
    perspScreen = salloc(sizeof(unsigned char)*k_nPixels*3);
    for(int i=0; i<k_nPixels*3; i++){perspScreen[i]=0;}
    dimScreen = salloc(sizeof(point)*500*k_nMaxObj);
    dimScreen[0] = p_termPoint;
    fontSize = io_getFont(&font, "mario.font");
    s.menu = k_menuStatic;

    image *im;
    im = loadTexture();

    wn_menuWindow();
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, im->sizeX, im->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, im->data);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}

void gr_deinit(){
    free(perspScreen);
    free(dimScreen);
    free(font);
}
