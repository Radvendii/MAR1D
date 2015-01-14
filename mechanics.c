#include "mechanics.h"
#include "helpers.h"
#include "objects.h"

/*
 *int mh_isCollision(struct world w, int i1, int i2, int which){
 *    if(i1 % 3 !=0 || i2 % 3 !=0){printf("Error in mh_isCollision(): indicies must be the beginning of object (i.e. %%3==0)");exit(1);}
 *    line l1, l2;
 *    line *o1 = objFtype(w.scene[i1]);
 *    line *o2 = objFtype(w.scene[i2]);
 *    for(int j1=0;;j1++){
 *        l1 = objFtype(w.scene[i1])[j1];
 *        if(ob_isTerminating(l1)){break;}
 *        ob_realifyLine(&l1, (w.scene + i1 + 1));
 *        for(int j2=0;;j2++){
 *            l2 = objFtype(w.scene[i2])[j2];
 *            if(ob_isTerminating(l2)){break;}
 *            ob_realifyLine(&l2, (w.scene + i2 + 1));
 *            if(ob_intersectIn(l1, l2) && which-- == 0){return j1;}
 *        }
 *    }
 *    return j1;
 *}
 */

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
