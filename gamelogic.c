#include "gamelogic.h"

void gl_init(){
    mh_init();
    cl_init();
    s.scene = salloc(sizeof(int) * k_nMaxObj*3);
    int pl;
    for(int i=0;!((s.scene[i] = ob_levelTest[i]) == '\0' && (i%3 == 0));i++){if(ob_levelTest[i] == '@' && i%3==0){pl = i;}}
    s.x = ob_levelTest[pl+1];
    s.y = ob_levelTest[pl+2];
    s.flip = false;
}

void gl_update(){
    cl_update();
    mh_update();
}

void gl_deinit(){
    free(s.scene);
}
