#include "objects.h"

void ob_init_old() {
    int i = 0;
    ob_ground_old = salloc(sizeof(line) * k_nMaxLinesPerObj);
    //top face
    ob_ground_old[i++] = (line) { .x1=0, .y1=16, .x2=1, .y2=16, .r=231, .g=95, .b=19 };
    ob_ground_old[i++] = (line) { .x1=1, .y1=16, .x2=10, .y2=16, .r=240, .g=208, .b=176 };
    ob_ground_old[i++] = (line) { .x1=10, .y1=16, .x2=11, .y2=16, .r=0, .g=0, .b=0 };
    ob_ground_old[i++] = (line) { .x1=11, .y1=16, .x2=12, .y2=16, .r=231, .g=95, .b=19 };
    ob_ground_old[i++] = (line) { .x1=12, .y1=16, .x2=15, .y2=16, .r=240, .g=208, .b=176 };
    ob_ground_old[i++] = (line) { .x1=15, .y1=16, .x2=16, .y2=16, .r=231, .g=95, .b=19 };

    //back face
    ob_ground_old[i++] = (line) { .x1=16, .y1=16, .x2=16, .y2=0, .r=231, .g=95, .b=19 };

    //bottom face
    ob_ground_old[i++] = (line) { .x1=0, .y1=0, .x2=16, .y2=0, .r=231, .g=95, .b=19 };

    //front face
    ob_ground_old[i++] = (line) { .x1=0, .y1=0, .x2=0, .y2=16, .r=231, .g=95, .b=19 };

    ob_ground_old[i++] = (line) { .x1=0, .y1=0, .x2=0, .y2=0, .r=0, .g=0, .b=0 };

    ob_brick_old = salloc(sizeof(line) * k_nMaxLinesPerObj);
    i = 0;
    //bottom face
    ob_brick_old[i++] = (line) { .x1=0, .y1=0, .x2=3, .y2=0, .r=0, .g=0, .b=0 };
    ob_brick_old[i++] = (line) { .x1=3, .y1=0, .x2=4, .y2=0, .r=20, .g=20, .b=20 };
    ob_brick_old[i++] = (line) { .x1=4, .y1=0, .x2=11, .y2=0, .r=0, .g=0, .b=0 };
    ob_brick_old[i++] = (line) { .x1=11, .y1=0, .x2=12, .y2=0, .r=20, .g=20, .b=20 };
    ob_brick_old[i++] = (line) { .x1=12, .y1=0, .x2=16, .y2=0, .r=0, .g=0, .b=0 };

    //front face
    ob_brick_old[i++] = (line) { .x1=0, .y1=0, .x2=0, .y2=1, .r=30, .g=30, .b=30 };
    ob_brick_old[i++] = (line) { .x1=0, .y1=1, .x2=0, .y2=4, .r=231, .g=95, .b=19 };
    ob_brick_old[i++] = (line) { .x1=0, .y1=4, .x2=0, .y2=5, .r=30, .g=30, .b=30 };
    ob_brick_old[i++] = (line) { .x1=0, .y1=5, .x2=0, .y2=8, .r=231, .g=95, .b=19 };
    ob_brick_old[i++] = (line) { .x1=0, .y1=8, .x2=0, .y2=9, .r=30, .g=30, .b=30 };
    ob_brick_old[i++] = (line) { .x1=0, .y1=9, .x2=0, .y2=12, .r=231, .g=95, .b=19 };
    ob_brick_old[i++] = (line) { .x1=0, .y1=12, .x2=0, .y2=13, .r=30, .g=30, .b=30 };
    ob_brick_old[i++] = (line) { .x1=0, .y1=13, .x2=0, .y2=16, .r=231, .g=95, .b=19 };

    //top face
    ob_brick_old[i++] = (line) { .x1=0, .y1=16, .x2=7, .y2=16, .r=240, .g=208, .b=176 };
    ob_brick_old[i++] = (line) { .x1=7, .y1=16, .x2=8, .y2=16, .r=200, .g=200, .b=200 };
    ob_brick_old[i++] = (line) { .x1=0, .y1=16, .x2=15, .y2=16, .r=240, .g=208, .b=176 };
    ob_brick_old[i++] = (line) { .x1=15, .y1=16, .x2=16, .y2=16, .r=200, .g=200, .b=200 };

    //back face
    ob_brick_old[i++] = (line) { .x1=16, .y1=16, .x2=16, .y2=15, .r=240, .g=208, .b=176 };
    ob_brick_old[i++] = (line) { .x1=16, .y1=15, .x2=16, .y2=12, .r=0, .g=0, .b=0 };
    ob_brick_old[i++] = (line) { .x1=16, .y1=12, .x2=16, .y2=9, .r=231, .g=95, .b=19 };
    ob_brick_old[i++] = (line) { .x1=16, .y1=9, .x2=16, .y2=4, .r=0, .g=0, .b=0 };
    ob_brick_old[i++] = (line) { .x1=16, .y1=4, .x2=16, .y2=1, .r=231, .g=95, .b=19 };
    ob_brick_old[i++] = (line) { .x1=16, .y1=1, .x2=16, .y2=0, .r=0, .g=0, .b=0 };

    ob_brick_old[i++] = (line) { .x1=0, .y1=0, .x2=0, .y2=0, .r=0, .g=0, .b=0 };

    i=0;
    ob_coin_old = salloc(sizeof(line) * k_nMaxLinesPerObj);
    ob_coin_old[i++] = (line) { .x1=5, .y1=0, .x2=2, .y2=0, .r=179, .g=114, .b=50 };

    ob_coin_old[i++] = (line) { .x1=2, .y1=0, .x2=0, .y2=3, .r=191, .g=122, .b=54 };

    ob_coin_old[i++] = (line) { .x1=0, .y1=3, .x2=0, .y2=10, .r=217, .g=139, .b=61 };

    ob_coin_old[i++] = (line) { .x1=0, .y1=10, .x2=2, .y2=13, .r=255, .g=163, .b=71 };

    ob_coin_old[i++] = (line) { .x1=2, .y1=13, .x2=5, .y2=13, .r=255, .g=178, .b=86 };

    ob_coin_old[i++] = (line) { .x1=5, .y1=13, .x2=7, .y2=13, .r=77, .g=49, .b=21 };
    ob_coin_old[i++] = (line) { .x1=7, .y1=13, .x2=9, .y2=10, .r=77, .g=49, .b=21 };
    ob_coin_old[i++] = (line) { .x1=9, .y1=10, .x2=9, .y2=3, .r=77, .g=49, .b=21 };
    ob_coin_old[i++] = (line) { .x1=9, .y1=3, .x2=7, .y2=0, .r=77, .g=49, .b=21 };
    ob_coin_old[i++] = (line) { .x1=7, .y1=0, .x2=5, .y2=0, .r=77, .g=49, .b=21 };

    ob_nothing_old = salloc(sizeof(line));
    i=0;
    ob_nothing_old[i++] = (line) { .x1=0, .y1=0, .x2=0, .y2=0, .r=0, .g=0, .b=0 };

    ob_playerBox_old = salloc(sizeof(line)*5);
    i=0;
    ob_playerBox_old[i++] = (line) { .x1=-14, .y1=2, .x2=2, .y2=2, .r=200, .g=200, .b=200 };
    ob_playerBox_old[i++] = (line) { .x1=2, .y1=2, .x2=2, .y2=-14, .r=200, .g=200, .b=200 };
    ob_playerBox_old[i++] = (line) { .x1=2, .y1=-14, .x2=-14, .y2=-14, .r=200, .g=200, .b=200 };
    ob_playerBox_old[i++] = (line) { .x1=-14, .y1=-14, .x2=-14, .y2=2, .r=200, .g=200, .b=200 };
    ob_playerBox_old[i++] = (line) { .x1=0, .y1=0, .x2=0, .y2=0, .r=0, .g=0, .b=0 };

    ob_levelTest_old = salloc(sizeof(int) * k_nMaxObj);
    i=0;
    ob_levelTest_old[i++] = objPlayer;
    ob_levelTest_old[i++] = 14;
    ob_levelTest_old[i++] = 14;
    while(i/3<20){
        ob_levelTest_old[i++] = objGround;
        ob_levelTest_old[i++] = (i/3-10)*16;
        ob_levelTest_old[i++] = -17;
    }
    while(i/3<30){
        ob_levelTest_old[i++] = objBrick;
        ob_levelTest_old[i++] = (i/3-20)*16;
        ob_levelTest_old[i++] = 30;
    }
    ob_levelTest_old[i++] = terminator;

}

