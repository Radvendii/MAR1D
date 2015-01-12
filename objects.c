#include "objects.h"
#include "helpers.h"
#include <math.h>
#define pi (22/7.0)

//objects possible in the scene
void ob_init() {
    ob_ground = salloc(sizeof(line) * 5);
    ob_ground[0] = (line) { .x1=0, .y1=16, .x2=16, .y2=16, .r=231, .g=95, .b=19 };
    ob_ground[1] = (line) { .x1=16, .y1=16, .x2=16, .y2=0, .r=231, .g=95, .b=19 };
    ob_ground[2] = (line) { .x1=16, .y1=0, .x2=0, .y2=0, .r=231, .g=95, .b=19 };
    ob_ground[3] = (line) { .x1=0, .y1=0, .x2=0, .y2=16, .r=231, .g=95, .b=19 };
    ob_ground[4] = (line) { .x1=0, .y1=0, .x2=0, .y2=0, .r=0, .g=0, .b=0 };

    ob_nothing = salloc(sizeof(line));
    ob_nothing[0] = (line) { .x1=0, .y1=0, .x2=0, .y2=0, .r=0, .g=0, .b=0 };

    ob_levelTest = salloc(sizeof(int) * 7);
    ob_levelTest[0] = objGround;
    ob_levelTest[1] = 100;
    ob_levelTest[2] = -8;
    ob_levelTest[3] = objGround;
    ob_levelTest[4] = 95;
    ob_levelTest[5] = 0;
    ob_levelTest[6] = terminator;
}
void ob_deinit() {
    free(ob_ground);
    free(ob_nothing);
    free(ob_levelTest);
}

void ob_printLine(line *l){
    printf("{ x1: %d, x2: %d, y1: %d, y2: %d, r: %d, g: %d, b: %d }\n", l->x1, l->x2, l->y1, l->y2, l->r, l->g, l->b);
}

void ob_realifyLine(line *l, int *p){
    l->x1 += p[0];
    l->x2 += p[0];
    l->y1 += p[1];
    l->y2 += p[1];
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

    return pow(x-c.x1, 2) + pow(y-c.y1, 2);
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
        default:
            printf("No object data found for object type %d", type);
            exit(1);
            break;
    }
}
