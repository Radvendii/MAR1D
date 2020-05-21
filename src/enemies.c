#include "enemies.h"

// Enemies are initially inactive unless they are right next to the player
void ai_init(){
  for(int i=0;s.scene[i].type != '\0';i++){
    if((s.scene[i].type == 'e' || s.scene[i].type == '&') && s.scene[i].x >  s.scene[s.pli].x + (16*8)){
      s.scene[i].active = false;
    }
  }
}

void ai_killAt(int i){
  ai_kill(&s.scene[i]);
}

// Makes the enemy nonphysical and shorter / squashed.
void ai_kill(obj* e){
  au_play(SND_shot);
  (*e).physical = false;
  (*e).vy = 8;
}

void ai_update(){
  for(int i=0;s.scene[i].type != '\0';i++){
    if((s.scene[i].type == 'e' || s.scene[i].type == '&') && s.scene[i].x < s.scene[s.pli].x + (16*8)+16){
      ai_activate(s.scene[i].x, s.scene[i].y);
    }
  }
}

// Activates enemies in groups
void ai_activate(int x, int y){
  for(int i=0;s.scene[i].type != '\0'; i++){
    if(s.scene[i].x == x && s.scene[i].y == y && s.scene[i].active == false){
      s.scene[i].active = true;
      ai_activate(x-2*16, y);
      ai_activate(x-1*16, y);
      ai_activate(x+1*16, y);
      ai_activate(x+2*16, y);
    }
  }
}
