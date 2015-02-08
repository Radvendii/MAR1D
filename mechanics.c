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

char letterFtype(enum objType type){
    switch(type){
        case objPlayer:
            return 'p';
        case objBrick:
            return 'b';
        case objGround:
            return 'g';
        case nothing:
            return '.';
    }
}

bool mh_isCollision(struct world w, int i1, int i2){
    if(i1 % 3 !=0 || i2 % 3 !=0){printf("Error in mh_isCollision(): indicies must be the beginning of object (i.e. %%3==0)");exit(1);}
    box b1 = ob_boxes[letterFtype(w.scene[i1])];
    box b2 = ob_boxes[letterFtype(w.scene[i2])];
    ob_realifyBox(&b1, (w.scene + i1 + 1));
    ob_realifyBox(&b2, (w.scene + i2 + 1));
    return !(k_oneSide(b1.x, b1.x+b1.w, b2.x, b2.x+b2.w) || k_oneSide(b1.y, b1.y+b1.h, b2.y, b2.y+b2.h));
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

