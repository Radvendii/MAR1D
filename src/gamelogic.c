#include "gamelogic.h"

int gl_playerIndex(){
  int ret=0;
  for(ret=0;s.scene[ret].type != '\0' && s.scene[ret].type != '@';ret++);
  if(s.scene[ret].type == '\0'){printf("Error in gl_playerIndex(): Player not found\n");exit(1);}
  return ret;
}

void gl_init(){
  s.scene = salloc(sizeof(obj) * k_nMaxObj);
}

void gl_load(){
  s.level = '1';
  s.loc = '1';
  s.pli = -1;
  s.bigMario = false;
  s.star = false;
  s.check = 0;
  s.invincible = false;
  s.dead = false;
  s.fire = false;
  s.lives = 3;
  s.nFBalls = 0;
  s.coins = 0;
  s.score = 0;
  s.lowTime = false;
  gl_loadLevel(s.level, '\0');
  s.time = k_timeTick * k_time;
}

void gl_killed(){
  if(s.invincible){return;}
  if(s.bigMario){
    cl_smallMario();
    s.invincible = 400;
  }
  else{gl_die();}
}

void gl_die(){
  s.dead = k_dieStart;
  au_mainStop();
  au_play(SND_death);
  cl_uncrouch();
  cl_smallMario();
  s.paused = true;
  s.scene[s.pli].physical = false;
  s.scene[s.pli].vy=10;
  s.scene[s.pli].vx=0;
  s.lives--;
}

void gl_win(){
  if(!s.won){
    au_mainStop();
    au_play(SND_levelend);
    sleep(2);
    s.won = true;
    s.score+=400;
    s.paused = true;
    au_mainPlay(SND_scorering);
  }
  else if(s.time/k_timeTick > 0){
    s.time -= k_timeTick;
    s.score+=50;
  }
  else{
    au_mainStop();
    s.menu = k_menuWin;
    s.lives = 3;
  }
}

void gl_loadLevel(char l, char w){
  int i;
  int wi;

  obj keepMario; //we want to keep mario's bigness state
  keepMario.type = '\0';
  if(s.pli != -1){
    keepMario = s.scene[s.pli];
  }

  for(i=0; ob_levels[l][i].type != '\0';i++){
    s.scene[i] = ob_levels[l][i];
    if(w != '\0' && (ob_levels[l][i].type == '.' || ob_levels[l][i].type == '`') && ob_levels[l][i].c == w){wi = i;}
  }
  s.scene[i].type = '\0';
  s.pli = gl_playerIndex();
  if(keepMario.type != '\0'){
    obj tmp = s.scene[s.pli];
    s.scene[s.pli] = keepMario;
    s.scene[s.pli].x = tmp.x;
    s.scene[s.pli].y = tmp.y;
    s.scene[s.pli].physical = tmp.physical;
    s.scene[s.pli].hidden = tmp.hidden;

  }
  if(w != '\0'){
    s.scene[s.pli].x = s.scene[wi].x;
    s.scene[s.pli].y = s.scene[wi].y;
  }
  s.flip = false;
  s.leftMost = s.scene[s.pli].x > 16*8 ? s.scene[s.pli].x - 16*8 : 0;
  s.nFBalls = 0;
  s.dead = false;
  mh_init();
  cl_init();
  ai_init();
  for(i=0;s.scene[i].type != '\0'; i++){
    if(s.scene[i].type == 'e' || s.scene[i].type == '&'){s.scene[i].vx=-0.5;}
  }
  s.loc = l;
  if(l == 's'){ //TODO: make this not a kludge
    au_mainPlay(s.lowTime ? SND_underground_fast : SND_underground);
    io_cs['l'] = (color) {.r = 0, .g = 255, .b = 255};
    io_cs['b'] = (color) {.r = 0, .g = 139, .b = 139};
    io_cs['d'] = (color) {.r = 0, .g = 92, .b = 92};
  }
  else{
    au_mainPlay(s.lowTime ? SND_overworld_fast : SND_overworld);
    io_cs['l'] = (color) {.r = 231, .g = 95, .b = 19};
    io_cs['b'] = (color) {.r = 255, .g = 128, .b = 57};
    io_cs['d'] = (color) {.r = 212, .g = 76, .b = 0};
  }
  s.paused = false;
  s.userPaused = false;
}

void gl_resetLevel(){
  if(s.lives){
    cl_smallMario();
    s.lowTime = false;
    gl_loadLevel(s.level, s.check);
    s.time = k_timeTick * k_time;
  }
  else{
    au_playWait(SND_gameover);
    s.menu = k_menuStatic;
  }
}

void gl_update(){
  cl_update();
  mh_update();
  ai_update();
  if(s.leftMost<s.scene[s.pli].x-16*8){
    s.leftMost = s.scene[s.pli].x-16*8;
  }
  for(int i=0;s.scene[i].type != '\0';i++){
    if(s.scene[i].x<s.leftMost+16*16){s.scene[i].onScreen = true;}
    if(s.scene[i].x<s.leftMost || (s.scene[i].x > s.leftMost+16*20 && s.scene[i].onScreen == true)){
      /* cl_delObjAt(i); */
    }
  }
  if(s.scene[s.pli].x > 76 * 16){ //TODO: Make this not a kludge
    s.check = '1';
  }
  s.pli = gl_playerIndex();
  if(!s.paused){
    if(s.invincible){s.invincible--;}
    if(s.invincible == 50){au_mainPlay(SND_overworld);}
    if(s.star){s.star--;}
    if(s.star == 50){au_mainPlay(SND_overworld);} //this should really be more general than SND_overworld (like for time running out)
    if(s.time){s.time--;}
    else{gl_die();}

    if(s.time == k_timeLow*k_timeTick){
      au_lowTime();
      s.lowTime = true;
    }
  }
  if(s.won){
    gl_win();
  }
}

void gl_deinit(){
  free(s.scene);
}
