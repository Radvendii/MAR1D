#include "objects.h"

//objects possible in the scene
void ob_init() {
    ob_ground = malloc(sizeof(line) * k_nMaxLinesPerObj);
    ob_ground[0] = (line) { .x1=0, .y1=16, .x2=16, .y2=16, .r=231, .g=95, .b=19 };
    ob_ground[1] = (line) { .x1=16, .y1=16, .x2=16, .y2=16, .r=231, .g=95, .b=19 };
    ob_ground[2] = (line) { .x1=16, .y1=0, .x2=0, .y2=0, .r=231, .g=95, .b=19 };
    ob_ground[3] = (line) { .x1=0, .y1=0, .x2=0, .y2=16, .r=231, .g=95, .b=19 };

    ob_nothing = malloc(sizeof(line) * k_nMaxLinesPerObj);
    ob_nothing[0] = (line) { .x1=0, .y1=0, .x2=0, .y2=16, .r=231, .g=95, .b=19 };

    ob_levelTest[0] = objGround;
    ob_levelTest[1] = 10;
    ob_levelTest[2] = 3;
    ob_levelTest[3] = objGround;
    ob_levelTest[4] = 20;
    ob_levelTest[5] = 100;
    ob_levelTest[6] = 0;
}

void ob_printLine(line *l){
    printf("{ x1: %d, x2: %d, y1: %d, y2: %d, r: %d, g: %d, b: %d }\n", l->x1, l->x2, l->y1, l->y2, l->r, l->g, l->b);
}

bool ob_isTerminating(line *l){
    return ! (bool) (l->x1 | l->x2 | l->y1 | l->y2);
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