void ob_deinit_old() {
    free(ob_ground_old);
    free(ob_nothing_old);
    free(ob_levelTest_old);
    free(ob_playerBox_old);
    free(ob_brick_old);
    free(ob_coin_old);
}

void ob_init(){
    int i = 0;
    ob_ground = salloc(sizeof(point) * (16*4+1));
    //bottom face
    ob_ground[i++] = (point) {.x=0, .y=0, c_brick};
    ob_ground[i++] = (point) {.x=1, .y=0, c_black};
    ob_ground[i++] = (point) {.x=2, .y=0, c_black};
    ob_ground[i++] = (point) {.x=3, .y=0, c_black};
    ob_ground[i++] = (point) {.x=4, .y=0, c_black};
    ob_ground[i++] = (point) {.x=5, .y=0, c_black};
    ob_ground[i++] = (point) {.x=6, .y=0, c_black};
    ob_ground[i++] = (point) {.x=7, .y=0, c_brick};
    ob_ground[i++] = (point) {.x=8, .y=0, c_lbrick};
    ob_ground[i++] = (point) {.x=9, .y=0, c_black};
    ob_ground[i++] = (point) {.x=10, .y=0, c_black};
    ob_ground[i++] = (point) {.x=11, .y=0, c_black};
    ob_ground[i++] = (point) {.x=12, .y=0, c_black};
    ob_ground[i++] = (point) {.x=13, .y=0, c_black};
    ob_ground[i++] = (point) {.x=14, .y=0, c_black};
    ob_ground[i++] = (point) {.x=15, .y=0, c_brick};

    //back face
    ob_ground[i++] = (point) {.x=15, .y=0, c_brick};
    ob_ground[i++] = (point) {.x=15, .y=1, c_black};
    ob_ground[i++] = (point) {.x=15, .y=2, c_black};
    ob_ground[i++] = (point) {.x=15, .y=3, c_black};
    ob_ground[i++] = (point) {.x=15, .y=4, c_black};
    ob_ground[i++] = (point) {.x=15, .y=5, c_black};
    ob_ground[i++] = (point) {.x=15, .y=6, c_black};
    ob_ground[i++] = (point) {.x=15, .y=7, c_black};
    ob_ground[i++] = (point) {.x=15, .y=8, c_black};
    ob_ground[i++] = (point) {.x=15, .y=9, c_black};
    ob_ground[i++] = (point) {.x=15, .y=10, c_black};
    ob_ground[i++] = (point) {.x=15, .y=11, c_brick};
    ob_ground[i++] = (point) {.x=15, .y=12, c_black};
    ob_ground[i++] = (point) {.x=15, .y=13, c_black};
    ob_ground[i++] = (point) {.x=15, .y=14, c_black};
    ob_ground[i++] = (point) {.x=15, .y=15, c_brick};

    //top face
    ob_ground[i++] = (point) {.x=0, .y=15, c_brick};
    ob_ground[i++] = (point) {.x=1, .y=15, c_lbrick};
    ob_ground[i++] = (point) {.x=2, .y=15, c_lbrick};
    ob_ground[i++] = (point) {.x=3, .y=15, c_lbrick};
    ob_ground[i++] = (point) {.x=4, .y=15, c_lbrick};
    ob_ground[i++] = (point) {.x=5, .y=15, c_lbrick};
    ob_ground[i++] = (point) {.x=6, .y=15, c_lbrick};
    ob_ground[i++] = (point) {.x=7, .y=15, c_lbrick};
    ob_ground[i++] = (point) {.x=8, .y=15, c_lbrick};
    ob_ground[i++] = (point) {.x=9, .y=15, c_black};
    ob_ground[i++] = (point) {.x=10, .y=15, c_brick};
    ob_ground[i++] = (point) {.x=11, .y=15, c_lbrick};
    ob_ground[i++] = (point) {.x=12, .y=15, c_lbrick};
    ob_ground[i++] = (point) {.x=13, .y=15, c_lbrick};
    ob_ground[i++] = (point) {.x=14, .y=15, c_lbrick};
    ob_ground[i++] = (point) {.x=15, .y=15, c_brick};

    //front face
    ob_ground[i++] = (point) {.x=0, .y=0, c_brick};
    ob_ground[i++] = (point) {.x=0, .y=1, c_lbrick};
    ob_ground[i++] = (point) {.x=0, .y=2, c_lbrick};
    ob_ground[i++] = (point) {.x=0, .y=3, c_lbrick};
    ob_ground[i++] = (point) {.x=0, .y=4, c_lbrick};
    ob_ground[i++] = (point) {.x=0, .y=5, c_black};
    ob_ground[i++] = (point) {.x=0, .y=6, c_lbrick};
    ob_ground[i++] = (point) {.x=0, .y=7, c_lbrick};
    ob_ground[i++] = (point) {.x=0, .y=8, c_lbrick};
    ob_ground[i++] = (point) {.x=0, .y=9, c_lbrick};
    ob_ground[i++] = (point) {.x=0, .y=10, c_lbrick};
    ob_ground[i++] = (point) {.x=0, .y=11, c_lbrick};
    ob_ground[i++] = (point) {.x=0, .y=12, c_lbrick};
    ob_ground[i++] = (point) {.x=0, .y=13, c_lbrick};
    ob_ground[i++] = (point) {.x=0, .y=14, c_lbrick};
    ob_ground[i++] = (point) {.x=0, .y=15, c_brick};

    ob_ground[i] = p_termPoint;

    ob_brick = salloc(sizeof(point) * (16*4+1));
    i = 0;
    //bottom face
    ob_brick[i++] = (point) {.x=0, .y=0, c_black};
    ob_brick[i++] = (point) {.x=1, .y=0, c_black};
    ob_brick[i++] = (point) {.x=2, .y=0, c_black};
    ob_brick[i++] = (point) {.x=3, .y=0, c_black};
    ob_brick[i++] = (point) {.x=4, .y=0, c_black};
    ob_brick[i++] = (point) {.x=5, .y=0, c_black};
    ob_brick[i++] = (point) {.x=6, .y=0, c_black};
    ob_brick[i++] = (point) {.x=7, .y=0, c_black};
    ob_brick[i++] = (point) {.x=8, .y=0, c_black};
    ob_brick[i++] = (point) {.x=9, .y=0, c_black};
    ob_brick[i++] = (point) {.x=10, .y=0, c_black};
    ob_brick[i++] = (point) {.x=11, .y=0, c_black};
    ob_brick[i++] = (point) {.x=12, .y=0, c_black};
    ob_brick[i++] = (point) {.x=13, .y=0, c_black};
    ob_brick[i++] = (point) {.x=14, .y=0, c_black};
    ob_brick[i++] = (point) {.x=15, .y=0, c_black};

    //back face
    ob_brick[i++] = (point) {.x=15, .y=0, c_black};
    ob_brick[i++] = (point) {.x=15, .y=1, c_brick};
    ob_brick[i++] = (point) {.x=15, .y=2, c_brick};
    ob_brick[i++] = (point) {.x=15, .y=3, c_brick};
    ob_brick[i++] = (point) {.x=15, .y=4, c_black};
    ob_brick[i++] = (point) {.x=15, .y=5, c_black};
    ob_brick[i++] = (point) {.x=15, .y=6, c_black};
    ob_brick[i++] = (point) {.x=15, .y=7, c_black};
    ob_brick[i++] = (point) {.x=15, .y=8, c_black};
    ob_brick[i++] = (point) {.x=15, .y=9, c_brick};
    ob_brick[i++] = (point) {.x=15, .y=10, c_brick};
    ob_brick[i++] = (point) {.x=15, .y=11, c_brick};
    ob_brick[i++] = (point) {.x=15, .y=12, c_black};
    ob_brick[i++] = (point) {.x=15, .y=13, c_black};
    ob_brick[i++] = (point) {.x=15, .y=14, c_black};
    ob_brick[i++] = (point) {.x=15, .y=15, c_lbrick};

    //top face
    ob_brick[i++] = (point) {.x=0, .y=15, c_lbrick};
    ob_brick[i++] = (point) {.x=1, .y=15, c_lbrick};
    ob_brick[i++] = (point) {.x=2, .y=15, c_lbrick};
    ob_brick[i++] = (point) {.x=3, .y=15, c_lbrick};
    ob_brick[i++] = (point) {.x=4, .y=15, c_lbrick};
    ob_brick[i++] = (point) {.x=5, .y=15, c_lbrick};
    ob_brick[i++] = (point) {.x=6, .y=15, c_lbrick};
    ob_brick[i++] = (point) {.x=7, .y=15, c_lbrick};
    ob_brick[i++] = (point) {.x=8, .y=15, c_lbrick};
    ob_brick[i++] = (point) {.x=9, .y=15, c_lbrick};
    ob_brick[i++] = (point) {.x=10, .y=15, c_lbrick};
    ob_brick[i++] = (point) {.x=11, .y=15, c_lbrick};
    ob_brick[i++] = (point) {.x=12, .y=15, c_lbrick};
    ob_brick[i++] = (point) {.x=13, .y=15, c_lbrick};
    ob_brick[i++] = (point) {.x=14, .y=15, c_lbrick};
    ob_brick[i++] = (point) {.x=15, .y=15, c_lbrick};

    //front face
    ob_brick[i++] = (point) {.x=0, .y=0, c_black};
    ob_brick[i++] = (point) {.x=0, .y=1, c_brick};
    ob_brick[i++] = (point) {.x=0, .y=2, c_brick};
    ob_brick[i++] = (point) {.x=0, .y=3, c_brick};
    ob_brick[i++] = (point) {.x=0, .y=4, c_black};
    ob_brick[i++] = (point) {.x=0, .y=5, c_brick};
    ob_brick[i++] = (point) {.x=0, .y=6, c_brick};
    ob_brick[i++] = (point) {.x=0, .y=7, c_brick};
    ob_brick[i++] = (point) {.x=0, .y=8, c_black};
    ob_brick[i++] = (point) {.x=0, .y=9, c_brick};
    ob_brick[i++] = (point) {.x=0, .y=10, c_brick};
    ob_brick[i++] = (point) {.x=0, .y=11, c_brick};
    ob_brick[i++] = (point) {.x=0, .y=12, c_black};
    ob_brick[i++] = (point) {.x=0, .y=13, c_brick};
    ob_brick[i++] = (point) {.x=0, .y=14, c_brick};
    ob_brick[i++] = (point) {.x=0, .y=15, c_lbrick};

    ob_brick[i] = p_termPoint;

    ob_nothing = salloc(sizeof(point));
    ob_nothing[0] = p_termPoint;
    ob_playerBox = salloc(sizeof(point)*(16*4+1));
    i=0;
    ob_playerBox[i++] = (point) {.x=-13, .y=-13, c_black};
    ob_playerBox[i++] = (point) {.x=-13, .y=-12, c_black};
    ob_playerBox[i++] = (point) {.x=-13, .y=-11, c_black};
    ob_playerBox[i++] = (point) {.x=-13, .y=-10, c_black};
    ob_playerBox[i++] = (point) {.x=-13, .y=-9, c_black};
    ob_playerBox[i++] = (point) {.x=-13, .y=-8, c_black};
    ob_playerBox[i++] = (point) {.x=-13, .y=-7, c_black};
    ob_playerBox[i++] = (point) {.x=-13, .y=-6, c_black};
    ob_playerBox[i++] = (point) {.x=-13, .y=-5, c_black};
    ob_playerBox[i++] = (point) {.x=-13, .y=-4, c_black};
    ob_playerBox[i++] = (point) {.x=-13, .y=-3, c_black};
    ob_playerBox[i++] = (point) {.x=-13, .y=-2, c_black};
    ob_playerBox[i++] = (point) {.x=-13, .y=-1, c_black};
    ob_playerBox[i++] = (point) {.x=-13, .y=0, c_black};
    ob_playerBox[i++] = (point) {.x=-13, .y=1, c_black};
    ob_playerBox[i++] = (point) {.x=-13, .y=2, c_black};

    ob_playerBox[i++] = (point) {.x=2, .y=-13, c_black};
    ob_playerBox[i++] = (point) {.x=2, .y=-12, c_black};
    ob_playerBox[i++] = (point) {.x=2, .y=-11, c_black};
    ob_playerBox[i++] = (point) {.x=2, .y=-10, c_black};
    ob_playerBox[i++] = (point) {.x=2, .y=-9, c_black};
    ob_playerBox[i++] = (point) {.x=2, .y=-8, c_black};
    ob_playerBox[i++] = (point) {.x=2, .y=-7, c_black};
    ob_playerBox[i++] = (point) {.x=2, .y=-6, c_black};
    ob_playerBox[i++] = (point) {.x=2, .y=-5, c_black};
    ob_playerBox[i++] = (point) {.x=2, .y=-4, c_black};
    ob_playerBox[i++] = (point) {.x=2, .y=-3, c_black};
    ob_playerBox[i++] = (point) {.x=2, .y=-2, c_black};
    ob_playerBox[i++] = (point) {.x=2, .y=-1, c_black};
    ob_playerBox[i++] = (point) {.x=2, .y=0, c_black};
    ob_playerBox[i++] = (point) {.x=2, .y=1, c_black};
    ob_playerBox[i++] = (point) {.x=2, .y=2, c_black};

    ob_playerBox[i++] = (point) {.x=-13, .y=2, c_black};
    ob_playerBox[i++] = (point) {.x=-12, .y=2, c_black};
    ob_playerBox[i++] = (point) {.x=-11, .y=2, c_black};
    ob_playerBox[i++] = (point) {.x=-10, .y=2, c_black};
    ob_playerBox[i++] = (point) {.x=-9, .y=2, c_black};
    ob_playerBox[i++] = (point) {.x=-8, .y=2, c_black};
    ob_playerBox[i++] = (point) {.x=-7, .y=2, c_black};
    ob_playerBox[i++] = (point) {.x=-6, .y=2, c_black};
    ob_playerBox[i++] = (point) {.x=-5, .y=2, c_black};
    ob_playerBox[i++] = (point) {.x=-4, .y=2, c_black};
    ob_playerBox[i++] = (point) {.x=-3, .y=2, c_black};
    ob_playerBox[i++] = (point) {.x=-2, .y=2, c_black};
    ob_playerBox[i++] = (point) {.x=-1, .y=2, c_black};
    ob_playerBox[i++] = (point) {.x=0, .y=2, c_black};
    ob_playerBox[i++] = (point) {.x=1, .y=2, c_black};
    ob_playerBox[i++] = (point) {.x=2, .y=2, c_black};

    ob_playerBox[i++] = (point) {.x=-13, .y=-13, c_black};
    ob_playerBox[i++] = (point) {.x=-12, .y=-13, c_black};
    ob_playerBox[i++] = (point) {.x=-11, .y=-13, c_black};
    ob_playerBox[i++] = (point) {.x=-10, .y=-13, c_black};
    ob_playerBox[i++] = (point) {.x=-9, .y=-13, c_black};
    ob_playerBox[i++] = (point) {.x=-8, .y=-13, c_black};
    ob_playerBox[i++] = (point) {.x=-7, .y=-13, c_black};
    ob_playerBox[i++] = (point) {.x=-6, .y=-13, c_black};
    ob_playerBox[i++] = (point) {.x=-5, .y=-13, c_black};
    ob_playerBox[i++] = (point) {.x=-4, .y=-13, c_black};
    ob_playerBox[i++] = (point) {.x=-3, .y=-13, c_black};
    ob_playerBox[i++] = (point) {.x=-13, .y=-13, c_black};
    ob_playerBox[i++] = (point) {.x=-1, .y=-13, c_black};
    ob_playerBox[i++] = (point) {.x=0, .y=-13, c_black};
    ob_playerBox[i++] = (point) {.x=1, .y=-13, c_black};
    ob_playerBox[i++] = (point) {.x=2, .y=-13, c_black};

    ob_playerBox[i++] = p_termPoint;

    ob_levelTest = salloc(sizeof(int) * k_nMaxObj);
    i=0;
    ob_levelTest[i++] = objPlayer;
    ob_levelTest[i++] = -5;
    ob_levelTest[i++] = 14;
    while(i/3<30){
        ob_levelTest[i++] = objGround;
        ob_levelTest[i++] = (i/3-15)*16;
        ob_levelTest[i++] = -17;
    }
    while(i/3<30+4){
        ob_levelTest[i++] = objBrick;
        ob_levelTest[i++] = 16;
        ob_levelTest[i++] = (i/3-30)*16;
    }
    ob_levelTest[i++] = terminator;

}

