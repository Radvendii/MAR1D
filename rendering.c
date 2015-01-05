#include "objects.h"
#include <math.h>
#define pi (22/7.0)

line* realLine(line *objLine, int *pos){
    line* rl = malloc(sizeof(line));
    rl->x1 = objLine->x1 + pos[0];
    rl->x2 = objLine->x2 + pos[0];
    rl->y1 = objLine->y1 + pos[1];
    rl->y2 = objLine->y2 + pos[1];
    return rl;
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

int* orthoTest() {
    int *renderArr = malloc(sizeof(int) * k_nPixels);
    float closeD, newD;
    line *l;
    for(int y=0;y<k_nPixels;y++) {
        closeD = k_drawD;
        for(int obj=0;obj<k_nMaxObj;obj++) {
            for(int line=0;line<k_nMaxLinesPerObj;line++) {
                if(!g_isNotTerminating(&(objFtype(g_levelTest[obj*3])[line]))){break;}
                l = realLine(&(objFtype(g_levelTest[obj*3])[line]), (g_levelTest + obj*3 +1));
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

int main(int argc, char **argv) {
    line l = (line) {.x1 = 0, .y1=0, .x2=2, .y2=16};
    int *pos = malloc(sizeof(int)*2);
    pos[0] = 2;
    pos[1] = 2;
    int y = 1;
    g_printLine(realLine(&l, pos));
    printf("%f", orthoIntersectD(y, &l));
    /*
     *int *rendered = orthoTest();
     *for(int i=0;i<k_nPixels;i++) {
     *    printf("%d\n", rendered[i*3]);
     *}
     */

    return 0;
}

/*
 *double intersectD(int *c_, int *l_){
 *    double c[4];
 *    double l[4];
 *    double P[2];
 *    for(int i=0;i<4;i++){c[i]=(double)c_[i]; l[i]=(double)l_[i];}
 *    if(c_[0]==c_[2]) {
 *        if(l_[0]==l_[2]) {return 0;}
 *        P[0] = c[0];
 *        P[1] = (l[3]-l[1])/(l[2]-l[0])*(c[0]-l[0])+l[1];
 *    }
 *    else if(l_[0]==l_[2]) {
 *        P[0] = l[0];
 *        P[1] = (c[3]-c[1])/(c[2]-c[0])*(l[0]-c[0])+c[1];
 *    }
 *    else if((l[3]-l[1])/(l[2]-l[0])==(c[3]-c[1])/(c[2]-c[0])){
 *        return 0;
 *    }
 *    else {
 *        double ml = (l[3]-l[1])/(l[2]-l[0]);
 *        double mc = (c[3]-c[1])/(c[2]-c[0]);
 *        P[0] = (ml*l[0] - mc*c[0] + c[1] - l[1]) / (ml-mc);
 *        P[1] = (l[1]/ml - c[1]/mc + c[0] - l[0]) / (1/ml-1/mc);
 *    }
 *
 *    if( !((P[0]<l[0] == P[0]>l[2]) && (P[1]<l[1] == P[1]>l[3]) && (c[2]>c[0] == P[0]>c[2]) && (c[3]>c[1] == P[1]>c[3])) ) {return 0;}
 *
 *    return pow(P[0]-c[0], 2)+pow(P[1]-c[1], 2);
 *}
 *
 *
 *int* renderTest(int *renderArr, int camX, int camY, int camT, int camD) { //returns an array of k_nPixels colors
 *    double k0 = -cos(pi/180*(90-camT));
 *    double k1 = camD/cos(pi/180*k_FOV/2.0)*cos(pi/180*(camT+k_FOV/2.0));
 *    double k2 = sin(pi/180*(90-camT));
 *    double k3 = camD/cos(pi/180*k_FOV/2.0)*sin(pi/180*(camT+k_FOV/2.0));
 *    int camLine[4]; //Holds the ray eminating from the camera. Calculated based on what pixel we're on
 *        camLine[0] = camX;
 *        camLine[1] = camY;
 *    int lineLine[4];
 *    int closeD, newD;
 *    for(int y=0;y<k_nPixels;y++) {
 *        double y_ = y*2*camD*tan(pi/180*k_FOV/2.0)/k_nPixels; //unit conversion from pixels to "true distance"
 *        camLine[2] = (int)(camX+ y_*k0+k1);
 *        camLine[3] = (int)(camY+ y_*k2+k3);
 *        closeD=k_drawD;
 *        for(int obj=0;obj<k_nMaxObj;obj++) {
 *            for(int line=0;line<k_nMaxLinesPerObj;line++) {
 *                for(int i=0;i<4;i++){
 *                    lineLine[i] = *(objFtype(g_levelTest[obj*3]) +line*7 +i)+g_levelTest[obj*3 +(i%2+1)]; //accesses the $line-th line of the object in question, and adds the location of that object to the lines' locations
 *                }
 *[>if(y==250 && obj==0 && line==3){printf("%d, %d, %d, %d\n", camLine[0], camLine[1], camLine[2], camLine[3]);}<]
 *                if((newD = intersectD(camLine, lineLine) ) == 0) {continue;}
 *                if(newD < closeD) {
 *                    closeD=newD;
 *                    for(int i=0;i<3;i++) {
 *                        renderArr[y*3+i]=*(objFtype(g_levelTest[obj*3]) +line*7+4+i);
 *                    }
 *                }
 *            }
 *        }
 *    }
 *    return renderArr;
 *}
 */

