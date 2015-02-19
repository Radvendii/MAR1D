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
    if((int)s.scene[s.pli].vy != 0){s.onGround = false;}
    for(int i=0;s.scene[i].type[0] != '\0';i++){
        if(s.scene[i].i == act_nothing){continue;}
        if(s.scene[i].i <= act_bounce && (s.scene[i].type[0] == 'C' || s.scene[i].type[0] == 'b' || s.scene[i].type[0] == 'R')){
            s.scene[i].i--;
            s.scene[i].y -= (s.scene[i].i < act_bounceD && s.scene[i].i > act_bounceU) * 2 - 1;
        }
        if(s.scene[i].type[0] == 'R' && s.scene[i].i == act_nothing){
            int l;
            for(l=0;s.scene[l].type[0] != '\0';l++);
            s.scene[l] = ob_objFchar('r');
            s.scene[l].x = s.scene[i].x;
            s.scene[l].y = s.scene[i].y+16;
            s.scene[l].vx = 1.0;
            s.scene[l+1].type[0] = '\0';
        }
        if((s.scene[i].type[0] == 'C' || s.scene[i].type[0] == 'R') && s.scene[i].i == act_nothing){
            obj temp = s.scene[i];
            s.scene[i] = ob_objFchar('D');
            s.scene[i].x = temp.x;
            s.scene[i].y = temp.y;
            s.scene[i].i = temp.i;
        }
    }
    for(int i=0;s.scene[i].type[0] != '\0';i++){
        if(s.scene[i].gravity){
            s.scene[i].vy += s.gravity;
            if(s.scene[i].vy<k_yVelMin){s.scene[i].vy=k_yVelMin;}
        }
        mh_move(i);
    }
    s.moveFrameY++;
    s.moveFrameX++;
}

void mh_move(int i){
    double yMove = s.scene[i].vy;
    double xMove = s.scene[i].vx;
    if(abs(s.scene[i].vy) < 1){
        if((s.moveFrameY %= k_moveFrames) == 0){yMove *= k_moveFrames;}
        else{goto AFTER_Y_MOTION;}
    }
    cl_move(i, 'y', yMove);
AFTER_Y_MOTION: ;

    if(abs(s.scene[i].vx) < 1){
        if((s.moveFrameX %= k_moveFrames) == 0){xMove *= k_moveFrames;}
        else{goto AFTER_X_MOTION;}
    }
    cl_move(i, 'x', xMove);
AFTER_X_MOTION: ;
}

bool mh_collision(int i1, int i2){
    bool ret;
    int nCols;
    int cols1, cols2;
    box b1 = s.scene[i1].bb;
    box b2 = s.scene[i2].bb;
    box b3;
    ob_realifyBox(&b1, s.scene[i1].x, s.scene[i1].y);
    ob_realifyBox(&b2, s.scene[i2].x, s.scene[i2].y);
    ret = k_boxInter(b1, b2);

    cols1 = ret;
    nCols = s.scene[i1].nCols;
    if(nCols>1){
    }
    for(int i=0;i<nCols;i++){
        b3 = s.scene[i1].cols[i];
        ob_realifyBox(&b3, s.scene[i1].x, s.scene[i1].y);
        if(k_boxInter(b3, b2) && s.scene[i2].physical){cols1 |= (int)pow(2, i+1);}
    }

    cols2 = ret;
    nCols = s.scene[i2].nCols;
    for(int i=0;i<nCols;i++){
        b3 = s.scene[i2].cols[i];
        ob_realifyBox(&b3, s.scene[i2].x, s.scene[i2].y);
        if(k_boxInter(b1, b3) && s.scene[i1].physical){cols2 |= (int)pow(2, i+1);}
    }
    if(cols2){mh_doCollision(&(s.scene[i1]), &(s.scene[i2]), cols1, cols2);}
    if(cols1){mh_doCollision(&(s.scene[i2]), &(s.scene[i1]), cols2, cols1);}

    return ret && s.scene[i1].physical && s.scene[i2].physical;
}

void mh_doCollision(obj* er, obj* ee, int colser, int colsee){
    switch((*er).type[0]){
        case '@':
            if(colser & 2){(*er).vx = 0;}
            if(colser & (4 | 8)){(*er).vy = 0;}
            if(colser & 8){s.onGround = true;}
            switch((*ee).type[0]){
                case 'C':
                    if(colsee & 2 && (*ee).i == act_nothing){
                        s.coins++;
                        (*ee).i = act_bounce;
                    }
                    break;
                case 'R':
                    if(colsee & 2 && (*ee).i == act_nothing){
                        (*ee).i = act_bounce;
                    }
                    break;
                case 'b':
                    if(colsee & 2 && (*ee).i == act_nothing){
                        (*ee).i = act_bounce;
                    }
                    break;
                case 'r':
                    if(s.bigMario == false){
                        s.bigMario = true;
                        (*er).y += 16;
                        (*er).bb.h -= 16;
                    }
                    break;
                case 'e':
                    if(colsee & 2){
                        (*ee)=ob_objFchar('.');
                        cl_smallJump();
                    }
                    else{s.gameOver=true;}
                    break;
                case '~':
                    s.gameOver = true;
            }
            break;
        case 'e':
            if(colser & (4 | 8)){
                (*er).vx = -(*er).vx;
            }
            break;
        case 'r':
            if((*ee).type[0] == '@'){
                *er = ob_objFchar('.');
            }
            else if(colser & (2 | 4)){
                (*er).vx = -(*er).vx;
            }
            break;
        case 'c':
            if((*ee).type[0] == '@'){
                s.coins++;
                *er = ob_objFchar('.');
            }
            break;
    }
}

