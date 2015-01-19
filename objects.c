#include "objects.h"
#include "helpers.h"
#include <math.h>

//objects possible in the scene
void ob_init() {
    int i = 0;
    ob_ground = salloc(sizeof(line) * k_nMaxObj);
    //top face
    ob_ground[i++] = (line) { .x1=0, .y1=16, .x2=1, .y2=16, .r=231, .g=95, .b=19 };
    ob_ground[i++] = (line) { .x1=1, .y1=16, .x2=10, .y2=16, .r=240, .g=208, .b=176 };
    ob_ground[i++] = (line) { .x1=10, .y1=16, .x2=11, .y2=16, .r=0, .g=0, .b=0 };
    ob_ground[i++] = (line) { .x1=11, .y1=16, .x2=12, .y2=16, .r=231, .g=95, .b=19 };
    ob_ground[i++] = (line) { .x1=12, .y1=16, .x2=15, .y2=16, .r=240, .g=208, .b=176 };
    ob_ground[i++] = (line) { .x1=15, .y1=16, .x2=16, .y2=16, .r=231, .g=95, .b=19 };

    //back face
    ob_ground[i++] = (line) { .x1=16, .y1=16, .x2=16, .y2=0, .r=231, .g=95, .b=19 };

    //bottom face
    ob_ground[i++] = (line) { .x1=0, .y1=0, .x2=16, .y2=0, .r=231, .g=95, .b=19 };

    //front face
    ob_ground[i++] = (line) { .x1=0, .y1=0, .x2=0, .y2=16, .r=231, .g=95, .b=19 };

    ob_ground[i++] = (line) { .x1=0, .y1=0, .x2=0, .y2=0, .r=0, .g=0, .b=0 };

    ob_brick = salloc(sizeof(line) * k_nMaxObj);
    i = 0;
    //top face
    ob_brick[i++] = (line) { .x1=0, .y1=16, .x2=7, .y2=16, .r=240, .g=208, .b=176 };
    ob_brick[i++] = (line) { .x1=7, .y1=16, .x2=8, .y2=16, .r=200, .g=200, .b=200 };
    ob_brick[i++] = (line) { .x1=0, .y1=16, .x2=15, .y2=16, .r=240, .g=208, .b=176 };
    ob_brick[i++] = (line) { .x1=15, .y1=16, .x2=16, .y2=16, .r=200, .g=200, .b=200 };

    //back face
    ob_brick[i++] = (line) { .x1=16, .y1=16, .x2=16, .y2=15, .r=240, .g=208, .b=176 };
    ob_brick[i++] = (line) { .x1=16, .y1=15, .x2=16, .y2=12, .r=0, .g=0, .b=0 };
    ob_brick[i++] = (line) { .x1=16, .y1=12, .x2=16, .y2=9, .r=231, .g=95, .b=19 };
    ob_brick[i++] = (line) { .x1=16, .y1=9, .x2=16, .y2=4, .r=0, .g=0, .b=0 };
    ob_brick[i++] = (line) { .x1=16, .y1=4, .x2=16, .y2=1, .r=231, .g=95, .b=19 };
    ob_brick[i++] = (line) { .x1=16, .y1=1, .x2=16, .y2=0, .r=0, .g=0, .b=0 };

    //bottom face
    ob_brick[i++] = (line) { .x1=0, .y1=0, .x2=3, .y2=0, .r=0, .g=0, .b=0 };
    ob_brick[i++] = (line) { .x1=3, .y1=0, .x2=4, .y2=0, .r=20, .g=20, .b=20 };
    ob_brick[i++] = (line) { .x1=4, .y1=0, .x2=11, .y2=0, .r=0, .g=0, .b=0 };
    ob_brick[i++] = (line) { .x1=11, .y1=0, .x2=12, .y2=0, .r=20, .g=20, .b=20 };
    ob_brick[i++] = (line) { .x1=12, .y1=0, .x2=16, .y2=0, .r=0, .g=0, .b=0 };

    //front face
    ob_brick[i++] = (line) { .x1=0, .y1=0, .x2=0, .y2=1, .r=30, .g=30, .b=30 };
    ob_brick[i++] = (line) { .x1=0, .y1=1, .x2=0, .y2=4, .r=231, .g=95, .b=19 };
    ob_brick[i++] = (line) { .x1=0, .y1=4, .x2=0, .y2=5, .r=30, .g=30, .b=30 };
    ob_brick[i++] = (line) { .x1=0, .y1=5, .x2=0, .y2=8, .r=231, .g=95, .b=19 };
    ob_brick[i++] = (line) { .x1=0, .y1=8, .x2=0, .y2=9, .r=30, .g=30, .b=30 };
    ob_brick[i++] = (line) { .x1=0, .y1=9, .x2=0, .y2=12, .r=231, .g=95, .b=19 };
    ob_brick[i++] = (line) { .x1=0, .y1=12, .x2=0, .y2=13, .r=30, .g=30, .b=30 };
    ob_brick[i++] = (line) { .x1=0, .y1=13, .x2=0, .y2=16, .r=231, .g=95, .b=19 };

    ob_brick[i++] = (line) { .x1=0, .y1=0, .x2=0, .y2=0, .r=0, .g=0, .b=0 };

    ob_nothing = salloc(sizeof(line));
    ob_nothing[0] = (line) { .x1=0, .y1=0, .x2=0, .y2=0, .r=0, .g=0, .b=0 };

    ob_playerBox = salloc(sizeof(line)*5);
    ob_playerBox[0] = (line) { .x1=-14, .y1=2, .x2=2, .y2=2, .r=231, .g=0, .b=231 };
    ob_playerBox[1] = (line) { .x1=2, .y1=2, .x2=2, .y2=-14, .r=231, .g=0, .b=231 };
    ob_playerBox[2] = (line) { .x1=2, .y1=-14, .x2=-14, .y2=-14, .r=231, .g=0, .b=231 };
    ob_playerBox[3] = (line) { .x1=-14, .y1=-14, .x2=-14, .y2=2, .r=231, .g=0, .b=231 };
    ob_playerBox[4] = (line) { .x1=0, .y1=0, .x2=0, .y2=0, .r=0, .g=0, .b=0 };

    ob_levelTest = salloc(sizeof(int) * k_nMaxObj);
    ob_levelTest[0] = objPlayer;
    ob_levelTest[1] = 14;
    ob_levelTest[2] = 14;
    for(int i=1;i<20;i++){
        ob_levelTest[i*3] = objGround;
        ob_levelTest[i*3+1] = (i-10)*16;
        ob_levelTest[i*3+2] = -17;
    }
    ob_levelTest[60] = objBrick;
    ob_levelTest[61] = 30;
    ob_levelTest[62] = 30;
    ob_levelTest[63] = terminator;

}
void ob_deinit() {
    free(ob_ground);
    free(ob_nothing);
    free(ob_levelTest);
}

