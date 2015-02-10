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
    for(int i=0;s.scene[i*3] != '\0';i++){
        if(s.action[i] == act_nothing){continue;}
        if((s.scene[i*3] == 'C' || s.scene[i*3] == 'b') && s.action[i] <= act_bounceD){
            s.scene[i*3+2]--;
            s.action[i]--;
        }
        else if((s.scene[i*3] == 'C' || s.scene[i*3] == 'b') && s.action[i] <= act_bounce){
            s.scene[i*3+2]++;
            s.action[i]--;
        }
        if(s.scene[i*3] == 'C' && s.action[i] == act_nothing){
            s.scene[i*3] = 'D';
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

int mh_isCollision(int i1, int i2){ //returns side of collision starting from front and moving clockwise
    if(i1 % 3 !=0 || i2 % 3 !=0){printf("Error in mh_isCollision(): indicies must be the beginning of object (i.e. %%3==0)");exit(1);}
    int ret = 0;
    box b1 = ob_objs[s.scene[i1]].bb;
    box b2 = ob_objs[s.scene[i2]].bb;
    ob_realifyBox(&b1, (s.scene + i1 + 1));
    ob_realifyBox(&b2, (s.scene + i2 + 1));
    ret += k_boxInter(b1, b2);
    int nCols = ob_objs[s.scene[i1]].nCols;
    for(int i=0;i<nCols;i++){
        b1 = ob_objs[s.scene[i1]].cols[i];
        ob_realifyBox(&b1, (s.scene + i1 + 1));
        if(k_boxInter(b1, b2)){ret += pow(2, i+1);}
    }
    return ret;
}

bool mh_playerCollision(int i){
    int obj=0;
    while(!(s.scene[obj] == '\0')){obj+=3;}
    s.scene[obj] = '@';
    s.scene[obj+1] = s.x;
    s.scene[obj+2] = s.y;
    s.scene[obj+3] = '\0';
    int ret = mh_isCollision(i, obj);
    if(ret){
        switch(s.scene[i]){
            case 'c':
                s.coins++;
                s.scene[i]='.';
                ret = false;
                break;
            case '.':
                ret = false;
                break;
            case 'C':
                if(ret/2){
                    s.coins++;
                }
            case 'b':
                if(ret/2){
                    s.action[i/3] = act_bounce;
                    cl_jumpEnd();
                }
                break;
        }
    }
    s.scene[obj] = '\0';
    return ret%2;
}

