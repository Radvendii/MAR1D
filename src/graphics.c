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
  static double last = 0; // Stores the last cursor position so we can calculate offset.
  double offset = yPos-last;
  last = yPos;
  if(!s.menu && !s.paused){
    cam.T = fmod(cam.T-offset*camSpeed, 2*pi);
    // Bound the camera angle by straight down and straight up. 
    // Don't want people spinning the camera 360.
    if(cam.T < -pi/2 - k_FOV/2){cam.T = -pi/2 - k_FOV/2;}
    if(cam.T > pi/2-k_FOV/2){cam.T = pi/2-k_FOV/2;}
  }
}

void gr_update(){
  if(!s.paused){
    // Background color based on level.
    if(s.loc == '1'){ //TODO: make this not a kludge. 
                      //      The background color of the level should be stored in the level data. Or maybe just "underground" and "aboveground"?
      cam.bg.r = k_bgr;
      cam.bg.g = k_bgg;
      cam.bg.b = k_bgb;
    }
    else{
      cam.bg.r = 0;
      cam.bg.g = 0;
      cam.bg.b = 0;
    }
    cam.animFrame++;
    for(int i=0; cam.scene[i].type != '\0'; i++){
      cam.scene[i].animFrame++; //TODO: Does each object really need it's own animation counter?
    }

    // Set camera effects
    cam.flashD = s.invincible && !s.star;
    cam.flashB = s.star;
    cam.redTint = s.fire;
    cam.flip = s.flip;

    //Warp FOV when the player is running
    if(s.runWarp < k_tBeforeFOVChange || cam.flip){
      cam.FOV = k_FOV;
    }
    else{
      cam.FOV = (k_FOVrun - k_FOV)/(k_durationFOVChange)*(s.runWarp - k_tBeforeFOVChange)+k_FOV;
    }
    if(s.runWarp>k_tBeforeFOVChange+k_durationFOVChange){s.runWarp = k_tBeforeFOVChange+k_durationFOVChange;}

  }
  //TODO: 3rd person deathcam?
  // Position camera in player
  cam.x = s.scene[s.pli].x+14;
  cam.y = s.scene[s.pli].y-2;

  // Render
  rn_dimFcamera(dimScreen, cam);
  rn_perspFcamera(perspScreen, cam);
}

// Render a text character
void gr_char(bool vert, char c, GLfloat* x, GLfloat* y){
  glPointSize(k_fontSize);
  glBegin(GL_POINTS);
  for(int i=0;i<fontSize;i++){
    if(font[(c)*fontSize+i]){
      //TODO: This hard-codes the dimensions of the font. Fix that.
      glVertex2f(*x+(i%7)*k_fontSize, *y-(i/7)*k_fontSize);
    }
  }
  glEnd();
  // Shift over for next character
  if(vert){*y -= 8*k_fontSize;}
  else{*x += 8*k_fontSize;}
}

// Renders a text string
void gr_text(bool vert, char *s, GLfloat x, GLfloat y){
  float x_orig = x;
  float y_orig = y;
  for(;*s != '\0';s++){
    if(*s == '\n'){
      if(vert){
        x+=8*k_fontSize;
        y=y_orig;
      }
      else{
        y-=8*k_fontSize;
        x=x_orig;
      }
    }
    else{
      gr_char(vert, *s, &x, &y);
    }
  }
}

// Render a "pixel" of the vertical screen
void gr_pixel(int y, unsigned char r, unsigned char g, unsigned char b){
  glBegin(GL_QUADS);
  glColor3ub(r, g, b);
  glVertex2f(-lineSize, y);
  glVertex2f(-lineSize, y+1);
  glVertex2f(lineSize, y+1);
  glVertex2f(lineSize, y);
  glEnd();
}

// Render an array of pixels
void gr_pixels(unsigned char *renderArr){
  if(debug) {
    // Write output to the image file so we can make an image timelapse of gameplay
    FILE *imageFile = sfopen("mar1d_image.raw", "wb");
    fwrite(renderArr, sizeof(unsigned char), k_nPixels * 3, imageFile);
    sfclose(imageFile);
    imageWidth++;
  }

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(-k_perspWindowW, k_perspWindowW, 0, k_nPixels, -1, 1);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  for(int y=0;y<k_nPixels;y++){
    gr_pixel(y, renderArr[y*3+0], renderArr[y*3+1], renderArr[y*3+2]);
  }
  //TODO: What is up with these matrices? I feel like I'm using them all wrong
  glPopMatrix();
}

