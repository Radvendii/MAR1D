#include "mechanics.h"

void mh_init(){
    s.world.scene = salloc(sizeof(int) * k_nMaxObj*3);
    for(int i=0;!((s.world.scene[i] = ob_levelTest[i]) == terminator && (i%3 == 0));i++);
    s.velY = 0;
    s.velX = 0;
    s.onGround = true;
    s.paused = false;
    s.world.camX = ob_levelTest[1];
    s.world.camY = ob_levelTest[2];
    s.world.camT = 0;
    s.camFlip = false;
}

void mh_deinit(){
    free(s.world.scene);
}

void mh_update(){
    cl_gravity();
    bool colis = !cl_go(&s.world, 'y', s.velY);
    cl_go(&s.world, 'x', s.velX);
    if(colis == true){s.velY = 0;
        if(s.velY <= 0) {s.onGround = true;}
    }
    else if(colis == false && (int) s.velY != 0){s.onGround = false;}
}

bool mh_isCollision(struct world w, int i1, int i2){
    if(i1 % 3 !=0 || i2 % 3 !=0){printf("Error in mh_isCollision(): indicies must be the beginning of object (i.e. %%3==0)");exit(1);}
    line l1, l2;
    line *o1 = objFtype(w.scene[i1]);
    line *o2 = objFtype(w.scene[i2]);
    for(int j1=0;;j1++){
        l1 = objFtype(w.scene[i1])[j1];
        if(ob_isTerminating(l1)){break;}
        ob_realifyLine(&l1, (w.scene + i1 + 1));
        for(int j2=0;;j2++){
            l2 = objFtype(w.scene[i2])[j2];
            if(ob_isTerminating(l2)){break;}
            ob_realifyLine(&l2, (w.scene + i2 + 1));
            if(ob_intersectIn(l1, l2)){return true;}
        }
    }
    return false;
}

bool mh_playerCollision(int i){
    bool ret = mh_isCollision(s.world, 0, i);
    if(ret){
        switch(s.world.scene[i]){
            case objCoin:
                s.coins++;
                s.world.scene[i]=nothing;
                ret = false;
                break;
        }
    }
    return ret;
}

void mh_listCollisions(){
    for(int i1=0;;i1++){
        if(s.world.scene[i1*3] == terminator) {break;}
        for(int i2=0;;i2++){
            if(s.world.scene[i2*3] == terminator) {break;}
            printf("%d collides with %d: %d\n", i1, i2, mh_isCollision(s.world, i1*3, i2*3));
        }
        printf("\n");
    }
    printf("\n");
}
