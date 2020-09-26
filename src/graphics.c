#include "graphics.h"

void gr_keypress(int key, int state, int mods){
  if (KEY_DN(turn) && state == SDL_PRESSED){
    cam.flip = !cam.flip;
  }
}

void gr_mousemove(double xMove, double yMove){
  if(!s.paused){
    cam.T = fmod(cam.T-yMove*k_camSpeed, 2*pi);
    // Bound the camera angle by straight down and straight up. 
    // Don't want people spinning the camera 360.
    BOUND(cam.T, -pi/2 - k_FOV/2, pi/2 - k_FOV/2);
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

void gr_color(color c) {
  glColor3ub(c.r, c.g, c.b);
}

void gr_drawRect(color c, rect r) {
  glBegin(GL_QUADS);
  gr_color(c);
  glVertex2f(r.l, r.t);
  glVertex2f(r.r, r.t);
  glVertex2f(r.r, r.b);
  glVertex2f(r.l, r.b);
  glEnd();
}

void gr_drawBezelOut(rect rct) {
  float l = rct.l;
  float t = rct.t;
  float r = rct.r;
  float b = rct.b;
  float s = k_bezelSize;

  //top edge
  gr_drawRect(RGB(0xffffff), RECT_LTRB(l, t + s, r, t));
  //left edge
  gr_drawRect(RGB(0xffffff), RECT_LTRB(l - s, t, l, b));
  //bottom edge
  gr_drawRect(RGB(0x000000), RECT_LTRB(l, b, r, b - s));
  //right edge
  gr_drawRect(RGB(0x000000), RECT_LTRB(r, t, r + s, b));
}

void gr_drawBezelIn(rect rct) {
  #define k_bezelSize 1
  float l = rct.l;
  float t = rct.t;
  float r = rct.r;
  float b = rct.b;
  float s = k_bezelSize;

  //top edge
  gr_drawRect(RGB(0x000000), RECT_LTRB(l, t + s, r, t));
  //left edge
  gr_drawRect(RGB(0x000000), RECT_LTRB(l - s, t, l, b));
  //bottom edge
  gr_drawRect(RGB(0xffffff), RECT_LTRB(l, b, r, b - s));
  //right edge
  gr_drawRect(RGB(0xffffff), RECT_LTRB(r, t, r + s, b));
}

// Render a text character
void gr_char(char c, float size, GLfloat x, GLfloat y){
  glPointSize(k_fontSize);
  glBegin(GL_POINTS);
  for(int i=0;i<fontSize;i++){
    if(font[toupper(c)*fontSize+i]){
      glVertex2f(x + (i % k_fontW) * size, y - (i / k_fontH) * size);
    }
  }
  glEnd();
}

// Renders a text string
void gr_text(color col, bool vert, char *s, float size, GLfloat x_orig, GLfloat y_orig){
  gr_color(col);
  GLfloat x = x_orig;
  GLfloat y = y_orig;
  for(int i=0; s[i] != '\0'; i++){
    if(s[i] == '\n'){
      if(vert){
        x += k_fontSpaceX(vert) * size;
        y = y_orig;
      }
      else{
        y -= k_fontSpaceY(vert) * size;
        x = x_orig;
      }
    }
    else{
      gr_char(s[i], size, x, y);

      // Shift over for next character
      if(vert){
        y -= k_fontSpaceY(vert) * size;
      }
      else{
        x += k_fontSpaceX(vert) * size;
      }
    }
  }
}

// Render a "pixel" of the vertical screen
void gr_pixel(int y, unsigned char r, unsigned char g, unsigned char b){
  gr_drawRect((color) {.r = r, .g = g, .b = b}, RECT_LTRB(-conf.lineSize, y+1, conf.lineSize, y));
}

// Render an array of pixels
void gr_pixels(unsigned char *renderArr){
  if(debug) {
    // Write output to the image file so we can make an image timelapse of gameplay
    // TODO: write output to internal buffer so we're not saving a partial file?
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
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(-k_perspWindowW/2, k_perspWindowW/2, -k_perspWindowH/2, k_perspWindowH/2, -1, 1);
    gr_text(k_colorTextDim, false, "PAUSED", k_fontSize, -3*k_fontSpaceX(false)*k_fontSize, 0);
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

void gr_menuWindowMatrix() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glOrtho(0, k_menuWindowW, 0, k_menuWindowH, -1, 1);
}

void gr_drawHud(){
  char hud[60];
  // Formatting of this is very finicky
  sprintf(hud, "   MAR1D          WORLD  TIME\n   %06d  @x%02d    1-1   %03d", s.score, s.coins, s.time/k_timeTick);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glOrtho(0, k_hudWindowW, 0, k_hudWindowH, -1, 1);

  gr_text(k_colorTextDim, true, hud, k_fontSize, 5, k_hudWindowH-5);
}

void gr_init(){
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
}

void gr_image(image *im, rect r) {
  /*
   * if this image has not been loaded into a texture yet, do so now
   * this method only makes sense if the total number of images used by the
   * program is small. otherwise, textures may need to be unloaded
   */
  if (!im->texture) {
    glGenTextures(1, &im->texture);
    glBindTexture(GL_TEXTURE_2D, im->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, im->sizeX, im->sizeY, 0, GL_BGR, GL_UNSIGNED_BYTE, im->data);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  }

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, im->texture);
  glBegin(GL_QUADS);
  glTexCoord2f(0.f, 1.f); glVertex2f(r.l, r.t);
  glTexCoord2f(1.f, 1.f); glVertex2f(r.r, r.t);
  glTexCoord2f(1.f, 0.f); glVertex2f(r.r, r.b);
  glTexCoord2f(0.f, 0.f); glVertex2f(r.l, r.b);
  glEnd();
  glDisable(GL_TEXTURE_2D);
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