void ob_deinit() {
    free(ob_brick);
    free(ob_levelTest);
    free(ob_nothing);
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

void ob_realifyPoint(point *p, int *pos){
    (*p).x += pos[0];
    (*p).y += pos[1];
    return;
}

float f_round(float f){
    return( (((int) (f*10 + ((f > 0) ? 0.5: -0.5))))/10.0);
}

bool ob_intersect(line l1, line l2, float* x, float* y){ //returns whether or not there was an intersection
    //if( (l1.y1 == l1.y2 && l1.x1 == l1.x2) || (l2.y1 == l2.y2 && l2.x1 == l2.x2) ) {printf("Error in ob_intersect(): Argument not a line\n"); exit(1);}
    float d;
    if( f_round(d = (l1.x1-l1.x2)*(l2.y1-l2.y2) - (l1.y1-l1.y2)*(l2.x1-l2.x2)) == 0 ) {
        if(l1.x1 * (l1.y2-l2.y1) + l1.x2 * (l2.y1 - l1.y1) + l2.x1 * (l1.y1 - l1.y2) == 0){ //area of a triangle
            int xs[4] = {l1.x1, l1.x2, l2.x1, l2.x2};
            int ys[4] = {l1.y1, l1.y2, l2.y1, l2.y2};
            intsort(xs, 4);
            intsort(ys, 4);
            *x = (xs[2] + xs[1]) / 2.0;
            *y = (ys[2] + ys[1]) / 2.0;
            return true;
        }
        else{return false;}
        return false;
    }

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

    return sqrt(pow(x-c.x1, 2) + pow(y-c.y1, 2));
}

float ob_slope(line l){
    if(l.x1==l.x2){printf("Error in ob_slope(): Dx = 0"); exit(1);}
    return ( (float) l.y2 - l.y1) / (l.x2 - l.x1);
}

float ob_len(line l){
    return sqrt(pow(l.x2-l.x1, 2) + pow(l.y2-l.y1, 2));
}

bool ob_isTerminating(line l){
    return ! (bool) (l.x1 | l.x2 | l.y1 | l.y2);
}

line* objFtype_old(enum objType type) {
    switch(type) {
        case objGround:
            return ob_ground_old;
            break;
        case nothing:
            return ob_nothing_old;
            break;
        case objPlayer:
            return ob_playerBox_old;
            break;
        case objBrick:
            return ob_brick_old;
            break;
        case objCoin:
            return ob_coin_old;
            break;
        default:
            printf("Error in objFtype_old(): No object data found for object type %d", type);
            exit(1);
            break;
    }
}

bool ob_p_isTerm(point p){
    return (p.x == 124214 && p.y == 143512 && p.r == 100 && p.g == 100 && p.b == 100);
}

point* objFtype(enum objType type) {
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
            printf("Error in objFtype(): No object data found for object type %d", type);
            exit(1);
            break;
    }
}
