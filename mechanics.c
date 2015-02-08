#include "mechanics.h"

void mh_init(){
    s.onGround = true;
    s.paused = false;
    s.gravity = k_gravity;
    s.upcount = 0;
    s.moveFrameY = 0;
    s.moveFrameX = 0;
}

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

    bool colisY = !cl_go('y', yMove);
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
    bool colisX = !cl_go('x', xMove);
    if(colisX == true){
        s.velX = 0;
    }
AFTER_X_MOTION: ;
}

bool mh_isCollision(int i1, int i2){
    if(i1 % 3 !=0 || i2 % 3 !=0){printf("Error in mh_isCollision(): indicies must be the beginning of object (i.e. %%3==0)");exit(1);}
    box b1 = ob_boxes[s.scene[i1]];
    box b2 = ob_boxes[s.scene[i2]];
    ob_realifyBox(&b1, (s.scene + i1 + 1));
    ob_realifyBox(&b2, (s.scene + i2 + 1));
    return !(k_oneSide(b1.x, b1.x+b1.w, b2.x, b2.x+b2.w) || k_oneSide(b1.y, b1.y+b1.h, b2.y, b2.y+b2.h));
}

bool mh_playerCollision(int i){
    int obj=0;
    while(!(s.scene[obj] == '\0')){obj+=3;}
    s.scene[obj] = '@';
    s.scene[obj+1] = s.x;
    s.scene[obj+2] = s.y;
    s.scene[obj+3] = '\0';
    bool ret = mh_isCollision(obj, i);
    if(ret){
        switch(s.scene[i]){
            case 'c':
                s.coins++;
                s.scene[i]='.';
                ret = false;
                break;
            case '.':
                ret = false;
        }
    }
    s.scene[obj] = '\0';
    return ret;
}

