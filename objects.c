#include "objects.h"

//objects possible in the scene
void g_init() {
    g_ground = malloc(sizeof(line) * k_nMaxLinesPerObj);
    g_ground[0] = (line) { .x1=0, .y1=16, .x2=16, .y2=16, .r=231, .g=95, .b=19 };
    g_ground[1] = (line) { .x1=16, .y1=16, .x2=16, .y2=16, .r=231, .g=95, .b=19 };
    g_ground[2] = (line) { .x1=16, .y1=0, .x2=0, .y2=0, .r=231, .g=95, .b=19 };
    g_ground[3] = (line) { .x1=0, .y1=0, .x2=0, .y2=16, .r=231, .g=95, .b=19 };

    g_nothing = malloc(sizeof(line) * k_nMaxLinesPerObj);
    g_nothing[0] = (line) { .x1=0, .y1=0, .x2=0, .y2=16, .r=231, .g=95, .b=19 };

    g_levelTest[0] = objGround;
    g_levelTest[1] = 20;
    g_levelTest[2] = -8;
}

void g_printLine(line *l){
    printf("{ x1: %d, x2: %d, y1: %d, y2: %d }\n", l->x1, l->x2, l->y1, l->y2);
}

bool g_isNotTerminating(line *l){
    return (bool) (l->x1 & l->x2 & l->y1 & l->y2);
}

line* objFtype(enum objType type) {
    switch(type) {
        case objGround:
            return g_ground;
            break;
        case nothing:
            return g_nothing;
            break;
        default:
            printf("No object data found for object type %d", type);
            exit(1);
            break;
    }
}