void ob_printLine(line l){
    printf("{ x1: %d, x2: %d, y1: %d, y2: %d, r: %d, g: %d, b: %d }\n", l.x1, l.x2, l.y1, l.y2, l.r, l.g, l.b);
}

void ob_realifyLine(line *l, int *p){
    (*l).x1 += p[0];
    (*l).x2 += p[0];
    (*l).y1 += p[1];
    (*l).y2 += p[1];
    return;
}

float f_round(float f){
    return( (((int) (f*10 + ((f > 0) ? 0.5: -0.5))))/10.0);
}

bool ob_intersect(line l1, line l2, float* x, float* y){ //returns whether or not there was an intersection
    if( (l1.y1 == l1.y2 && l1.x1 == l1.x2) || (l2.y1 == l2.y2 && l2.x1 == l2.x2) ) {printf("Error in ob_intersect(): Argument not a line"); exit(1);}
    float d;
    if( f_round(d = (l1.x1-l1.x2)*(l2.y1-l2.y2) - (l1.y1-l1.y2)*(l2.x1-l2.x2)) == 0 ) {return false;}

    *x=( (l1.x1*l1.y2-l1.y1*l1.x2)*(l2.x1-l2.x2) - (l1.x1-l1.x2)*(l2.x1*l2.y2-l2.y1*l2.x2) )/d;
    *y=( (l1.x1*l1.y2-l1.y1*l1.x2)*(l2.y1-l2.y2) - (l1.y1-l1.y2)*(l2.x1*l2.y2-l2.y1*l2.x2) )/d;

    return true;
}

bool ob_intersectIn(line l1, line l2){
    float x, y;
    if(!ob_intersect(l1, l2, &x, &y)){return false;}

    if(x<l1.x1 == x<l1.x2 && l1.x1!=l1.x2){return false;}
    if(y<l1.y1 == y<l1.y2 && l1.y1!=l1.y2){return false;}
    if(x<l2.x1 == x<l2.x2 && l2.x1!=l2.x2){return false;}
    if(y<l2.y1 == y<l2.y2 && l2.y1!=l2.y2){return false;}

    return true;
}

float ob_intersectD(line c, line l){ //returns 0 if there's no intersection. Edge cases are whatever.
    float x, y;
    if(!ob_intersect(c, l, &x, &y)){return 0;}

    if(x<l.x1 == x<l.x2 && l.x1!=l.x2){return 0;}
    if(y<l.y1 == y<l.y2 && l.y1!=l.y2){return 0;}
    if(c.x1<x != c.x1<c.x2){return 0;}
    if(c.y1<y != c.y1<c.y2){return 0;}

    return (pow(x-c.x1, 2) + pow(y-c.y1, 2));
}

float ob_slope(line l){
    if(l.x1==l.x2){printf("Error in ob_slope(): Dx = 0"); exit(1);}
    return ( (float) l.y2 - l.y1) / (l.x2 - l.x1);
}

bool ob_isTerminating(line l){
    return ! (bool) (l.x1 | l.x2 | l.y1 | l.y2);
}

line* objFtype(enum objType type) {
    switch(type) {
        case objGround:
            return ob_ground;
            break;
        case nothing:
            return ob_nothing;
            break;
        case objPlayer:
            return ob_playerBox;
            break;
        case objBrick:
            return ob_brick;
            break;
        default:
            printf("No object data found for object type %d", type);
            exit(1);
            break;
    }
}
