#include "gamelogic.h"

int gl_playerIndex(){
    int ret=0;
    for(ret=0;ob_levels['t'][ret].type != '\0' && ob_levels['t'][ret].type != '@';ret++);
    if(ob_levels['t'][ret].type == '\0'){printf("Error in gl_playerIndex(): Player not found\n");exit(1);}
    return ret;
}

void gl_init(){
    s.bigMario = false;
    s.invincible = 1000;
    s.star = 1000;
    s.lives = 3;
    s.scene = salloc(sizeof(obj) * k_nMaxObj);
    s.pli = gl_playerIndex();

    int i;
    for(i=0; ob_levels['t'][i].type != '\0';i++){s.scene[i] = ob_levels['t'][i];}
    s.scene[i].type = '\0';
    s.flip = false;
    s.leftMost = 0;
    s.coins = 0;
    mh_init();
    cl_init();
    ai_init();
    for(int i=0;s.scene[i].type != '\0'; i++){
        if(s.scene[i].type == 'e'){s.scene[i].vx=-0.5;}
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


void gl_die(){
    if(!s.lives){exit(0);}
    int i;
    for(i=0; ob_levels['t'][i].type != '\0';i++){s.scene[i] = ob_levels['t'][i];}
    s.scene[i].type = '\0';
    s.flip = false;
    s.leftMost = 0;
    for(int i=0;s.scene[i].type != '\0'; i++){
        if(s.scene[i].type == 'e'){s.scene[i].vx=-0.5;}
    }
    s.lives--;
    ai_init();
}

void gl_update(){
    cl_update();
    mh_update();
    ai_update();
    if(s.leftMost<s.scene[s.pli].x-16*16){
        s.leftMost = s.scene[s.pli].x-16*16;
    }
    s.pli = gl_playerIndex();
    if(s.invincible){s.invincible--;}
    if(s.star){s.star--;}
}

void gl_deinit(){
    free(s.scene);
}
