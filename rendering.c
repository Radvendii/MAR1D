#include "objects.h"
#include "helpers.h"
#include <math.h>
#define pi (22/7.0)

float orthoIntersectD(int y, line l){ //returns 0 if there's no intersection
    if(l.y1 == l.y2){return 0;}
    if( (l.y1 < y)==(l.y2 < y) ){return 0;}
    if(l.x1 == l.x2){return l.x1;}
    return (y - l.y1 - ob_slope(l) * l.x1 ) / ob_slope(l);
}

void rn_orthoTest(unsigned char* screen) {
    for(int i=0;i<k_nPixels*3;i++){ screen[i]=0; }
    float closeD, newD;
    line *l = salloc(sizeof(line));
    for(int y=0;y<k_nPixels;y++) {
        closeD = k_drawD;
        for(int obj=0;;obj++) {
            if(ob_levelTest[obj*3] == terminator) {break;} //Check for termination
            for(int line=0;;line++) {
                if(ob_isTerminating(objFtype(ob_levelTest[obj*3])[line])){break;}
                *l = objFtype(ob_levelTest[obj*3])[line];
                ob_realifyLine(l, (ob_levelTest + obj*3 +1));
                if( (newD = orthoIntersectD(y, *l)) == 0){continue;}
                if(newD < closeD){
                    closeD=newD;
                    screen[y*3 +0]=l->r;
                    screen[y*3 +1]=l->g;
                    screen[y*3 +2]=l->b;
                }
            }
        }
    }
    free(l);
    return;
}

void rn_perspTest(int camX, int camY, int camT, unsigned char *screen) {
    int camD = 500; //This is arbitrary, it just has to be set large enough so there aren't roundoff errors
    for(int i=0;i<k_nPixels*3;i++){ screen[i]=0; }
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
        for(int obj=0;;obj++) {
            if(ob_levelTest[obj*3] == terminator) {break;} //Check for termination
            for(int line=0;;line++) {
                if(ob_isTerminating(objFtype(ob_levelTest[obj*3])[line])){break;}
                l = objFtype(ob_levelTest[obj*3])[line];
                ob_realifyLine(&l, (ob_levelTest + obj*3 +1));
                if((newD = ob_intersectD(c, l) ) == 0) {continue;}
                if(newD < closeD) {
                    closeD=newD;
                    screen[y*3 +0]=l.r;
                    screen[y*3 +1]=l.g;
                    screen[y*3 +2]=l.b;
                }
            }
        }
    }
    return;
}

int rn_dimTest(line **renderArr){//returns the number of lines
    line l;
    int nLines=0;
    *renderArr = resalloc(*renderArr, sizeof(line) * k_nMaxLinesPerObj * k_nMaxObj);
    for(int obj=0;;obj++) {
        if(ob_levelTest[obj*3] == terminator) {break;} //Check for termination
        for(int line=0;;line++) {
            if(ob_isTerminating(objFtype(ob_levelTest[obj*3])[line])){break;}
            l = objFtype(ob_levelTest[obj*3])[line];
            ob_realifyLine(&l, (ob_levelTest + obj*3 +1));
            (*renderArr)[nLines++] = l;
        }
    }
    return nLines;
}

void rn_screenFworld(unsigned char *screen, struct world w){
    int camD = 500; //This is arbitrary, it just has to be set large enough so there aren't roundoff errors
    for(int i=0;i<k_nPixels*3;i++){ screen[i]=0; }
    double a = camD/cos((pi/180)*k_FOV/2);
    double xa = a*cos((pi/180)*w.camT);
    double ya = a*sin((pi/180)*w.camT);
    double beta = 90+k_FOV/2+w.camT;
    line c; //Holds the ray eminating from the camera. Calculated based on what pixel we're on
        c.x1 = w.camX;
        c.y1 = w.camY;
    float closeD, newD;
    line l;
    for(int y=0;y<k_nPixels;y++) {
        double y_ = y*2*camD*tan(pi/180*k_FOV/2.0)/k_nPixels; //unit conversion from pixels to "true distance"
        c.x2 = (int)(xa + y_*cos((pi/180)*beta));
        c.y2 = (int)(ya + y_*sin((pi/180)*beta));
        closeD=k_drawD*k_drawD;
        for(int obj=0;;obj++) {
            if(w.scene[obj*3] == terminator) {break;} //Check for termination
            for(int line=0;;line++) {
                l = objFtype(w.scene[obj*3])[line];
                if(ob_isTerminating(l)){break;}
                ob_realifyLine(&l, (w.scene + obj*3 +1));
                if((newD = ob_intersectD(c, l) ) == 0) {continue;}
                if(newD < closeD) {
                    closeD=newD;
                    screen[y*3 +0]=l.r;
                    screen[y*3 +1]=l.g;
                    screen[y*3 +2]=l.b;
                }
            }
        }
    }
    return;
}

