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
        if(c.scene[obj].type == '\0') {break;}
        for(int pn=0;;pn++) {
            if(ob_p_isTerm(p = c.scene[obj].ps[c.scene[obj].animFrame/k_animFreq % c.scene[obj].nps][pn])){break;}

            ob_realifyPoint(&p, c.scene[obj].x, c.scene[obj].y);
            points[nPoints++] = p;
        }
    }
    points[nPoints] = p_termPoint;
    return;
}

void rn_perspFcamera(unsigned char *screen, struct camera c, point *points){
    for(int i=0;i<k_nPixels;i++){
        screen[i*3]=k_bgr;
        screen[i*3+1]=k_bgg;
        screen[i*3+2]=k_bgb;
        if(c.flashD && c.animFrame/16 % 2){
            screen[i*3+0] /= 2;
            screen[i*3+1] /= 2;
            screen[i*3+2] /= 2;
        }
        if(c.flashB && c.animFrame/16 % 2){
            screen[i*3+0] = (screen[i*3+0]+255)/2;
            screen[i*3+1] = (screen[i*3+1]+255)/2;
            screen[i*3+2] = (screen[i*3+2]+255)/2;
        }
        if(c.redTint){
            screen[i*3+0] = (2*screen[i*3+0]+255)/3;
            screen[i*3+1] = (2*screen[i*3+1]+0)/3;
            screen[i*3+2] = (2*screen[i*3+2]+0)/3;
        }
    }
    float ds[k_nPixels];
    for(int i=0;i<k_nPixels;i++){ds[i] = c.drawD;}
    float d;
    int D;
    point p;
    int y;
    for(int obj=0;;obj++) {
        if(c.scene[obj].type == '\0') {break;}
        if(c.scene[obj].hidden == true || c.scene[obj].type == '.') {continue;}
        for(point* pp=c.scene[obj].ps[c.scene[obj].animFrame/k_animFreq % c.scene[obj].nps];!ob_p_isTerm(p = *pp);pp++) {
            if(ob_p_isSkip(p)){continue;}
            if(c.scene[obj].flip){p.x = c.scene[obj].bb.w - p.x;}
            ob_realifyPoint(&p, c.scene[obj].x, c.scene[obj].y);
            p.x -= c.x;
            p.y -= c.y;
            double alpha = atan2(p.y, p.x);
            if(c.flip){alpha = pi-alpha;}
            alpha = fmod(alpha - c.T, 2*pi);
            if(alpha > pi){alpha -= 2*pi;}

            if(alpha<-0.5 || alpha>c.FOV+0.5){continue;}
            y = (tan(alpha-c.FOV/2) + tan(c.FOV/2) ) / (2*tan(c.FOV/2)) * k_nPixels;
            d = sqrt(p.x*p.x + p.y*p.y);
            D = ceil(1/(2*d*tan(c.FOV/2))*k_nPixels);
            for(int i=y-D;i<y+D;i++){
                if(i<0 || i >= k_nPixels) {continue;}
                if(d<ds[i]){
                    ds[i] = d;
                    screen[i*3+0] = p.r;
                    screen[i*3+1] = p.g;
                    screen[i*3+2] = p.b;
                    if(d>k_drawD1){
                        screen[i*3+0] = (k_bgr - screen[i*3+0])/(k_drawD2 - k_drawD1)*(d-k_drawD1) + screen[i*3+0];
                        screen[i*3+1] = (k_bgg - screen[i*3+1])/(k_drawD2 - k_drawD1)*(d-k_drawD1) + screen[i*3+1];
                        screen[i*3+2] = (k_bgb - screen[i*3+2])/(k_drawD2 - k_drawD1)*(d-k_drawD1) + screen[i*3+2];
                    }
                    if(c.flashD && c.animFrame/16 % 2){
                        screen[i*3+0] /= 2;
                        screen[i*3+1] /= 2;
                        screen[i*3+2] /= 2;
                    }
                    if(c.flashB && c.animFrame/16 % 2){
                        screen[i*3+0] = (screen[i*3+0]+255)/2;
                        screen[i*3+1] = (screen[i*3+1]+255)/2;
                        screen[i*3+2] = (screen[i*3+2]+255)/2;
                    }
                    if(c.redTint){
                        screen[i*3+0] = (2*screen[i*3+0]+255)/3;
                        screen[i*3+1] = (2*screen[i*3+1]+0)/3;
                        screen[i*3+2] = (2*screen[i*3+2]+0)/3;
                    }
                }
            }
        }
    }
    return;
}
