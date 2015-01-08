#include "objects.h"
#include <math.h>
#define pi (22/7.0)

line* realLine(line *objLine, int *pos){
    line* rl = malloc(sizeof(line));
    rl->x1 = objLine->x1 + pos[0];
    rl->x2 = objLine->x2 + pos[0];
    rl->y1 = objLine->y1 + pos[1];
    rl->y2 = objLine->y2 + pos[1];
    rl->r = objLine->r;
    rl->g = objLine->g;
    rl->b = objLine->b;
    return rl;
}

float f_round(float f){
    return( (((int) (f*10 + ((f > 0) ? 0.5: -0.5))))/10.0);
}

float slope(line *l){
    if(l->x1==l->x2){printf("Can't find slope of line with Dx 0"); exit(1);}
    return ( (float) l->y2 - l->y1) / (l->x2 - l->x1);
}

float orthoIntersectD(int y, line* l){ //returns 0 if there's no intersection
    if(l->y1 == l->y2){return 0;}
    if( (l->y1 < y)==(l->y2 < y) ){return 0;}
    if(l->x1 == l->x2){return l->x1;}
    return (y - l->y1 - slope(l) * l->x1 ) / slope(l);
}

float* intersect(line *l1, line* l2){ //returns NULL if there's no intersection. Edge cases are whatever.
    if( (l1->y1 == l1->y2 && l1->x1 == l1->x2) || (l2->y1 == l2->y2 && l2->x1 == l2->x2) ) {printf("This is not a line"); exit(1);}
    float d;
    if( f_round(d = (l1->x1-l1->x2)*(l2->y1-l2->y2) - (l1->y1-l1->y2)*(l2->x1-l2->x2)) == 0 ) {return NULL;}

    float x=( (l1->x1*l1->y2-l1->y1*l1->x2)*(l2->x1-l2->x2) - (l1->x1-l1->x2)*(l2->x1*l2->y2-l2->y1*l2->x2) )/d;
    float y=( (l1->x1*l1->y2-l1->y1*l1->x2)*(l2->y1-l2->y2) - (l1->y1-l1->y2)*(l2->x1*l2->y2-l2->y1*l2->x2) )/d;
    float *p = malloc(sizeof(float)*2);
    p[0] = x;
    p[1] = y;
    return p;
}

float intersectD(line *c, line* l){ //returns 0 if there's no intersection. Edge cases are whatever.
    float *p = intersect(c, l);
    if(p == NULL){return 0;}
    float x = p[0];
    float y = p[1];

    if(x<l->x1 == x<l->x2 && l->x1!=l->x2){return 0;}
    if(y<l->y1 == y<l->y2 && l->y1!=l->y2){return 0;}
    if(c->x1<x != c->x1<c->x2){return 0;}
    if(c->y1<y != c->y1<c->y2){return 0;}

    return pow(x-c->x1, 2) + pow(y-c->y1, 2);
}

unsigned char* orthoTest() {
    unsigned char *renderArr = malloc(sizeof(unsigned char) * k_nPixels * 3);
    for(int i=0;i<k_nPixels*3;i++){ renderArr[i]=0; }
    float closeD, newD;
    line *l;
    for(int y=0;y<k_nPixels;y++) {
        closeD = k_drawD;
        for(int obj=0;obj<k_nMaxObj;obj++) {
            if(ob_levelTest[obj*3] == terminator) {break;} //Check for termination
            for(int line=0;line<k_nMaxLinesPerObj;line++) {
                if(ob_isTerminating(&(objFtype(ob_levelTest[obj*3])[line]))){break;}
                l = realLine(&(objFtype(ob_levelTest[obj*3])[line]), (ob_levelTest + obj*3 +1));
                if( (newD = orthoIntersectD(y, l)) == 0){continue;}
                if(newD < closeD){
                    closeD=newD;
                    renderArr[y*3 +0]=l->r;
                    renderArr[y*3 +1]=l->g;
                    renderArr[y*3 +2]=l->b;
                }
            }
        }
    }
    return renderArr;
}

unsigned char* renderTest(int camX, int camY, int camT) {
    int camD = 500; //This is arbitrary, it just has to be set large enough so there aren't roundoff errors
    unsigned char *renderArr = malloc(sizeof(unsigned char)*k_nPixels*3);
    for(int i=0;i<k_nPixels*3;i++){ renderArr[i]=0; }
    double a = camD/cos((pi/180)*k_FOV/2);
    double xa = a*cos((pi/180)*camT);
    double ya = a*sin((pi/180)*camT);
    double beta = 90+k_FOV/2+camT;
    line c; //Holds the ray eminating from the camera. Calculated based on what pixel we're on
        c.x1 = camX;
        c.y1 = camY;
    float closeD, newD;
    line l;
    for(int y=0;y<k_nPixels;y++) {
        double y_ = y*2*camD*tan(pi/180*k_FOV/2.0)/k_nPixels; //unit conversion from pixels to "true distance"
        c.x2 = (int)(xa + y_*cos((pi/180)*beta));
        c.y2 = (int)(ya + y_*sin((pi/180)*beta));
        closeD=k_drawD*k_drawD;
        for(int obj=0;obj<k_nMaxObj;obj++) {
            if(ob_levelTest[obj*3] == terminator) {break;} //Check for termination
            for(int line=0;line<k_nMaxLinesPerObj;line++) {
                if(ob_isTerminating(&(objFtype(ob_levelTest[obj*3])[line]))){break;}
                l = *realLine(&(objFtype(ob_levelTest[obj*3])[line]), (ob_levelTest + obj*3 +1));
                if((newD = intersectD(&c, &l) ) == 0) {continue;}
                if(newD < closeD) {
                    closeD=newD;
                    renderArr[y*3 +0]=l.r;
                    renderArr[y*3 +1]=l.g;
                    renderArr[y*3 +2]=l.b;
                }
            }
        }
    }
    return renderArr;
}

