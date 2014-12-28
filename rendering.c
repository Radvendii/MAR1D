#include "objects.h"
#include <math.h>
#define pi (22/7.0)

int* orthoTest(int *renderArr, int camX) {
    int closeD;
    int l[4];
    for(int y=0;y<k_nPixels;y++) {
        closeD = k_drawD;
        for(int obj=0;obj<k_nMaxObj;obj++) {
            for(int line=0;line<k_nMaxLinesPerObj;line++) {
                for(int i=0;i<4;i++){
                    l[i] = *(objFtype(g_levelTest[obj*3]) +line*7 +i) + g_levelTest[obj*3 +(i%2+1)];
                }
                if(l[1]==l[3]){ continue; }
                else{newD = y-l[1]-l[3]-l[1]
            }
        }
    }
}

/*double intersectD(int *c_, int *l_){*/
    /*double c[4];*/
    /*double l[4];*/
    /*double P[2];*/
    /*for(int i=0;i<4;i++){c[i]=(double)c_[i]; l[i]=(double)l_[i];}*/
    /*if(c_[0]==c_[2]) {*/
        /*if(l_[0]==l_[2]) {return 0;}*/
        /*P[0] = c[0];*/
        /*P[1] = (l[3]-l[1])/(l[2]-l[0])*(c[0]-l[0])+l[1];*/
    /*}*/
    /*else if(l_[0]==l_[2]) {*/
        /*P[0] = l[0];*/
        /*P[1] = (c[3]-c[1])/(c[2]-c[0])*(l[0]-c[0])+c[1];*/
    /*}*/
    /*else if((l[3]-l[1])/(l[2]-l[0])==(c[3]-c[1])/(c[2]-c[0])){*/
        /*return 0;*/
    /*}*/
    /*else {*/
        /*double ml = (l[3]-l[1])/(l[2]-l[0]);*/
        /*double mc = (c[3]-c[1])/(c[2]-c[0]);*/
        /*P[0] = (ml*l[0] - mc*c[0] + c[1] - l[1]) / (ml-mc);*/
        /*P[1] = (l[1]/ml - c[1]/mc + c[0] - l[0]) / (1/ml-1/mc);*/
    /*}*/

    /*if( !((P[0]<l[0] == P[0]>l[2]) && (P[1]<l[1] == P[1]>l[3]) && (c[2]>c[0] == P[0]>c[2]) && (c[3]>c[1] == P[1]>c[3])) ) {return 0;}*/

    /*return pow(P[0]-c[0], 2)+pow(P[1]-c[1], 2);*/
/*}*/


/*int* renderTest(int *renderArr, int camX, int camY, int camT, int camD) { //returns an array of k_nPixels colors*/
    /*double k0 = -cos(pi/180*(90-camT));*/
    /*double k1 = camD/cos(pi/180*k_FOV/2.0)*cos(pi/180*(camT+k_FOV/2.0));*/
    /*double k2 = sin(pi/180*(90-camT));*/
    /*double k3 = camD/cos(pi/180*k_FOV/2.0)*sin(pi/180*(camT+k_FOV/2.0));*/
    /*int camLine[4]; //Holds the ray eminating from the camera. Calculated based on what pixel we're on*/
        /*camLine[0] = camX;*/
        /*camLine[1] = camY;*/
    /*int lineLine[4];*/
    /*int closeD, newD;*/
    /*for(int y=0;y<k_nPixels;y++) {*/
        /*double y_ = y*2*camD*tan(pi/180*k_FOV/2.0)/k_nPixels; //unit conversion from pixels to "true distance"*/
        /*camLine[2] = (int)(camX+ y_*k0+k1);*/
        /*camLine[3] = (int)(camY+ y_*k2+k3);*/
        /*closeD=k_drawD;*/
        /*for(int obj=0;obj<k_nMaxObj;obj++) {*/
            /*for(int line=0;line<k_nMaxLinesPerObj;line++) {*/
                /*for(int i=0;i<4;i++){*/
                    /*lineLine[i] = *(objFtype(g_levelTest[obj*3]) +line*7 +i)+g_levelTest[obj*3 +(i%2+1)]; //accesses the $line-th line of the object in question, and adds the location of that object to the lines' locations*/
                /*}*/
/*[>if(y==250 && obj==0 && line==3){printf("%d, %d, %d, %d\n", camLine[0], camLine[1], camLine[2], camLine[3]);}<]*/
                /*if((newD = intersectD(camLine, lineLine) ) == 0) {continue;}*/
                /*if(newD < closeD) {*/
                    /*closeD=newD;*/
                    /*for(int i=0;i<3;i++) {*/
                        /*renderArr[y*3+i]=*(objFtype(g_levelTest[obj*3]) +line*7+4+i);*/
                    /*}*/
                /*}*/
            /*}*/
        /*}*/
    /*}*/
    /*return renderArr;*/
/*}*/

int main(int argc, char **argv) {
    /*int c[4];*/
    /*int l[4];*/
    /*for(int i=0;i<4;i++){c[i]=atoi(argv[i+1]);l[i]=atoi(argv[i+5]); printf("%d,%d\n", c[i], l[i]);}*/
    /*printf("%f", intersectD(c, l));*/

    int rendered[k_nPixels*3] = {};
    int *renderptr = renderTest(rendered, 0,0,0,10);
    for(int i=0;i<k_nPixels;i++) {
        printf("%d\n", *(renderptr+i*3));
    }
    return 0;
}
