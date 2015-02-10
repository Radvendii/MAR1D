#include "gamelogic.h"

void gl_init(){
    mh_init();
    cl_init();
    s.scene = salloc(sizeof(int) * k_nMaxObj*3);
    s.action = salloc(sizeof(int) * k_nMaxObj);
    int pl;
    int i=0;
    for(i=0;!((s.scene[i] = ob_levelTest[i]) == '\0' && (i%3 == 0));i++){
        s.action[i/3] = act_nothing;
        if(ob_levelTest[i] == '@' && i%3==0){pl = i;}
    }
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
    free(s.action);
}
