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

bool cl_move1(int i, char dir, bool pos){
    bool ret=true;
    if(dir == 'x'){s.scene[i].x += pos*2-1;}
    else{s.scene[i].y += pos*2-1;}
    for(int obj=0;;obj++){
        if(s.scene[obj].type[0] == '\0') {break;}
        if(obj==i) {continue;}
        if(mh_isCollision(i, obj)) {ret = false;}
    }
    if(!ret){
        if(dir == 'x'){s.scene[i].x -= pos*2-1;}
        else{s.scene[i].y -= pos*2-1;}
    }
    return ret;
}

bool cl_go1(char dir, bool pos){
    bool ret=true;
    if(dir == 'x'){s.scene[s.pli].x += pos*2-1;}
    else{s.scene[s.pli].y += pos*2-1;}
    for(int obj=0;;obj++){
        if(s.scene[obj].type[0] == '\0') {break;}
        if(s.scene[obj].type[0] == '@') {continue;}
        if(mh_playerCollision(obj)) {ret = false;}
    }
    if(!ret){
        if(dir == 'x'){s.scene[s.pli].x -= pos*2-1;}
        else{s.scene[s.pli].y -= pos*2-1;}
    }
    return ret;
}

bool cl_go(char dir, int amt){
    bool ret=true;
    while(amt != 0){
        ret = ret && cl_go1(dir, amt > 0);
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


bool cl_forward(){
    return cl_go('x', 2);
}

bool cl_backward(){
    return cl_go('x', -2);
}

bool cl_upward(){
    return cl_go('y', 2);
}

void cl_gravity(){
    s.velY += s.gravity;
    if(s.velY<k_yVelMin){s.velY=k_yVelMin;}
}
