#include "gamelogic.h"

void gl_init(){
    mh_init();
    cl_init();
    s.world.scene = salloc(sizeof(int) * k_nMaxObj*3);
    for(int i=0;!((s.world.scene[i] = ob_levelTest[i]) == terminator && (i%3 == 0));i++);
    s.world.camX = ob_levelTest[1]+14;
    s.world.camY = ob_levelTest[2]-2;
    s.world.camT = 0;
    s.camFlip = false;
}

void gl_update(){
    cl_update();
    mh_update();
}

void gl_deinit(){
    free(s.world.scene);
}
