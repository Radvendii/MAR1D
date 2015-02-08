#include "rendering.h"

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
    points[nPoints++] = (point) {.x = 0, .y = 0, c_brick};

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
    int D;
    point p;
    int y;

    double theta = (pi/180)*w.camT;
    double gamma = (pi/180)*k_FOV/2;

        for(int obj=0;;obj++) {
            if(w.scene[obj*3] == terminator) {break;} //Check for termination
            if(w.scene[obj*3] == objPlayer) {continue;} //don't render playerBox
            for(point* pp=objFtype(w.scene[obj*3]);!ob_p_isTerm(p = *pp);pp++) {
                ob_realifyPoint(&p, (w.scene + obj*3 +1));
                p.x -= w.camX;
                p.y -= w.camY;
                double alpha = atan2(p.y, p.x) - theta;
                alpha = fmod(alpha, 2*pi);
                if(alpha<0){alpha += 2*pi;}

                if(!(alpha>=0 && alpha<gamma*2)){continue;}
                y = (tan(alpha-gamma) + tan(gamma) ) / (2*tan(gamma)) * k_nPixels;
                d = sqrt(p.x*p.x + p.y*p.y);
                D = ceil(1/(2*d*tan((pi/180)*k_FOV/2))*k_nPixels);
                for(int i=y-D;i<y+D;i++){
                    if(i<0 || i >= k_nPixels) {continue;}
                    if(d<ds[i]){
                        ds[i] = d;
                        screen[i*3+0] = p.r;
                        screen[i*3+1] = p.g;
                        screen[i*3+2] = p.b;
                    }
                }
            }
        }
    return;
}
