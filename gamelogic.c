#include "gamelogic.h"

int gl_playerIndex(){
    int ret=0;
    for(ret=0;s.scene[ret].type != '\0' && s.scene[ret].type != '@';ret++);
    if(s.scene[ret].type == '\0'){printf("Error in gl_playerIndex(): Player not found\n");exit(1);}
    return ret;
}

void gl_init(){
    s.bigMario = false;
    s.star = false;
    s.invincible = false;
    s.dead = false;
    s.fire = false;
    s.lives = 3;
    s.nFBalls = 0;
    s.scene = salloc(sizeof(obj) * k_nMaxObj);

    int i;
    for(i=0; ob_levels['t'][i].type != '\0';i++){s.scene[i] = ob_levels['t'][i];}
    s.scene[i].type = '\0';
    s.pli = gl_playerIndex();
    s.flip = false;
    s.leftMost = 0;
    s.coins = 0;
    mh_init();
    cl_init();
    ai_init();
    for(int i=0;s.scene[i].type != '\0'; i++){
        if(s.scene[i].type == 'e' || s.scene[i].type == '&'){s.scene[i].vx=-0.5;}
    }
}

void gl_killed(){
    if(s.invincible){return;}
    if(s.bigMario){
        cl_smallMario();
        s.invincible = 400;
    }
    else{gl_die();}
}

void gl_die(){ //TODO: consolodate reset level stuff into one function
    //if(!s.lives){exit(0);}
    s.paused = true;
    s.scene[s.pli].physical = false;
    s.scene[s.pli].vy=10;
    s.scene[s.pli].vx=0;
    s.dead = k_dieStart;
}

void gl_win(){
    gl_resetLevel();
}

void gl_resetLevel(){
    cl_smallMario();
    int i;
    for(i=0; ob_levels['t'][i].type != '\0';i++){s.scene[i] = ob_levels['t'][i];}
    s.scene[i].type = '\0';
    s.pli = gl_playerIndex();
    s.flip = false;
    s.leftMost = 0;
    s.nFBalls = 0;
    s.dead = false;
    mh_init();
    cl_init();
    ai_init();
    for(i=0;s.scene[i].type != '\0'; i++){
        if(s.scene[i].type == 'e' || s.scene[i].type == '&'){s.scene[i].vx=-0.5;}
    }
    s.lives--;
}

void gl_update(){
    cl_update();
    mh_update();
    ai_update();
    if(s.leftMost<s.scene[s.pli].x-16*8){
        s.leftMost = s.scene[s.pli].x-16*8;
    }
    for(int i=0;s.scene[i].type != '\0';i++){
        if(s.scene[i].x<s.leftMost){
            cl_delObjAt(i);
        }
    }
    s.pli = gl_playerIndex();
    if(s.invincible){s.invincible--;}
    if(s.star){s.star--;}
}

void gl_deinit(){
    free(s.scene);
}
