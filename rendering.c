#include "rendering.h"

int rn_dimFworld_old(line **renderArr, struct world w){//returns the number of lines
    line l;
    int nLines=0;
    *renderArr = resalloc(*renderArr, sizeof(line) * (k_nMaxLinesPerObj * k_nMaxObj + 1000));
    int camD = 30;
    double a = camD/cos((pi/180)*k_FOV/2);
    double xa = w.camX + a*cos((pi/180)*w.camT);
    double ya = w.camY + a*sin((pi/180)*w.camT);
    double xb = w.camX + a*cos((pi/180)*(w.camT+k_FOV));
    double yb = w.camY + a*sin((pi/180)*(w.camT+k_FOV));
    (*renderArr)[nLines++] = (line) {.x1 = w.camX, .y1= w.camY, .x2=xa, .y2=ya, .r=200, .g=200, .b=200};
    (*renderArr)[nLines++] = (line) {.x1 = w.camX, .y1= w.camY, .x2=xb, .y2=yb, .r=200, .g=200, .b=200};
    (*renderArr)[nLines++] = (line) {.x1 = xa, .y1= ya, .x2=xb, .y2=yb, .r=200, .g=200, .b=200};
    (*renderArr)[nLines++] = (line) {.x1 = -k_drawD, .x2 = k_drawD, .y1=0, .y2=0, .r=55, .g=55, .b=55};
    (*renderArr)[nLines++] = (line) {.y1 = -k_drawD, .y2 = k_drawD, .x1=0, .x2=0, .r=55, .g=55, .b=55};
    for(int obj=0;;obj++) {
        if(w.scene[obj*3] == terminator) {break;} //Check for termination
        for(int line=0;;line++) {
            if(ob_isTerminating(objFtype_old(w.scene[obj*3])[line])){break;}
            l = objFtype_old(w.scene[obj*3])[line];
            ob_realifyLine(&l, (w.scene + obj*3 +1));
            (*renderArr)[nLines++] = l;
        }
    }
    (*renderArr)[nLines] = (line) {.y1=0, .y2=0, .x1=0, .x2=0, .r=0, .g=0, .b=0};
    return nLines;
}

void rn_perspFworld_v_old(unsigned char *screen, struct world w, line **lineArr){
    bool debug = !(lineArr == NULL);
    bool rays = true; //true for fun visual effects
    int nLines=0; //for debug purposes
    int camD = 500; //This is arbitrary, it just has to be set large enough so there aren't roundoff errors
    for(int i=0;i<k_nPixels;i++){
        screen[i*3]=107;
        screen[i*3+1]=136;
        screen[i*3+2]=255;
    }
    if(debug){
        *lineArr = resalloc(*lineArr, sizeof(line) * k_nMaxLinesPerObj * k_nMaxObj + k_nPixels);
        do{nLines++;}while(!ob_isTerminating((*lineArr)[nLines]));
    }
    double a = camD/cos((pi/180)*k_FOV/2);
    double xa = w.camX + a*cos((pi/180)*w.camT);
    double ya = w.camY + a*sin((pi/180)*w.camT);
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
        closeD=k_drawD;
        if(debug && rays){
            c.r = 200;
            c.g = 200;
            c.b = 200;
            (*lineArr)[nLines] = c;
        }
        for(int obj=0;;obj++) {
            if(w.scene[obj*3] == terminator) {break;} //Check for termination
            if(w.scene[obj*3] == objPlayer) {continue;} //don't render playerBox
            //for(int li=0;;li++) {}
            for(line* lp=objFtype_old(w.scene[obj*3]);!ob_isTerminating(l = *lp);lp++) {
                ob_realifyLine(&l, (w.scene + obj*3 +1));
                if((newD = ob_intersectD(c, l) ) == 0) {continue;}
                if(newD < closeD) {
                    closeD=newD;
                    screen[y*3 +0]=l.r;
                    screen[y*3 +1]=l.g;
                    screen[y*3 +2]=l.b;
                    if(debug){
                        float intX, intY;
                        int xx, yy;
                        ob_intersect(c, l, &intX, &intY);
                        if(rays){
                            c.x2 = intX;
                            c.y2 = intY;
                            c.r = 250;
                            c.g = 250;
                            c.b = 250;
                            (*lineArr)[nLines] = c;
                        }
                        else{
                            if(l.x1==l.x2){xx = intX; yy = intY+1;}
                            else{xx = intX+1; yy=intY+1*ob_slope(l);}
                            (*lineArr)[nLines] = (line){.x1 = (int) intX, .y1 = (int) intY, .x2 = xx, .y2 = yy, .r = fmin(l.r+80, 255), .g = fmin(l.g+80, 255), .b = fmin(l.b+80, 255)};
                        }
                    }
                }
            }
        }
        if(debug){
            if(!ob_isTerminating((*lineArr)[nLines]) || rays) {nLines++;}
        }
    }
    if(debug){
        (*lineArr)[nLines] = (line) {.y1=0, .y2=0, .x1=0, .x2=0, .r=0, .g=0, .b=0};
    }
    return;
}

