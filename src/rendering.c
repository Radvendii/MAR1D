#include "rendering.h"

// Constructs a two-dimensional view of the world, like the standard mario game
void rn_dimFcamera(point *points, struct camera c){
  point p;
  int nPoints=0;

  // Shows where the camera is pointing in the debug window
  // NOTE: doesn't account for camera flipping
  int camD = 10;
  double a = camD/cos(c.FOV/2);
  double xa = c.x + a*cos(c.T);
  double ya = c.y + a*sin(c.T);
  double xb = c.x + a*cos(c.T+c.FOV);
  double yb = c.y + a*sin(c.T+c.FOV);
  points[nPoints++] = (point) {.x = c.x, .y = c.y, .c = '.'};
  points[nPoints++] = (point) {.x = xa, .y = ya, .c = '.'};
  points[nPoints++] = (point) {.x = xb, .y = yb, .c = '.'};

  for(int obj = 0; c.scene[obj].type != '\0'; obj++) {
    for(point* pp = c.scene[obj].ps[c.scene[obj].animFrame / k_animFreq % c.scene[obj].nps]; !ob_p_isTerm(p = *pp); pp++) {
      ob_realifyPoint(&p, c.scene[obj].x, c.scene[obj].y);
      points[nPoints++] = p;
    }
  }
  points[nPoints] = p_termPoint;
  return;
}

//Constructs a perspective view of the game
void rn_perspFcamera(unsigned char *screen, struct camera c){
  // Copy background color to every pixel
  for(int i=0;i<k_nPixels;i++){
    screen[i*3]=c.bg.r;
    screen[i*3+1]=c.bg.g;
    screen[i*3+2]=c.bg.b;
  }
  // Depth buffer; initialize to draw distance
  float ds[k_nPixels];
  for(int i=0; i<k_nPixels; i++){
    ds[i] = c.drawD;
  }
  float d;
  int D;
  int y;
  point p;
  for(int obj = 0; c.scene[obj].type != '\0'; obj++) {
    if(c.scene[obj].hidden == true || c.scene[obj].type == '.'){
      continue;
    }
    for(point* pp = c.scene[obj].ps[c.scene[obj].animFrame / k_animFreq % c.scene[obj].nps]; !ob_p_isTerm(p = *pp); pp++) {
      if(ob_p_isSkip(p)){
        continue;
      }
      if(c.scene[obj].flip){
        p.x = c.scene[obj].bb.w - p.x;
      }
      ob_realifyPoint(&p, c.scene[obj].x, c.scene[obj].y);
      p.x -= c.x;
      p.y -= c.y;

      // Alpha is the angle from bottom of camera to p
      double alpha = atan2(p.y, p.x);
      if(c.flip){alpha = pi-alpha;}
      alpha = fmod(alpha - c.T, 2*pi);
      if(alpha > pi){alpha -= 2*pi;}

      if(alpha < -0.1 || alpha > c.FOV+0.1){ // Outside FOV.
        continue;
      }

      y = (tan(alpha-c.FOV/2) + tan(c.FOV/2) ) / (2*tan(c.FOV/2)) * k_nPixels;
      d = sqrt(p.x*p.x + p.y*p.y);
      D = ceil(1/(2*d*tan(c.FOV/2))*k_nPixels);
      for(int i = y - D; i < y + D; i++){
        if(i < 0 || i >= k_nPixels){
          continue;
        }
        if(d < ds[i] && abs(p.x) < k_drawD2){
          ds[i] = d;
          screen[i*3+0] = io_cs[p.c].r;
          screen[i*3+1] = io_cs[p.c].g;
          screen[i*3+2] = io_cs[p.c].b;
          if(p.x > k_drawD1){ // Fade to background
            screen[i*3+0] = (c.bg.r - screen[i*3+0])/(k_drawD2 - k_drawD1)*(p.x-k_drawD1) + screen[i*3+0];
            screen[i*3+1] = (c.bg.g - screen[i*3+1])/(k_drawD2 - k_drawD1)*(p.x-k_drawD1) + screen[i*3+1];
            screen[i*3+2] = (c.bg.b - screen[i*3+2])/(k_drawD2 - k_drawD1)*(p.x-k_drawD1) + screen[i*3+2];
          }
        }
      }
    }
  }

  // Apply special effects
  for(int i=0;i<k_nPixels;i++){
    if(c.flashD && c.animFrame/16 % 2){
      screen[i*3+0] /= 2;
      screen[i*3+1] /= 2;
      screen[i*3+2] /= 2;
    }
    if(c.flashB && c.animFrame/16 % 2){
      screen[i*3+0] = (screen[i*3+0]+255)/2;
      screen[i*3+1] = (screen[i*3+1]+255)/2;
      screen[i*3+2] = (screen[i*3+2]+255)/2;
    }
    if(c.redTint){
      screen[i*3+0] = (2*screen[i*3+0]+255)/3;
      screen[i*3+1] = (2*screen[i*3+1]+0)/3;
      screen[i*3+2] = (2*screen[i*3+2]+0)/3;
    }
  }
  return;
}
