#include "mechanics.h"

void mh_init(){
    s.pipeTo = '\0';
    s.onGround = true;
    s.paused = false;
    s.won = false;
    s.gravity = k_gravity;
    s.upcount = 0;
    s.moveFrameY = 0;
    s.moveFrameX = 0;
}

void mh_update(){
    if((int)s.scene[s.pli].vy != 0){s.onGround = false; s.pipeTo = '\0';}
    for(int i=0;s.scene[i].type != '\0';i++){
        if(s.scene[i].i == act_nothing){continue;}
        if(s.scene[i].type == 'O' && s.scene[i].i-- == 1){cl_delObjAt(i);}
        if(s.scene[i].type == 'E' && s.scene[i].i-- == 1){cl_delObjAt(i);}
        if(s.scene[i].type == '7' && s.scene[i].i-- == 1 && s.scene[i].vx == 0){
            int x_temp = s.scene[i].x;
            int y_temp = s.scene[i].y+16;
            s.scene[i] = ob_objFchar('&');
            s.scene[i].x = x_temp;
            s.scene[i].y = y_temp;
            s.scene[i].vx = -0.5;
        }
        if(s.scene[i].i <= act_bounce && (s.scene[i].type == '?' || s.scene[i].type == '#')){
            if(s.bigMario && s.scene[i].type == '#' && s.scene[i].i == act_bounce-1){
                au_play(SND_blockbreak);
                s.score += 50;
                int l;
                for(l=0;s.scene[l].type != '\0';l++);
                for(int j=0;j<2;j++){
                    for(int k=0;k<2;k++){
                        s.scene[l+j+k*2] = ob_objFchar('b');
                        s.scene[l+j+k*2].x = s.scene[i].x+j*8;
                        s.scene[l+j+k*2].y = s.scene[i].y+k*8;
                        s.scene[l+j+k*2].vx = (j*2-1);
                        s.scene[l+j+k*2].vy = 1+(k);
                    }
                }
                s.scene[l+4].type = '\0';
                cl_delObjAt(i);
            }
            s.scene[i].i--;
            s.scene[i].y -= (s.scene[i].i < act_bounceD && s.scene[i].i > act_bounceU) * 2 - 1;
        }
        if(s.scene[i].type == '?' && s.scene[i].i == act_nothing){
            int l;
            if(s.scene[i].j){s.scene[i].j--;}
            if(s.scene[i].c == 'c'){
                s.coins++;
                au_play(SND_coin);
                s.score += 200;
            }
            else{
                for(l=0;s.scene[l].type != '\0';l++);
                s.scene[l] = ob_objFchar(s.scene[i].c);
                s.scene[l].x = s.scene[i].x;
                s.scene[l].y = s.scene[i].y+16;
                if(s.scene[i].c == 'r' || s.scene[i].c == 'g'){
                    au_play(SND_mushroomappear);
                }
                if(s.scene[i].c != 'R'){
                    s.scene[l].vx = 0.5;
                }
                s.scene[l+1].type = '\0';
            }
        }
        if(s.scene[i].type == '?' && s.scene[i].i == act_nothing && s.scene[i].j == 0){
            obj temp = s.scene[i];
            s.scene[i] = ob_objFchar('D');
            s.scene[i].x = temp.x;
            s.scene[i].y = temp.y;
            s.scene[i].i = temp.i;
        }
        if(s.scene[i].type == '@'){
            if(s.scene[i].i && s.bigMario == true && s.scene[i].i < 12*k_growthRate){
                s.paused = true;
                int grow[12] = {0,1,-1,1,-1,1,1,-2,1,1,-2,2};
                int growth = 8*grow[s.scene[i].i/k_growthRate] * !(s.scene[i].i % k_growthRate);
                s.scene[s.pli].y += growth;
                s.scene[s.pli].bb.h -= growth;
                s.scene[s.pli].cols[0].h -= growth;
                s.scene[s.pli].cols[2].y -= growth;
                if(++s.scene[i].i == 12 * k_growthRate){s.paused = false;}
            }
            if(s.scene[i].i > 0 && s.bigMario == false){
                s.paused = true;
                int grow[12] = {0,-1,1,-1,1,-1,-1,2,-1,-1,2,-2};
                int growth = 8*grow[s.scene[i].i/k_growthRate] * !(s.scene[i].i % k_growthRate);
                s.scene[s.pli].y += growth;
                s.scene[s.pli].bb.h -= growth;
                s.scene[s.pli].cols[0].h -= growth;
                s.scene[s.pli].cols[2].y -= growth;
                if(--s.scene[i].i == 0){s.paused = false;}
            }
        }
    }
    if(!s.paused){
        for(int i=0;s.scene[i].type != '\0';i++){
            if(s.scene[i].active){
                cl_gravity(i);
                mh_move(i);
            }
        }
    }
    else if(s.dead){
        s.scene[s.pli].vx=0;
        if(!(--s.dead)){
            gl_resetLevel();
        }
        else if(s.dead<k_dieStartBlack){
            s.scene[s.pli].hidden = false;
        }
        else if(s.dead<k_dieStartMoving){
            cl_gravity(s.pli);
            mh_move(s.pli);
        }
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
    if(s.scene[i1].type == '.' || s.scene[i2].type == '.' || s.dead){return false;}
    bool ret;
    int nCols;
    int cols1, cols2;
    box b1 = s.scene[i1].bb;
    box b2 = s.scene[i2].bb;
    box b3;
    ob_realifyBox(&b1, s.scene[i1].x, s.scene[i1].y);
    ob_realifyBox(&b2, s.scene[i2].x, s.scene[i2].y);
    ret = k_boxInter(b1, b2);
    if(s.scene[i1].type == '@' && s.scene[i2].type == 'E'){return false;}
    if(s.scene[i1].type == '@' && s.invincible && (s.scene[i2].type == 'e' || s.scene[i2].type == '&' || s.scene[i2].type == '7')){return false;}
    if(s.scene[i2].type == '@' && s.invincible && (s.scene[i1].type == 'e' || s.scene[i1].type == '&' || s.scene[i1].type == '7')){return false;}

    cols1 = ret;
    nCols = s.scene[i1].nCols;
    if(nCols>1){
    }
    for(int i=0;i<nCols;i++){
        b3 = s.scene[i1].cols[i];
        ob_realifyBox(&b3, s.scene[i1].x, s.scene[i1].y);
        if(k_boxInter(b3, b2) && s.scene[i2].physical){cols1 |= 1 << (i+1);}
    }

    cols2 = ret;
    nCols = s.scene[i2].nCols;
    for(int i=0;i<nCols;i++){
        b3 = s.scene[i2].cols[i];
        ob_realifyBox(&b3, s.scene[i2].x, s.scene[i2].y);
        if(k_boxInter(b1, b3) && s.scene[i1].physical){cols2 |= 1 << (i+1);}
    }
    if(cols2){mh_doCollision(&(s.scene[i1]), &(s.scene[i2]), cols1, cols2);}
    if(cols1){mh_doCollision(&(s.scene[i2]), &(s.scene[i1]), cols2, cols1);}

    return (ret && s.scene[i1].physical && s.scene[i2].physical) || (s.scene[i1].x<s.leftMost && s.scene[i1].type == '@');
}

void mh_doCollision(obj* er, obj* ee, int colser, int colsee){
    double vy;
    switch((*er).type){
        case '@':
            vy = (*er).vy;
            if(!((s.star || s.invincible) && ((*er).type == '@' && ((*ee).type == 'e' || (*ee).type == '&')))){
                if(colser & 2){(*er).vx = 0;}
            }
            if(colser & (4 | 8)){(*er).vy = 0;}
            if(colser & 8){s.onGround = true;}
            switch((*ee).type){
                //case '`': //TODO: checkpoints
                    //s.check = '1';
                    //break;
                case '=':
                    if(colsee & 2){
                        s.pipeTo = (*ee).c;
                    }
                    if(colsee & 4){
                        if(s.time % 3){
                            if((*ee).c == '\0'){(*er).y++;}
                            else{(*er).y--;}
                        }
                    }
                    if(colsee & 8 && (*ee).c != '\0'){
                        cl_pipe();
                    }
                    break;
                case ']':
                    //if(colsee & 2){
                        //s.pipeTo = (*ee).c;
                        //(*er).x++;
                    //}
                    if(colsee & (2|4)){
                        s.pipeTo = (*ee).c;
                        if(s.time % 3){
                            if((*ee).c == '\0'){(*er).x--;}
                            else{(*er).x++;}
                        }
                    }
                    if(colsee & 8 && (*ee).c != '\0'){
                        au_play(SND_pipe);
                        cl_pipe();
                    }
                    break;
                case '?':
                    if(colsee & 2 && (*ee).i == act_nothing){
                        (*ee).i = act_bounce; //coin-producing code is in mh_update()
                        (*ee).hidden = false;
                    }
                    if(colsee & 4 && !(colsee & (8 | 2))){
                        (*er).x++;
                        (*er).vy = vy;
                    }
                    if(colsee & 8 && !(colsee & (4 | 2))){
                        (*er).x--;
                        (*er).vy = vy;
                    }
                    break;
                case 'D':
                    if(colsee & 2 && colsee & 4 ){
                        au_play(SND_blockhit);
                    }
                    if(colsee & 2 && !(colsee & 4)){
                        (*er).vx = 0;
                        (*er).x++;
                        (*er).vy = vy;
                    }
                    if(colsee & 4 && !(colsee & 2)){
                        (*er).vx = 0;
                        (*er).x--;
                        (*er).vy = vy;
                    }
                    break;
                case '#':
                    if(colsee & 2 && (*ee).i == act_nothing){
                        if(!s.bigMario){
                            au_play(SND_blockhit);
                        }
                        (*ee).i = act_bounce; //brick-breaking code is in mh_update()
                    }
                    if(colsee & 4 && !(colsee & (8 | 2))){
                        (*er).vx = 0;
                        (*er).x++;
                        (*er).vy = vy;
                    }
                    if(colsee & 8 && !(colsee & (4 | 2))){
                        (*er).vx = 0;
                        (*er).x--;
                        (*er).vy = vy;
                    }
                    break;
                case 'e':
                    if((*ee).physical == true){
                        if(s.star){
                            ai_kill(ee);
                            s.score+=100;
                        }
                        else if(colsee & 2){
                            int x_temp = (*ee).x;
                            int y_temp = (*ee).y-8;
                            (*ee) = ob_objFchar('E');
                            (*ee).x = x_temp;
                            (*ee).y = y_temp;
                            (*ee).i = k_corpseLife;
                            cl_smallJump();
                            s.score+=100*s.multibounce;
                        }
                        else{gl_killed();}
                    }
                    break;
                case '&':
                    if(s.star){
                        ai_kill(ee);
                        s.score+=100;
                    }
                    else if(colsee & 2){
                        int x_temp = (*ee).x;
                        int y_temp = (*ee).y-8;
                        (*ee) = ob_objFchar('7');
                        (*ee).x = x_temp;
                        (*ee).y = y_temp;
                        (*ee).i = k_shellLife;
                        cl_smallJump();
                        s.score+=100*s.multibounce;
                    }
                    else{gl_killed();}
                    break;
                case '7':
                    if(colsee & 2){
                        au_play(SND_shot);
                        (*ee).vx = 2.0;
                        (*er).vx = -0.5;
                        (*ee).nps = 1;
                        s.score += 400+100*s.multibounce;
                    }
                    if(colsee & 4){
                        au_play(SND_shot);
                        (*ee).vx = -2.0;
                        (*er).vx = 0.5;
                        (*ee).nps = 1;
                        s.score += 400+100*s.multibounce;
                    }
                    if(!(colsee & (4|2))){
                        if(s.star){
                            ai_kill(ee);
                            s.score += 200;
                        }
                        else{gl_killed();}
                    }
                    break;
                case '!':
                    if(!s.won){
                        gl_win();
                    }
                    break;
                case '~':
                    gl_die();
                    break;
            }
            break;
        case '~':
            if((*ee).type != '@'){
                cl_delObj(ee);
            }
            break;
        case '#':
            ;
        case '?':
            if((*er).i != act_nothing){
                if(((*ee).type == 'e' || (*ee).type == '&') && (*ee).physical == true){
                    ai_kill(ee);
                    s.score += 100;
                }
                if((*ee).type == 'r' || (*ee).type == 'g'){
                    (*ee).vx = -(*ee).vx;
                    (*ee).vy = 4;
                }
            }
            break;
        case 'e':
            if((*er).physical == true && colser & (4 | 8)){
                (*er).vx = -(*er).vx;
            }
            break;
        case '&':
            if((*er).physical == true && colser & (4 | 8)){
                (*er).vx = -(*er).vx;
                (*er).flip = !(*er).flip;
            }
            break;
        case '7':
            switch((*ee).type){
                case '&':
                    ;
                case 'e':
                    if((*ee).physical == true){
                        ai_kill(ee);
                    }
                    break;
                default:
                    if(colser & (8 | 16)){
                        (*er).vx = -(*er).vx;
                    }
                    break;
            }
            break;
        case 'o':
            if(((*ee).type == 'e' || (*ee).type == '&') && (*ee).physical == true){
                ai_kill(ee);
                int x_temp = (*er).x;
                int y_temp = (*er).y;
                (*er) = ob_objFchar('O');
                (*er).x = x_temp;
                (*er).y = y_temp;
                (*er).i = 2*10;
                s.score += (*ee).type == 'e' ? 100 : 200;
            }
            if(colser & (2 | 4) && (*ee).type != '@'){
                int x_temp = (*er).x;
                int y_temp = (*er).y;
                (*er) = ob_objFchar('O');
                (*er).x = x_temp;
                (*er).y = y_temp;
                (*er).i = 2*10;
            }
            else if(colser & 8 && (*ee).type != '@' && (*er).type != 'O'){
                (*er).y += 1;
                (*er).vy = 5;
            }
            break;
        case 's':
            if((*ee).type == '@'){
                cl_delObj(er);
                cl_starman();
                s.score += 1000;
            }
            if(colser & (2 | 4) && (*ee).type != 'e' && (*ee).type != '&'){
                (*er).vx = -(*er).vx;
            }
            else if(colser & 8 && (*ee).type != 'e' && (*ee).type != '&'){
                (*er).y += 1;
                (*er).vy = 6;
            }
            break;
        case 'R':
            if((*ee).type == '@'){
                cl_delObj(er);
                cl_fireMario();
                s.score += 1000;
            }
            break;
        case 'r':
            if((*ee).type == '@'){
                cl_delObj(er);
                cl_bigMario();
                s.score += 1000;
            }
            else if(colser & (2 | 4)){
                (*er).vx = -(*er).vx;
            }
            break;
        case 'g':
            if((*ee).type == '@'){
                cl_delObj(er);
                au_play(SND_oneup);
                s.lives++;
                s.score+= 1000;
            }
            else if(colser & (2 | 4)){
                (*er).vx = -(*er).vx;
            }
            break;
        case 'c':
            if((*ee).type == '@'){
                s.coins++;
                au_play(SND_coin);
                s.score += 200;
                cl_delObj(er);
            }
            break;
    }
}