void rn_dimFworld(point *points, struct world w){
    point p;
    int nPoints=0;

    int camD = 10;
    double a = camD/cos((pi/180)*k_FOV/2);
    double xa = w.camX + a*cos((pi/180)*w.camT);
    double ya = w.camY + a*sin((pi/180)*w.camT);
    double xb = w.camX + a*cos((pi/180)*(w.camT+k_FOV));
    double yb = w.camY + a*sin((pi/180)*(w.camT+k_FOV));
    points[nPoints++] = (point) {.x = w.camX, .y = w.camY, c_black};
    points[nPoints++] = (point) {.x = xa, .y = ya, c_black};
    points[nPoints++] = (point) {.x = xb, .y = yb, c_black};

    for(int obj=0;;obj++) {
        if(w.scene[obj*3] == terminator) {break;} //Check for termination
        for(int pn=0;;pn++) {
            if(ob_p_isTerm(objFtype(w.scene[obj*3])[pn])){break;}
            p = objFtype(w.scene[obj*3])[pn];
            ob_realifyPoint(&p, (w.scene + obj*3 +1));
            points[nPoints++] = p;
        }
    }
    points[nPoints] = p_termPoint;
    return;
}

void rn_perspFworld_v(unsigned char *screen, struct world w, point *points){
    for(int i=0;i<k_nPixels;i++){
        screen[i*3]=107;
        screen[i*3+1]=136;
        screen[i*3+2]=255;
    }
    float ds[k_nPixels];
    for(int i=0;i<k_nPixels;i++){ds[i] = k_drawD;}
    float d;
    point p;
    float intX, intY;
    int y;
    int camD = 500;
    double a = camD/cos((pi/180)*k_FOV/2);
    double xa = w.camX + a*cos((pi/180)*w.camT);
    double ya = w.camY + a*sin((pi/180)*w.camT);
    double xb = w.camX + a*cos((pi/180)*(w.camT+k_FOV));
    double yb = w.camY + a*sin((pi/180)*(w.camT+k_FOV));
    double beta = 90+k_FOV/2+w.camT;
    line c = (line) {.x1 = xa, .y1 = ya, .x2 = xb, .y2 = yb};
        for(int obj=0;;obj++) {
            if(w.scene[obj*3] == terminator) {break;} //Check for termination
            if(w.scene[obj*3] == objPlayer) {continue;} //don't render playerBox
            for(point* pp=objFtype(w.scene[obj*3]);!ob_p_isTerm(p = *pp);pp++) {
                ob_realifyPoint(&p, (w.scene + obj*3 +1));
                line l = (line) {.x1 = w.camX, .y1 = w.camY, .x2 = p.x, .y2 = p.y};
                if(ob_intersectD(l, c) == 0){continue;}
                ob_intersect(l, c, &intX, &intY);
                line l1 = (line) { .x1 = xa, .y1 = ya, .x2 = intX, .y2 = intY};
                line l2 = (line) { .x1 = xa, .y1 = ya, .x2 = xb, .y2 = yb};
                line l3 = (line) { .x1 = w.camX, .y1 = w.camY, .x2 = p.x, .y2 = p.y};
                y = ob_len(l1)/ob_len(l2) *k_nPixels;
                d = ob_len(l3);
                printf("%f, %f\n", d, ds[y]);
                if(d<ds[y]){
                    ds[y] = d;
                    screen[y*3+0] = p.r;
                    screen[y*3+1] = p.g;
                    screen[y*3+2] = p.b;
                }
            }
        }
    return;
}
