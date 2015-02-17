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
    for(int i=0;s.scene[i].type[0] != '\0';i++){
        if(s.scene[i].i == act_nothing){continue;}
        if(s.scene[i].i <= act_bounce){
            s.scene[i].i--;
            s.scene[i].y -= (s.scene[i].i < act_bounceD && s.scene[i].i > act_bounceU) * 2 - 1;
        }
        if(s.scene[i].type[0] == 'C' && s.scene[i].i == act_nothing){
            obj temp = s.scene[i];
            s.scene[i] = ob_objFchar('D');
            s.scene[i].x = temp.x;
            s.scene[i].y = temp.y;
            s.scene[i].i = temp.i;
        }
    }
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

int mh_isCollision(int i1, int i2){
    int ret = 0;
    box b1 = s.scene[i1].bb;
    box b2 = s.scene[i2].bb;
    ob_realifyBox(&b1, s.scene[i1].x, s.scene[i1].y);
    ob_realifyBox(&b2, s.scene[i2].x, s.scene[i2].y);
    ret += k_boxInter(b1, b2);
    int nCols = s.scene[i1].nCols;
    for(int i=0;i<nCols;i++){
        b1 = s.scene[i1].cols[i];
        ob_realifyBox(&b1, s.scene[i1].x, s.scene[i1].y);
        if(k_boxInter(b1, b2)){ret += pow(2, i+1);}
    }
    return ret;
}

bool mh_playerCollision(int i){
    int ret = mh_isCollision(i, s.pli);
    if(ret){
        switch(s.scene[i].type[0]){
            case 'c':
                s.coins++;
                s.scene[i] = ob_objFchar('.');
                ret = false;
                break;
            case '.':
                ret = false;
                break;
            case 'C':
                if(ret/2 && s.scene[i].i == act_nothing){
                    s.coins++;
                }
            case 'b':
                if(ret/2 && s.scene[i].i == act_nothing){
                    s.scene[i].i = act_bounce;
                }
                break;
            case 'e':
                if(ret/2){
                    s.scene[i]=ob_objFchar('.');
                }
                else{s.scene[s.pli].y-=100;}
                break;
        }
    }
    if(ret%2){
        cl_jumpEnd();
    }
    return ret%2;
}

