#include "gamelogic.h"

// escape from all loops and exit the game
bool gameEnd;

// Time that has been intentionally spent waiting, rather than time spent
// executing code. This is important for synchronizing game ticks.
int gl_playerIndex(){
  int ret=0;
  for(ret=0;s.scene[ret].type != '\0' && s.scene[ret].type != '@';ret++);
  if(s.scene[ret].type == '\0'){DEBUG("Error in gl_playerIndex(): Player not found");exit(EXIT_FAILURE);}
  return ret;
}

void gl_init(){
  s.scene = salloc(sizeof(obj) * k_nMaxObj);
}

// main game loop
void gl_main() {
  wn_eventCallbacks(&gl_keypress, &gl_mouseclick, &gl_mousemove);
  gl_load();
  gameEnd = false;
  io_resetRec();

  /*
   * the last SDL time at which the game was updated.
   * used to set a constant tick rate for the game.
   */
  int lastSDLTime = SDL_GetTicks();
  // main game loop
  while (!quit && !gameEnd) {

    //TODO: Do this when the pause button is pressed so that it doesn't have to happen every time through the loop.
    if (s.userPaused) {
      wn_disable_mouse(false);
    }
    else {
      wn_disable_mouse(true);
    }

    wn_menuWindow();
    gr_clear();
    wn_perspWindow();
    gr_clear();
    wn_dimWindow();
    gr_clear();
    wn_hudWindow();
    gr_clear();

    // update the state
    au_update();
    vs_update();
    // fix the current time in a variable so there's no chance of an infinite loop
    int curSDLTime = SDL_GetTicks();
    // update the game logic until we've caught up with the current time
    // (this will usually just be once)
    while (curSDLTime > lastSDLTime && !gameEnd && !quit) {
      if (au_waiting == SND_none) { // if we're waiting for audio, don't progress the game at all
        gl_update();
      }
      gr_update();
      lastSDLTime += k_msPerGameTick;
    }
    if (!gameEnd && !quit) {
      // draw updates
      wn_perspWindow();
      gr_drawPersp();
      if (conf.visualSounds) {
        vs_draw();
      }
      if (conf.debug) {
        wn_dimWindow();
        gr_drawDim();
      }
      wn_hudWindow();
      gr_drawHud();
      wn_update();
    }
  }

  SAFE_DELAY_UNTIL(au_waiting == SND_none);

  // release mouse on game exit
  wn_disable_mouse(false);
}

void gl_keypress(SDL_KeyboardEvent ev) {
  if (!ev.repeat) {
    cl_keypress(ev.keysym.sym, ev.state, ev.keysym.mod);
    gr_keypress(ev.keysym.sym, ev.state, ev.keysym.mod);
  }
}

void gl_mouseclick(SDL_MouseButtonEvent ev) {
  cl_click(ev.button, ev.state);
}

void gl_mousemove(SDL_MouseMotionEvent ev) {
  gr_mousemove(ev.xrel, ev.yrel);
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
  s.time = k_gameTicksPerTimeTick * k_time;
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
  s.star = false;
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
  // winning proceeds in stages
  static int stage = 0;

  switch(stage) {
  case 0:
    s.won = true;
    s.paused = true;
    s.score+=400;
    au_mainStop();
    au_playWait(SND_levelend);
    stage++;
    break;
  case 1:
    au_mainPlay(SND_scorering);
    stage++;
    break;
  case 2:
    if (s.time > k_gameTicksPerTimeTick) {
      s.time -= k_gameTicksPerTimeTick;
      s.score+=50;
    }
    else {
      stage++;
    }
    break;
  case 3:
    au_mainStop();
    mu_setWinScore(s.score);
    mu_menu(mu_winMenu);
    gameEnd = true;
    stage = 0;
    break;
  }
}

void gl_loadLevel(char l, char w){
  int i;
  int wi = -1;

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
  if(wi != -1){
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
  }
  else{
    au_mainPlay(s.lowTime ? SND_overworld_fast : SND_overworld);
  }
  s.paused = false;
  s.userPaused = false;
}

void gl_resetLevel(){
  if(s.lives){
    cl_smallMario();
    s.lowTime = false;
    gl_loadLevel(s.level, s.check);
    s.time = k_gameTicksPerTimeTick * k_time;
  }
  else{
    au_playWait(SND_gameover);
    gameEnd = true;
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
    if(s.invincible == 50){
      au_mainPlay(s.level == 's' //TODO: Make this not a kludge
                  ? (s.lowTime ? SND_underground_fast : SND_underground)
                  : (s.lowTime ? SND_overworld_fast : SND_overworld));
    }
    if(s.star){s.star--;}
    if(s.star == 50){
      au_mainPlay(s.level == 's'
                  ? (s.lowTime ? SND_underground_fast : SND_underground)
                  : (s.lowTime ? SND_overworld_fast : SND_overworld));
    }
    if(s.time){s.time--;}
    else{gl_die();}

    if(s.time == k_timeLow * k_gameTicksPerTimeTick){
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
