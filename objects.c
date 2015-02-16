#include "objects.h"
#include "parsing.h"

void ob_init(){
    ob_colors = NULL;
    ob_objs = NULL;
    ob_levels = NULL;
    io_getColors(&ob_colors, "test.colors");
    io_getLevels(&ob_levels, "test.colors");
    io_getObjs(&ob_objs, ob_colors, "test.colors");

    ob_levelTest = salloc(sizeof(obj) * k_nMaxObj);
    for(int i=0;ob_levels['t'][i]!='\0';i+=3){
        ob_levelTest[i/3] = ob_objs[ob_levels['t'][i]];
        ob_levelTest[i/3].type[0] = ob_levels['t'][i];
        ob_levelTest[i/3].type[1] = '\0';
        ob_levelTest[i/3].x = ob_levels['t'][i+1];
        ob_levelTest[i/3].y = ob_levels['t'][i+2];
    }
}

void ob_deinit() {
    free(ob_colors);
    for(int i=0;i<127;i++){
        free(ob_levels[i]);
        free(ob_objs[i].ps);
        free(ob_objs[i].cols);
    }
    free(ob_levelTest);
    free(ob_objs);
    free(ob_levels);
}

obj ob_objFchar(char c){
    return ob_objs[c];
}

void ob_printPoint(point p){
    printf("{ x: %d, y: %d, r: %d, g: %d, b: %d }\n", p.x, p.y, p.r, p.g, p.b);
}

void ob_realifyPoint(point *p, int x, int y){
    (*p).x += x;
    (*p).y += y;
    return;
}

void ob_realifyBox(box *b, int x, int y){
    (*b).x += x;
    (*b).y += y;
    return;
}

float f_round(float f){
    return( (((int) (f*10 + ((f > 0) ? 0.5: -0.5))))/10.0);
}

bool ob_p_isTerm(point p){
    return (p.x == 124214 && p.y == 143512 && p.r == 100 && p.g == 100 && p.b == 100);
}
