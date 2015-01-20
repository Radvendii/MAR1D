#include <GLFW/glfw3.h>
#include "controls.h"
#include "helpers.h"
#include "mechanics.h"
#include "objects.h"

bool cl_go1(struct world*w, char dir, bool pos){
    int dirNum = dir == 'x' ? 1 : 2;
    (*w).scene[dirNum] += pos*2-1;
    for(int obj=0;;obj+=3){
        if((*w).scene[obj] == terminator) {break;}
        if((*w).scene[obj] == objPlayer) {continue;}
        if(mh_isCollision(*w, 0, obj)) {(*w).scene[dirNum] -= pos*2-1; return false;}
    }
    if(dir == 'x'){(*w).camX += pos*2-1;}
    else{(*w).camY += pos*2-1;}
    return true;
}

bool cl_go(struct world *w, char dir, int amt){
    /*
     *int dirNum = dir == 'x' ? 1 : 2;
     *(*w).scene[dirNum] += amt;
     *for(int obj=0;;obj+=3){
     *    if((*w).scene[obj] == terminator) {break;}
     *    if((*w).scene[obj] == objPlayer) {continue;}
     *    if(mh_isCollision(*w, 0, obj)) {(*w).scene[dirNum] -= amt; return false;}
     *}
     *if(dir == 'x'){(*w).camX += amt;}
     *else{(*w).camY += amt;}
     *return true;
     */
    bool ret=true;
    while(amt != 0){
        ret = ret && cl_go1(w, dir, amt > 0);
        if(amt>0){amt--;}
        else{amt++;}
    }
    return ret;
}

void cl_keypress(struct state *s, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_W && action == GLFW_PRESS){
        if(!(*s).camFlip){(*s).velX += k_xVel;}
        else{(*s).velX -= k_xVel;}
    }
    if (key == GLFW_KEY_W && action == GLFW_RELEASE){
        if(!(*s).camFlip){(*s).velX -= k_xVel;}
        else{(*s).velX += k_xVel;}
    }

    if (key == GLFW_KEY_S && action == GLFW_PRESS){
        if(!(*s).camFlip){(*s).velX -= k_xVel;}
        else{(*s).velX += k_xVel;}
    }
    if (key == GLFW_KEY_S && action == GLFW_RELEASE){
        if(!(*s).camFlip){(*s).velX += k_xVel;}
        else{(*s).velX -= k_xVel;}
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
        if((*s).onGround) {(*s).velY = 5.5;}
        (*s).onGround = false;
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE){
        (*s).velY -= 0;
    }
    if (key == GLFW_KEY_Q && action == GLFW_PRESS){
        (*s).paused = !(*s).paused;
    }
    if ((key == GLFW_KEY_A || key == GLFW_KEY_D) && action == GLFW_PRESS){
        (*s).velX = -(*s).velX;
        (*s).camFlip = !(*s).camFlip;
        (*s).world.camT = 180-(*s).world.camT-k_FOV;
    }
}

void cl_cursormove(struct state *s, double xPos, double yPos){
    if(!(*s).camFlip){(*s).world.camT = -yPos/80;}
    else{(*s).world.camT = 180+yPos/80-k_FOV;}
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

void cl_gravity(struct state *s){
    (*s).velY -= .2;
}
