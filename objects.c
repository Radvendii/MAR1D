#include "objects.h"
#include "parsing.h"

void ob_init(){
    ob_colors = NULL;
    ob_objs = NULL;
    ob_levels = NULL;
    io_getColors(&ob_colors, "test.colors");
    io_getLevels(&ob_levels, "test.colors");
    io_getObjs(&ob_objs, ob_colors, "test.colors");

    ob_levelTest = ob_levels['t'];
}

void ob_deinit() {
    free(ob_colors);
    for(int i=0;i<127;i++){
        free(ob_levels[i]);
        free(ob_objs[i].ps);
        free(ob_objs[i].cols);
    }
    free(ob_objs);
    free(ob_levels);
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
