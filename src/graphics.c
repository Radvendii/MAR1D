#include "graphics.h"

int lineSize;
bool *font;
int fontSize; // length of the array containing a single character

struct camera cam;
// TODO: perhaps this should be an array of `color`s instead?
//       color[k_nPixels] perspScreen
unsigned char *perspScreen;
point *dimScreen;

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
  if (conf.debug) {
    rn_dimFcamera(dimScreen, cam);
  }
  rn_perspFcamera(perspScreen, cam);

  if (!s.won) { // ignore the long static view of the flag
    io_recAddFrame(perspScreen);
  }
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
  gr_drawRect(RGB_F, RECT_LTRB(l, t + s, r, t));
  //left edge
  gr_drawRect(RGB_F, RECT_LTRB(l - s, t, l, b));
  //bottom edge
  gr_drawRect(RGB_0, RECT_LTRB(l, b, r, b - s));
  //right edge
  gr_drawRect(RGB_0, RECT_LTRB(r, t, r + s, b));
}

void gr_drawBezelIn(rect rct) {
  float l = rct.l;
  float t = rct.t;
  float r = rct.r;
  float b = rct.b;
  float s = k_bezelSize;

  //top edge
  gr_drawRect(RGB_0, RECT_LTRB(l, t + s, r, t));
  //left edge
  gr_drawRect(RGB_0, RECT_LTRB(l - s, t, l, b));
  //bottom edge
  gr_drawRect(RGB_F, RECT_LTRB(l, b, r, b - s));
  //right edge
  gr_drawRect(RGB_F, RECT_LTRB(r, t, r + s, b));
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
void gr_text(color col, bool vert, const char *s, float size, GLfloat x_orig, GLfloat y_orig){
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
    gr_text(RGB_textDim, false, "PAUSED", k_fontSize, -3*k_fontSpaceX(false)*k_fontSize, 0);
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

  // draw game points
  gr_points(dimScreen);

  // draw 2D textures
  // TODO: this looks ugly here
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(-cam.drawD+cam.x, cam.drawD+cam.x, cam.y-cam.drawD, cam.y+cam.drawD, -1, 1);
  for(int obj = 0; cam.scene[obj].type != '\0'; obj++) {
    image *im = &cam.scene[obj].frames[cam.scene[obj].animFrame / k_animFreq % cam.scene[obj].nFrames].im;
    gr_image(im, RECT_LTWH(cam.scene[obj].x + cam.scene[obj].bb.x, cam.scene[obj].y + cam.scene[obj].bb.y, cam.scene[obj].bb.w, -cam.scene[obj].bb.h));
  }
  glPopMatrix();
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
  if(s.time/k_gameTicksPerTimeTick > 999) { // make sure time can fit in the format specifier
    DEBUG("Too much time left. Can't fit in the HUD.");
    exit(EXIT_FAILURE);
  }
  // Formatting of this is very finicky
  sprintf(hud, "   MAR1D          WORLD  TIME\n"
               "   %06d  @x%02d    1-1   %03u",
          s.score, s.coins, s.time/k_gameTicksPerTimeTick);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glOrtho(0, k_hudWindowW, 0, k_hudWindowH, -1, 1);

  gr_text(RGB_textDim, true, hud, k_fontSize, 5, k_hudWindowH-5);

  // bold the time left if they're running out
  // flash bold during SND_lowtime
  if (s.lowTime && !(au_playing(SND_lowtime) && (s.time/k_gameTicksPerTimeTick)%2==0)) {
    char time[32];
    sprintf(time, "TIME\n"
                  "%03u",
            s.time/k_gameTicksPerTimeTick);
    // TODO: this could be cleaner if gr_text returned the new x, y coords
    gr_text(RGB_textMed, true, time, k_fontSize, 5, k_hudWindowH-5 - k_fontSpaceY(true) * k_fontSize * strlen("   MAR1D          WORLD  "));
  }
}

void gr_init(){
  cam.animFrame=0;
  cam.drawD = k_drawD;
  cam.FOV = k_FOV;
  cam.scene = s.scene;
  cam.T = k_camT;
  perspScreen = salloc(sizeof(unsigned char)*k_nPixels*3);
  for(int i=0;i<k_nPixels*3;i++){perspScreen[i]=0;} // Initialize to zero // TODO: memset
  dimScreen = salloc(sizeof(point)*500*k_nMaxObj); // TODO: magic number >:(
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, im->sizeX, im->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, im->data);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  }

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0.5);

  glBindTexture(GL_TEXTURE_2D, im->texture);
  glBegin(GL_QUADS);
  glTexCoord2f(0.f, 1.f); glVertex2f(r.l, r.b);
  glTexCoord2f(1.f, 1.f); glVertex2f(r.r, r.b);
  glTexCoord2f(1.f, 0.f); glVertex2f(r.r, r.t);
  glTexCoord2f(0.f, 0.f); glVertex2f(r.l, r.t);
  glEnd();

  glDisable(GL_TEXTURE_2D);
  glDisable(GL_ALPHA_TEST);
}

void gr_unbindImage(image *im) {
  if (im->texture) {
    glDeleteTextures(1, &im->texture);
    im->texture = 0;
  }
}

void gr_deinit(){
  free(perspScreen);
  free(dimScreen);
  free(font);
}
