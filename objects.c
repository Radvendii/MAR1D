#include "objects.h"
#include "parsing.h"

void ob_init(){
    ob_colors = NULL;
    ob_pObjs = NULL;
    io_getColors(&ob_colors, "../test.colors");
    io_getPObjs(&ob_pObjs, ob_colors, "../test.colors");
    io_getBoxes(&ob_boxes, "../test.colors");

    ob_levelTest = salloc(sizeof(int) * k_nMaxObj);
    int i=0;
    ob_levelTest[i++] = 'p';
    ob_levelTest[i++] = -5;
    ob_levelTest[i++] = 14;
    while(i/3<30){
        ob_levelTest[i++] = 'g';
        ob_levelTest[i++] = (i/3-15)*16;
        ob_levelTest[i++] = -17;
    }
    while(i/3<30+4){
        ob_levelTest[i++] = 'b';
        ob_levelTest[i++] = 16;
        ob_levelTest[i++] = (i/3-30)*16;
    }
    ob_levelTest[i++] = '\0';

}

void ob_deinit() {
    free(ob_colors);
    for(int i=0;i<127;i++){
        free(ob_pObjs[i]);
    }
    free(ob_pObjs);
    free(ob_boxes);
    free(ob_levelTest);
}

void ob_printPoint(point p){
    printf("{ x: %d, y: %d, r: %d, g: %d, b: %d }\n", p.x, p.y, p.r, p.g, p.b);
}

void ob_realifyPoint(point *p, int *pos){
    (*p).x += pos[0];
    (*p).y += pos[1];
    return;
}

void ob_realifyBox(box *b, int *pos){
    (*b).x += pos[0];
    (*b).y += pos[1];
    return;
}

float f_round(float f){
    return( (((int) (f*10 + ((f > 0) ? 0.5: -0.5))))/10.0);
}

bool ob_p_isTerm(point p){
    return (p.x == 124214 && p.y == 143512 && p.r == 100 && p.g == 100 && p.b == 100);
}
