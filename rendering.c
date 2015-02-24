#include "rendering.h"

void rn_dimFcamera(point *points, struct camera c){
    point p;
    int nPoints=0;

    int camD = 10;
    double a = camD/cos(c.FOV/2);
    double xa = c.x + a*cos(c.T);
    double ya = c.y + a*sin(c.T);
    double xb = c.x + a*cos(c.T+c.FOV);
    double yb = c.y + a*sin(c.T+c.FOV);
    points[nPoints++] = (point) {.x = c.x, .y = c.y, c_black};
    points[nPoints++] = (point) {.x = xa, .y = ya, c_black};
    points[nPoints++] = (point) {.x = xb, .y = yb, c_black};
    points[nPoints++] = (point) {.x = 0, .y = 0, c_brick};

    for(int obj=0;;obj++) {
        if(c.scene[obj].type == '\0') {break;} //Check for termination
        for(int pn=0;;pn++) {
            if(ob_p_isTerm(p = c.scene[obj].ps[c.animFrame/10 % c.scene[obj].nps][pn])){break;}

            ob_realifyPoint(&p, c.scene[obj].x, c.scene[obj].y);
            points[nPoints++] = p;
        }
    }
    points[nPoints] = p_termPoint;
    return;
}

void rn_perspFcamera(unsigned char *screen, struct camera c, point *points){
    //TODO: bottom and top should render even if technically off screen
    static int starr = 255, starg = 255, starb = 255;
    for(int i=0;i<k_nPixels;i++){
        screen[i*3]=107;
        screen[i*3+1]=136;
        screen[i*3+2]=255;
        if(c.flashD && c.animFrame/16 % 2){
            screen[i*3+0] /= 2;
            screen[i*3+1] /= 2;
            screen[i*3+2] /= 2;
        }
        if(c.flashB && c.animFrame/16 % 2){
            screen[i*3+0] = (screen[i*3+0]+starr)/2;
            screen[i*3+1] = (screen[i*3+1]+starg)/2;
            screen[i*3+2] = (screen[i*3+2]+starb)/2;
        }
    }
    float ds[k_nPixels];
    for(int i=0;i<k_nPixels;i++){ds[i] = c.drawD;}
    float d;
    int D;
    point p;
    int y;
    for(int obj=0;;obj++) {
        if(c.scene[obj].type == '\0') {break;} //Check for termination
        if(c.scene[obj].hidden == true) {continue;}
        for(point* pp=c.scene[obj].ps[c.animFrame/10 % c.scene[obj].nps];!ob_p_isTerm(p = *pp);pp++) {
            ob_realifyPoint(&p, c.scene[obj].x, c.scene[obj].y);
            p.x -= c.x;
            p.y -= c.y;
            double alpha = atan2(p.y, p.x) - c.T;
            alpha = fmod(alpha, 2*pi); //TODO: Broke camFlip.

            if(!(alpha>=-1 && alpha<c.FOV+1)){continue;} //TODO: Edges don't work with CamFlip
            y = (tan(alpha-c.FOV/2) + tan(c.FOV/2) ) / (2*tan(c.FOV/2)) * k_nPixels;
            /*if(c.flip){y = k_nPixels-y;}*/
            d = sqrt(p.x*p.x + p.y*p.y);
            D = ceil(1/(2*d*tan(c.FOV/2))*k_nPixels);
            for(int i=y-D;i<y+D;i++){
                if(i<0 || i >= k_nPixels) {continue;}
                if(d<ds[i]){
                    ds[i] = d;
                    screen[i*3+0] = p.r;
                    screen[i*3+1] = p.g;
                    screen[i*3+2] = p.b;
                    if(c.flashD && c.animFrame/16 % 2){
                        screen[i*3+0] /= 2;
                        screen[i*3+1] /= 2;
                        screen[i*3+2] /= 2;
                    }
                    if(c.flashB && c.animFrame/16 % 2){
                        screen[i*3+0] = (screen[i*3+0]+starr)/2;
                        screen[i*3+1] = (screen[i*3+1]+starg)/2;
                        screen[i*3+2] = (screen[i*3+2]+starb)/2;
                    }
                    if(c.animFrame % 16 == 0){
                        starr = 255;
                        starg = 255;
                        starb = 255;
                    }
                }
            }
        }
    }
    return;
}
