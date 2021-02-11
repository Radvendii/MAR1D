#include "controls.h"

void cl_init(){
  s.backward = false;
  s.forward = false;
  s.upcount = 0;
  s.run = false;
  s.runWarp = false;
  s.multibounce=0;
}

void cl_update(){

  // current max x velocity
  double xVelMax = s.run ? k_xVelRunMax : k_xVelMax;

  // reference to player's x velocity
  double *vx = &(s.scene[s.pli].vx);

  // -1 if moving backward
  //  0 if not moving
  //  1 if moving forward
  int dirMove = (s.forward - s.backward);

  if(s.crouch){ // no control if you're crouched
      *vx -= copysign(k_xVel, *vx);
  }
  else{
    // accelerate as long as we're holding the key and haven't exceeded max velocity
    if(dirMove * *vx < xVelMax){*vx+=dirMove * k_xVel;}

    // slow down if we're not actively going in the direction of motion
    if(s.onGround && dirMove * *vx<=0){*vx-=copysign(k_xVel, *vx);}
  }

  // cap out at max velocity
  if(fabs(*vx) > xVelMax){*vx = copysign(xVelMax, *vx);}

  if(fabs(*vx) < k_xVelMin){ // if we're moving slowly
    if(dirMove * *vx > 0) {// if we're trying to move in that direction
      *vx = copysign(k_xVelMin, *vx); // give us a boost, so we start moving immediately
    }
    else { // if we're not trying to move in that direction
      *vx = 0; // halt
    }
  }

  // jump times out
  if(!--s.upcount){cl_jumpEnd();}

  // running also zooms vision, to accentuate the effect
  if(*vx > k_xVelMax){s.runWarp++;}
  else if(s.runWarp){s.runWarp--;}

  // reset bounce counter when we land
  if(s.onGround){s.multibounce = 0;}
}

bool cl_move1(int i, char dir, int inc){
  bool ret = true;
  if(dir == 'x'){s.scene[i].x += inc;}
  else{s.scene[i].y += inc;}
  for(int obj=0; s.scene[obj].type != '\0'; obj++){
    if(obj==i) {continue;}
    ret &= !mh_collision(i, obj);
  }
  if(!ret){
    if(dir == 'x'){s.scene[i].x -= inc;}
    else{s.scene[i].y -= inc;}
  }
  return ret;
}

bool cl_move(int i, char dir, int amt){
  bool ret = true;
  int inc = SGN(amt);
  while(amt && ret){
    ret = cl_move1(i, dir, inc);
    amt -= inc;
  }
  return ret;
}

void cl_uncrouch(){
  if(s.crouch){
    s.crouch = false;
    if(s.bigMario){
      s.scene[s.pli].y += 16;
      s.scene[s.pli].bb.h -= 16;
      s.scene[s.pli].cols[0].h -= 16;
      s.scene[s.pli].cols[2].y -= 16;
    }
  }
}

void cl_crouch(){
  if(s.pipeTo != '\0'){
    au_play(SND_pipe);
    s.scene[s.pli].y--;
  }
  else if(!s.crouch){
    s.crouch = true;
    if(s.bigMario){
      s.scene[s.pli].y -= 16;
      s.scene[s.pli].bb.h += 16;
      s.scene[s.pli].cols[0].h += 16;
      s.scene[s.pli].cols[2].y += 16;
    }
  }

}

void cl_starman(){
  au_mainPlay(SND_starmusic);
  s.star = 1000;
}

void cl_fireMario(){
  s.fire = true;
  au_play(SND_mushroomeat);
}

void cl_bigMario(){
  if(s.bigMario == false){
    au_play(SND_mushroomeat);
    s.bigMario = true;
    s.scene[s.pli].i = act_startGrow;
  }
  for(int i=0; s.scene[i].type != '\0';i++){
    if(s.scene[i].type == '?' && s.scene[i].c == 'r'){s.scene[i].c = 'R';}
  }
}

void cl_smallMario(){
  if(s.bigMario == true){
    cl_uncrouch();
    if(!s.dead){
      au_play(SND_shrink);
    }
    s.bigMario = false;
    s.fire = false;
    for(int i=0; s.scene[i].type != '\0';i++){
      if(s.scene[i].type == '?' && s.scene[i].c == 'R'){s.scene[i].c = 'r';}
    }
  }
}

void cl_smallJump(){
  au_play(SND_stomp);
  s.scene[s.pli].vy = k_yVel;
  s.onGround = false;
  s.multibounce++;
}

void cl_jumpStart(){
  if(s.onGround) {
    if(s.bigMario){au_play(SND_jumpbig);} else{au_play(SND_jump);}
    s.scene[s.pli].vy = k_yVel;
    s.gravity /= 5;
    s.upcount = k_nJumpFrames;
  }
}

void cl_jumpEnd(){
  s.gravity = k_gravity;
}

void cl_click(int button, int action){
  if(button == SDL_BUTTON_LEFT && action == SDL_PRESSED){
    if(s.fire){
      cl_fire();
    }
  }
}

void cl_fire(){
  if(s.nFBalls < 2){
    au_play(SND_fireball);
    s.nFBalls++;
    int i=0;
    for(i=0;s.scene[i].type != '\0' && s.scene[i].type != '.';i++);
    s.scene[i] = ob_objFchar('o');
    s.scene[i].x = s.scene[s.pli].x+14;
    s.scene[i].y = s.scene[s.pli].y-2;
    s.scene[i].vx = -4.5 * (s.flip*2-1);
  }
}

void cl_keypress(int key, int state, int mods){
  if (KEY_DN(forward)){
    if(!s.flip){s.forward = true;}
    else{s.backward = true;}
  }
  if (KEY_UP(forward)){
    if(!s.flip){s.forward = false;}
    else{s.backward = false;}
  }

  if (KEY_DN(backward)){
    if(!s.flip){s.backward = true;}
    else{s.forward = true;}
  }
  if (KEY_UP(backward)){
    if(!s.flip){s.backward = false;}
    else{s.forward = false;}
  }

  if (KEY_DN(turn)){
    bool temp = s.forward;
    s.forward = s.backward;
    s.backward = temp;
    s.flip = !s.flip;
  }

  if (KEY_DN(run)){
    s.run = true;
  }
  if (KEY_UP(run)){
    s.run = false;
  }

  if (KEY_DN(jump)){
    cl_jumpStart();
  }
  if (KEY_UP(jump)){
    cl_jumpEnd();
  }

  if (KEY_DN(crouch)){
    cl_crouch();
  }
  if (KEY_UP(crouch)){
    cl_uncrouch();
  }

  if (key == SDLK_ESCAPE && state == SDL_PRESSED){
    if(s.paused && !s.userPaused) {return;}
    s.paused = !s.paused;
    s.userPaused = !s.userPaused;
    if(s.paused){
      au_mainStop();
      au_play(SND_pause);
    }
    else{
      au_mainPlay(SND_continueMain);
    }
  }
}

void cl_pipe(){
  gl_loadLevel(s.pipeTo, s.loc);
}

void cl_gravity(int i){
  if(s.scene[i].gravity){
    s.scene[i].vy += i == s.pli ? s.gravity : k_gravity;
    if(s.scene[i].vy<k_yVelMin){s.scene[i].vy=k_yVelMin;}
  }
}

void cl_delObjAt(int i){
  cl_delObj(&s.scene[i]);
}

void cl_delObj(obj* obj){
  if((*obj).type == 'o' || (*obj).type == 'O'){
    s.nFBalls--;
  }
  (*obj) = ob_objFchar('.');
}