void gr_points(point *ps){
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(-cam.drawD+cam.x, cam.drawD+cam.x, cam.y-cam.drawD, cam.y+cam.drawD, -1, 1);

  glPointSize(360.0/k_drawD2);
  glBegin(GL_POINTS);
  for(int i=0; !ob_p_isTerm(ps[i]); i++){
    glColor3ub(io_cs[ps[i].c].r, io_cs[ps[i].c].g, io_cs[ps[i].c].b);
    glVertex2f(ps[i].x, ps[i].y);
  }
  glEnd();
  glPopMatrix();
  return;
}

void gr_drawPersp(){
  gr_pixels(perspScreen);
  if(s.userPaused){
    glColor3f(0.5, 0.5, 0.5);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(-k_perspWindowW/2, k_perspWindowW/2, -k_perspWindowH/2, k_perspWindowH/2, -1, 1);
    gr_text(false, "PAUSED", -3*7*k_fontSize, 0);
  }
}

void gr_drawDim(){
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Background
  glBegin(GL_QUADS);
  // glColor3ub(cam.bg.r, cam.bg.g, cam.bg.b);
  glColor3f(1.0, 1.0, 1.0);
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

void gr_drawHud(){
  char hud[60];
  // Formatting of this is very finicky
  sprintf(hud, "   MAR1D          WORLD  TIME\n   %06d  @x%02d    1-1   %03d", s.score, s.coins, s.time/k_timeTick);
  glColor3f(0.5, 0.5, 0.5);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glOrtho(0, k_hudWindowW, 0, k_hudWindowH, -1, 1);

  gr_text(true, hud, 5, k_hudWindowH-5);
}

void gr_drawMenu(){
  glColor3f(1.f, 1.f, 1.f);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  if(s.menu > k_menuStatic){
    char hud[100];
    sprintf(hud, "CONGRATULATIONS!\n\nYOUVE WON LEVEL 1-1!\n\nSCORE %06d", s.score);
    glOrtho(-k_menuWindowW/2, k_menuWindowW/2, 0, k_menuWindowH, -1, 1);
    gr_text(false, hud, -k_menuWindowW/2+20, k_menuWindowH-20);
    s.menu--;
  }
  else{ // The beginning menu

    // Rotate the image when the user starts the game
    glPushMatrix();
    float rot = linInterp(0.0, 90.0, k_menuTime, 0.0, s.menu);
    glRotatef(rot, 0.f, 1.f, 0.f);

    glEnable(GL_TEXTURE_2D);
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
      if(!s.menu){ // End of menu animation
        gl_load();
      }
    }
    glDisable(GL_TEXTURE_2D);
  }
}

void gr_init(int _lineSize, int _sensitivity){
  lineSize = _lineSize; // Width of virtual screen
  camSpeed = _sensitivity / 10000.0;

  cam.animFrame=0;
  cam.drawD = k_drawD;
  cam.FOV = k_FOV;
  cam.scene = s.scene;
  cam.T = k_camT;
  debug = false;
  imageWidth = 0; // Width of resulting timelapse image
  perspScreen = salloc(sizeof(unsigned char)*k_nPixels*3);
  for(int i=0;i<k_nPixels*3;i++){perspScreen[i]=0;} // Initialize to zero
  dimScreen = salloc(sizeof(point)*500*k_nMaxObj);
  dimScreen[0] = p_termPoint;
  fontSize = io_getFont(&font, "mario.font");
  s.menu = k_menuStatic;

  image *im;
  im = loadTexture(); // From parsing.c

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
  if(debug){ // Rename image out so it's clear how big it is.
    char imageFile[100];
    sprintf(imageFile, "mar1d_%dx%d.raw", k_nPixels, imageWidth);
    rename("mar1d_image.raw", imageFile);
  }
}
