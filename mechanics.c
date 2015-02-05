#include "mechanics.h"

void mh_update(){
    cl_gravity();
    s.moveFrameY++;
    s.moveFrameX++;
    double yMove = s.velY;
    double xMove = s.velX;
    if(s.velY < 1){
        if((s.moveFrameY %= k_moveFrames) == 0){yMove *= 2;}
        else{goto AFTER_Y_MOTION;}
    }

    bool colisY = !cl_go(&s.world, 'y', yMove);
    if(colisY == true){
        if(s.velY <= 0) {s.onGround = true;}
        s.velY = 0;
    }
    else if((int) s.velY != 0){s.onGround = false;}
AFTER_Y_MOTION: ;
    if(s.velX < 1){
        if((s.moveFrameX %= k_moveFrames) == 0){xMove *= 2;}
        else{goto AFTER_X_MOTION;}
    }
    bool colisX = !cl_go(&s.world, 'x', xMove);
    if(colisX == true){
        s.velX = 0;
    }
AFTER_X_MOTION: ;
}

bool mh_isCollision(struct world w, int i1, int i2){
    if(i1 % 3 !=0 || i2 % 3 !=0){printf("Error in mh_isCollision(): indicies must be the beginning of object (i.e. %%3==0)");exit(1);}
    line l1, l2;
    line *o1 = objFtype_old(w.scene[i1]);
    line *o2 = objFtype_old(w.scene[i2]);
    for(int j1=0;;j1++){
        l1 = objFtype_old(w.scene[i1])[j1];
        if(ob_isTerminating(l1)){break;}
        ob_realifyLine(&l1, (w.scene + i1 + 1));
        for(int j2=0;;j2++){
            l2 = objFtype_old(w.scene[i2])[j2];
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
