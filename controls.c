#include "controls.h"
#include "helpers.h"
#include "mechanics.h"
#include "objects.h"

bool cl_go(struct world *w, char dir, int amt){
    int dirNum = dir == 'x' ? 1 : 2;
    (*w).scene[dirNum] += amt;
    for(int obj=0;;obj+=3){
        if((*w).scene[obj] == terminator) {break;}
        if((*w).scene[obj] == objPlayer) {continue;}
        if(mh_isCollision(*w, 0, obj)) {(*w).scene[dirNum] -= amt; return false;}
    }
    if(dir == 'x'){(*w).camX += amt;}
    else{(*w).camY += amt;}
    return true;
}

bool cl_forward(struct world *w){
    return cl_go(w, 'x', 1);
}

bool cl_backward(struct world *w){
    return cl_go(w, 'x', -1);
}

bool cl_upward(struct world *w){
    return cl_go(w, 'y', 2);
}

bool cl_gravity(struct world *w){
    return cl_go(w, 'y', -1);
}
