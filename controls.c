#include "controls.h"

void cl_init(){
    s.velY = 0;
    s.velX = 0;
}

void cl_update(){
    if(s.forward && s.velX<k_xVelMax){s.velX+=k_xVel;}
    if(!s.forward && s.onGround && s.velX>0){s.velX-=k_xVel;}
    if(s.backward && s.velX>-k_xVelMax){s.velX-=k_xVel;}
    if(!s.backward && s.onGround && s.velX<0){s.velX+=k_xVel;}
    if(!--s.upcount){cl_jumpEnd();}
}

bool cl_go1(struct world*w, char dir, bool pos){
    int dirNum = dir == 'x' ? 1 : 2;
    (*w).scene[dirNum] += pos*2-1;
    for(int obj=0;;obj+=3){
        if((*w).scene[obj] == terminator) {break;}
        if((*w).scene[obj] == objPlayer) {continue;}
        if(mh_playerCollision(obj)) {(*w).scene[dirNum] -= pos*2-1; return false;}
    }
    if(dir == 'x'){(*w).camX += pos*2-1;}
    else{(*w).camY += pos*2-1;}
    return true;
}

bool cl_go(struct world *w, char dir, int amt){
    bool ret=true;
    while(amt != 0){
        ret = ret && cl_go1(w, dir, amt > 0);
        if(amt>0){amt--;}
        else{amt++;}
    }
    return ret;
}

void cl_jumpStart(){
    if(s.onGround) {s.velY = k_yVel;
        s.gravity/=5;
        s.upcount = k_nJumpFrames;
    }
    s.onGround = false;
}

void cl_jumpEnd(){
    s.gravity= k_gravity;
}

void cl_keypress(int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_W && action == GLFW_PRESS){
        if(!s.camFlip){s.forward = true;}
        else{s.backward = true;}
    }
    if (key == GLFW_KEY_W && action == GLFW_RELEASE){
        if(!s.camFlip){s.forward = false;}
        else{s.backward = false;}
    }

    if (key == GLFW_KEY_S && action == GLFW_PRESS){
        if(!s.camFlip){s.backward = true;}
        else{s.forward = true;}
    }
    if (key == GLFW_KEY_S && action == GLFW_RELEASE){
        if(!s.camFlip){s.backward = false;}
        else{s.forward = false;}
    }
    if ((key == GLFW_KEY_A || key == GLFW_KEY_D) && action == GLFW_PRESS){
        bool temp = s.forward;
        s.forward = s.backward;
        s.backward = temp;
        s.camFlip = !s.camFlip;
        s.world.camT = 180-s.world.camT-k_FOV;
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

void cl_cursormove(double xPos, double yPos){
    if(!s.camFlip){s.world.camT = fmod(-yPos/40, 360);}
    else{s.world.camT = fmod(180+yPos/40-k_FOV, 360);}
}

bool cl_forward(struct world *w){
    return cl_go(w, 'x', 2);
}

bool cl_backward(struct world *w){
    return cl_go(w, 'x', -2);
}

bool cl_upward(struct world *w){
    return cl_go(w, 'y', 2);
}

void cl_gravity(){
    s.velY += s.gravity;
    if(s.velY<k_yVelMin){s.velY=k_yVelMin;}
}
