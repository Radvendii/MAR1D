#include "controls.h"

void cl_init(){
    s.backward = false;
    s.forward = false;
    s.upcount = 0;
}

void cl_update(){
    if(s.forward && s.scene[s.pli].vx<k_xVelMax){s.scene[s.pli].vx+=k_xVel;}
    if(!s.forward && s.onGround && s.scene[s.pli].vx>0){s.scene[s.pli].vx-=k_xVel;}
    if(s.backward && s.scene[s.pli].vx>-k_xVelMax){s.scene[s.pli].vx-=k_xVel;}
    if(!s.backward && s.onGround && s.scene[s.pli].vx<0){s.scene[s.pli].vx+=k_xVel;}
    if(!--s.upcount){cl_jumpEnd();}
}

bool cl_move1(int i, char dir, bool pos){
    bool ret=true;
    if(dir == 'x'){s.scene[i].x += (pos ? 1 : -1);}
    else{s.scene[i].y += (pos ? 1 : -1);}
    for(int obj=0;;obj++){
        if(s.scene[obj].type[0] == '\0') {break;}
        if(obj==i) {continue;}
        if(mh_collision(i, obj)) {ret = false;}
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

void cl_jumpStart(){
    if(s.onGround) {
        s.scene[s.pli].vy = k_yVel;
        s.gravity/=5;
        s.upcount = k_nJumpFrames;
    }
}

void cl_smallJump(){
        s.scene[s.pli].vy = k_yVel;
}

void cl_jumpEnd(){
    s.gravity= k_gravity;
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


    if (key == GLFW_KEY_Q && action == GLFW_PRESS){
        s.paused = !s.paused;
    }
}

void cl_gravity(){
    s.scene[s.pli].vy += s.gravity;
    if(s.scene[s.pli].vy<k_yVelMin){s.scene[s.pli].vy=k_yVelMin;}
}
