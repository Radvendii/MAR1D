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
        if(c.scene[obj*3] == '\0') {break;} //Check for termination
        for(int pn=0;;pn++) {
            if(ob_p_isTerm(ob_objs[c.scene[obj*3]].ps[pn])){break;}
            p = ob_objs[c.scene[obj*3]].ps[pn];

            ob_realifyPoint(&p, (c.scene + obj*3 +1));
            points[nPoints++] = p;
        }
    }
    points[nPoints] = p_termPoint;
    return;
}

void rn_perspFcamera(unsigned char *screen, struct camera c, point *points){
    for(int i=0;i<k_nPixels;i++){
        screen[i*3]=107;
        screen[i*3+1]=136;
        screen[i*3+2]=255;
    }
    float ds[k_nPixels];
    for(int i=0;i<k_nPixels;i++){ds[i] = c.drawD;}
    float d;
    int D;
    point p;
    int y;

    double theta = c.T;
    double gamma = c.FOV/2;

        for(int obj=0;;obj++) {
            if(c.scene[obj*3] == '\0') {break;} //Check for termination
            if(c.scene[obj*3] == '@') {continue;} //don't render playerBox
            for(point* pp=ob_objs[c.scene[obj*3]].ps;!ob_p_isTerm(p = *pp);pp++) {
                ob_realifyPoint(&p, (c.scene + obj*3 +1));
                p.x -= c.x;
                p.y -= c.y;
                double alpha = atan2(p.y, p.x) - theta;
                alpha = fmod(alpha, 2*pi);
                if(alpha<0){alpha += 2*pi;}

                if(!(alpha>=0 && alpha<gamma*2)){continue;}
                y = (tan(alpha-gamma) + tan(gamma) ) / (2*tan(gamma)) * k_nPixels;
                if(c.flip){y = k_nPixels-y;}
                d = sqrt(p.x*p.x + p.y*p.y);
                D = ceil(1/(2*d*tan(c.FOV/2))*k_nPixels);
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
