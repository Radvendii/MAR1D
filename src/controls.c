#include "controls.h"

void cl_init(){
  s.backward = false;
  s.forward = false;
  s.upcount = 0;
  s.run = false;
  s.multibounce=0;
}

void cl_update(){
  if(s.crouch){
    if(s.scene[s.pli].vx>0){s.scene[s.pli].vx-=k_xVel;}
    if(s.scene[s.pli].vx<0){s.scene[s.pli].vx+=k_xVel;}
  }
  else{
    if(s.forward && s.scene[s.pli].vx<k_xVelMax){s.scene[s.pli].vx+=k_xVel;}
    if(!s.forward && s.onGround && s.scene[s.pli].vx>0){s.scene[s.pli].vx-=k_xVel;}
    if(s.backward && s.scene[s.pli].vx>-k_xVelMax){s.scene[s.pli].vx-=k_xVel;}
    if(!s.backward && s.onGround && s.scene[s.pli].vx<0){s.scene[s.pli].vx+=k_xVel;}
  }
  if(!--s.upcount){cl_jumpEnd();}
  if(k_xVelMax - s.scene[s.pli].vx < 0.0001){s.run++;}
  else if(s.run){s.run--;}
  if(s.onGround){s.multibounce = 0;}
}

bool cl_move1(int i, char dir, bool pos){
  bool ret=true;
  if(dir == 'x'){s.scene[i].x += (pos ? 1 : -1);}
  else{s.scene[i].y += (pos ? 1 : -1);}
  for(int obj=0;;obj++){
    if(s.scene[obj].type == '\0') {break;}
    if(obj==i) {continue;}
    ret &= !mh_collision(i, obj);
  }
  if(!ret){
    if(dir == 'x'){s.scene[i].x -= (pos ? 1 : -1);}
    else{s.scene[i].y -= (pos ? 1 : -1);}
  }
  return ret;
}

bool cl_move(int i, char dir, int amt){
  bool ret=true;
  while(amt != 0 && ret){
    ret = cl_move1(i, dir, amt > 0);
    if(amt>0){amt--;}
    else{amt++;}
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
    au_play(SND_shrink);
    s.bigMario = false;
    s.fire = false;
    cl_uncrouch();
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
    s.gravity/=5;
    s.upcount = k_nJumpFrames;
  }
}

void cl_jumpEnd(){
  s.gravity= k_gravity;
}

void cl_click(int button, int action, int mods){
  if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
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

void cl_keypress(int key, int scancode, int action, int mods){
  if (key == GLFW_KEY_W && action == GLFW_PRESS){
    if(!s.flip){s.forward = true;}
    else{s.backward = true;}
  }
  if (key == GLFW_KEY_W && action == GLFW_RELEASE){
    if(!s.flip){s.forward = false;}
    else{s.backward = false;}
  }

  if (key == GLFW_KEY_S && action == GLFW_PRESS){
    if(!s.flip){s.backward = true;}
    else{s.forward = true;}
  }
  if (key == GLFW_KEY_S && action == GLFW_RELEASE){
    if(!s.flip){s.backward = false;}
    else{s.forward = false;}
  }
  if ((key == GLFW_KEY_A || key == GLFW_KEY_D) && action == GLFW_PRESS){
    bool temp = s.forward;
    s.forward = s.backward;
    s.backward = temp;
    s.flip = !s.flip;
  }

  if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
    cl_jumpStart();
  }
  if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE){
    cl_jumpEnd();
  }

  if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS){
    cl_crouch();
  }
  if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE){
    cl_uncrouch();
  }

  if (key == GLFW_KEY_Q && action == GLFW_PRESS){
    if(s.paused && !s.userPaused) {return;}
    s.paused = !s.paused;
    s.userPaused = !s.userPaused;
    if(s.paused){
      au_mainStop();
      au_playWait(SND_pause);
    }
    else{
      au_mainPlay(au_mainAudio);
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
