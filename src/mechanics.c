#include "mechanics.h"

void mh_init(){
  s.pipeTo = '\0';
  s.onGround = true;
  s.paused = false;
  s.won = false;
  s.gravity = k_gravity;
  s.upcount = 0;
}

void mh_update(){
  if((int)s.scene[s.pli].vy != 0){s.onGround = false; s.pipeTo = '\0';}
  for(int i=0;s.scene[i].type != '\0';i++){
    if(s.scene[i].i == ACT_nothing){continue;}
    if(s.scene[i].type == 'O' && --s.scene[i].i == ACT_nothing){cl_delObjAt(i);}
    if(s.scene[i].type == 'E' && --s.scene[i].i == ACT_nothing){cl_delObjAt(i);}
    if(s.scene[i].type == '7'){
      if(s.scene[i].vx == 0 && --s.scene[i].i == ACT_nothing){
        int x_temp = s.scene[i].x;
        int y_temp = s.scene[i].y+16;
        s.scene[i] = ob_objFchar('&');
        s.scene[i].x = x_temp;
        s.scene[i].y = y_temp;
        s.scene[i].vx = -0.5;
      }
      if(s.scene[i].j){s.scene[i].j--;}
    }
    if(s.scene[i].i <= ACT_bounce && (s.scene[i].type == '?' || s.scene[i].type == '#' || s.scene[i].type == '3')){
      if(s.bigMario && (s.scene[i].type == '#' || s.scene[i].type == '3') && s.scene[i].i == ACT_bounce-1){
        au_play(SND_blockbreak);
        s.score += 50;
        int l;
        for(l=0;s.scene[l].type != '\0';l++);
        for(int j=0;j<2;j++){
          for(int k=0;k<2;k++){
            s.scene[l+j+k*2] = ob_objFchar(s.scene[i].type == '#' ? 'b' : 'B');
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
      s.scene[i].y -= (s.scene[i].i < ACT_bounceDn && s.scene[i].i > ACT_bounceUp) * 2 - 1;
    }
    if(s.scene[i].type == '?' && s.scene[i].i == ACT_nothing){
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
        if(s.scene[i].c == 'R'){
          au_play(SND_fireflowerappear);
        }
        if(s.scene[i].c != 'R'){
          s.scene[l].vx = 0.5;
        }
        s.scene[l+1].type = '\0';
      }
    }
    if(s.scene[i].type == '?' && s.scene[i].i == ACT_nothing && s.scene[i].j == 0){
      obj temp = s.scene[i];
      s.scene[i] = ob_objFchar('D');
      s.scene[i].x = temp.x;
      s.scene[i].y = temp.y;
      s.scene[i].i = temp.i;
    }
    if(s.scene[i].type == '@'){
      // TODO: seems like this logic could be simplified
      int grow[12] = {0,1,-1,1,-1,1,1,-2,1,1,-2,2};
      if(s.scene[i].i && s.bigMario == true && s.scene[i].i < 12*k_growthRate){
        s.paused = true;
        int growth = 8*grow[s.scene[i].i/k_growthRate] * !(s.scene[i].i % k_growthRate);
        s.scene[s.pli].y += growth;
        s.scene[s.pli].bb.h -= growth;
        s.scene[s.pli].cols[0].h -= growth;
        s.scene[s.pli].cols[2].y -= growth;
        if(++s.scene[i].i == 12 * k_growthRate){s.paused = false;}
      }
      if(s.scene[i].i > 0 && s.bigMario == false){
        s.paused = true;
        int growth = -8*grow[12 - s.scene[i].i/k_growthRate] * !(s.scene[i].i % k_growthRate);
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
  if(s.dead){
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
}

void mh_move(int i){
  double yMove = s.scene[i].vy + s.scene[i].yMantissa;
  double xMove = s.scene[i].vx + s.scene[i].xMantissa;
  s.scene[i].yMantissa = yMove - (int) yMove;
  s.scene[i].xMantissa = xMove - (int) xMove;

  cl_move(i, 'y', (int) yMove);
  cl_move(i, 'x', (int) xMove);
}

bool mh_collision(int i1, int i2){
  if(s.scene[i1].type == '.' || s.scene[i2].type == '.' || s.dead){return false;}
  bool ret;
  int cols1, cols2;
  box b1 = s.scene[i1].bb;
  box b2 = s.scene[i2].bb;
  box b3;
  ob_realifyBox(&b1, s.scene[i1].x, s.scene[i1].y);
  ob_realifyBox(&b2, s.scene[i2].x, s.scene[i2].y);
  ret = k_boxInter(b1, b2);
  if(s.scene[i1].type == '@' && s.scene[i2].type == 'E'){return false;}
  if(s.scene[i1].type == '@' && s.invincible && (s.scene[i2].type == 'e' || s.scene[i2].type == 'E' || s.scene[i2].type == '&' || s.scene[i2].type == '7')){return false;}
  if(s.scene[i2].type == '@' && s.invincible && (s.scene[i1].type == 'e' || s.scene[i1].type == 'E' || s.scene[i1].type == '&' || s.scene[i1].type == '7')){return false;}

  cols1 = ret;
  for(int i=0;i < s.scene[i1].nCols;i++){
    b3 = s.scene[i1].cols[i];
    ob_realifyBox(&b3, s.scene[i1].x, s.scene[i1].y);
    if(k_boxInter(b3, b2) && s.scene[i2].physical){cols1 |= 1 << (i+1);}
  }

  cols2 = ret;
  for(int i=0;i < s.scene[i2].nCols;i++){
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
    // if we're star or invincible, we pass through enemies
    if(!((s.star || s.invincible) && ((*ee).type == 'e' || (*ee).type == '&' || (*ee).type == 'E' || (*ee).type == '7'))){
      if(colser & 2){(*er).vx = 0;}
      if(colser & 4){
        cl_jumpEnd();
        (*er).vy = 0;
      }
      if(colser & 8){s.onGround = true;}
    }
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
      if(colsee & (2|4)){
        if (!s.pipeTo) {
          au_play(SND_pipe);
        }
        s.pipeTo = (*ee).c;
        if(s.time % 3){
          if((*ee).c == '\0'){(*er).x--;}
          else{(*er).x++;}
        }
      }
      if(colsee & 8 && (*ee).c != '\0'){
        cl_pipe();
      }
      break;
    case '?':
      if(colsee & 2 && (*ee).i == ACT_nothing){
        (*ee).i = ACT_bounce; // coin-producing code is in mh_update()
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
    case '3':
      if(colsee & 2 && (*ee).i == ACT_nothing){
        if(!s.bigMario){
          au_play(SND_blockhit);
        }
        (*ee).i = ACT_bounce; //brick-breaking code is in mh_update()
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
    //TODO: deal with 'e' and '&' together. the logic is identical
    case 'e':
      if((*ee).physical == true){
        if(s.star){
          ai_kill(ee);
          cl_score((*er).combo++);
        }
        // The hitboxes are positioned such that if you walk into them from the
        // side, you will hit both the top and the side box. But if you come
        // down from above, you will only hit the top box. This logic reflects
        // that.
        else if(colsee & (4|8)){
          gl_killed();
        }
        else if(colsee & 2){
          int x_temp = (*ee).x;
          int y_temp = (*ee).y-8;
          (*ee) = ob_objFchar('E');
          (*ee).x = x_temp;
          (*ee).y = y_temp;
          (*ee).i = k_corpseLife;
          au_play(SND_goombastomp);
          cl_smallJump();
          cl_score((*er).combo++);
        }
      }
      break;
    case '&':
      if((*ee).physical == true){
        if(s.star){
          ai_kill(ee);
          cl_score((*er).combo++);
        }
        // The hitboxes are positioned such that if you walk into them from the
        // side, you will hit both the top and the side box. But if you come
        // down from above, you will only hit the top box. This logic reflects
        // that.
        else if(colsee & (4|8)){
          gl_killed();
        }
        else if(colsee & 2) {
          int x_temp = (*ee).x;
          int y_temp = (*ee).y-8;
          (*ee) = ob_objFchar('7');
          (*ee).x = x_temp;
          (*ee).y = y_temp;
          (*ee).i = k_shellLife;
          au_play(SND_koopastomp);
          cl_smallJump();
          cl_score((*er).combo++);
        }
      }
      break;
    case '7':
      if(s.star){
        ai_kill(ee);
        cl_score((*er).combo++);
      }
      bool side = colsee & (8 | 16);
      bool top = colsee & (2 | 4) && !side;
      bool moving = (*ee).vx;
      if(moving && !(*ee).j){ // j marks if it's just been bopped
        if(side){
          gl_killed();
        }
        else if(top){
          int x_temp = (*ee).x;
          int y_temp = (*ee).y;
          (*ee) = ob_objFchar('7');
          (*ee).x = x_temp;
          (*ee).y = y_temp;
          (*ee).i = k_shellLife;
          cl_smallJump();
          cl_score((*er).combo++);
          (*ee).combo = 2; // reset combo
        }
      }
      else if(top || side){
        int dir = !!(colsee & 2) - !!(colsee & 4);
        au_play(SND_shot);
        (*ee).vx = k_xVelRunMax * dir; // can't catch it
        if (top) {
          (*er).vx = -1.0 * dir;
        }
        (*ee).nFrames = 1; // stop the animation
        (*ee).combo = 2; // koopa shell scores start at 500
        // prevent immediate repeated collisions
        (*ee).j = 5; // TODO: arbitrary magic number
      }
      break;
    case '!':
      s.won = true;
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
    if((*er).i != ACT_nothing){
      if(((*ee).type == 'e' || (*ee).type == '&') && (*ee).physical == true){
        ai_kill(ee);
        cl_score((*er).combo++);
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
    case 'e':
      if((*ee).physical == true){
        ai_kill(ee);
        cl_score((*er).combo++);
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
    if(((*ee).type == 'e' || (*ee).type == '&' || (*ee).type == '7') && (*ee).physical == true){
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
    if(colser & (2 | 4) && (*ee).type != 'e' && (*ee).type != '&' && (*ee).type != 'E' && (*ee).type != '7'){
      (*er).vx = -(*er).vx;
    }
    else if(colser & 8 && (*ee).type != 'e' && (*ee).type != '&' && (*ee).type != 'E' && (*ee).type != '7'){
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
      cl_oneUp();
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
