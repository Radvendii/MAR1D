#include "graphics.h"

void gr_update(){
    //rn_dimFworld_old(&dimScreen_old, s.world);
    //rn_perspFworld_v_old(perspScreen, s.world, &dimScreen_old);
    //if(!debug){dimScreen_old[0] = (line) {.x1 = 0, .x2 = 0, .y1 = 0, .y2 = 0, .r = 0, .g = 0, .b = 0};}
    rn_dimFworld(dimScreen, s.world);
    rn_perspFworld_v(perspScreen, s.world, NULL);
    if(s.camFlip){
        unsigned char swap_r;
        unsigned char swap_g;
        unsigned char swap_b;
        for(int i=0;i<k_nPixels/2;i++){
            swap_r = perspScreen[i*3];
            swap_g = perspScreen[i*3+1];
            swap_b = perspScreen[i*3+2];
            perspScreen[i*3] = perspScreen[3*k_nPixels-(1+i)*3];
            perspScreen[i*3+1] = perspScreen[3*k_nPixels-(1+i)*3+1];
            perspScreen[i*3+2] = perspScreen[3*k_nPixels-(1+i)*3+2];
            perspScreen[3*k_nPixels-(1+i)*3] = swap_r;
            perspScreen[3*k_nPixels-(1+i)*3+1] = swap_g;
            perspScreen[3*k_nPixels-(1+i)*3+2] = swap_b;
        }
    }
}

void gr_char(char c, GLfloat* x, GLfloat* y){
    glPointSize(8.f);
    glBegin(GL_POINTS);
    for(int i=0;i<5*7;i++){
        if(font[(c-48)*5*7+i]){
        glVertex2f(*x+i%5*5, *y-i/5*5);
        }
    }
    glEnd();
    *x += 5*6;
}

void gr_text(char *s, GLfloat x, GLfloat y){
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-k_drawD, k_drawD, -k_drawD, k_drawD, -1, 1);
    while(*s != '\0'){
        gr_char(*s, &x, &y);
        s++;
    }
    glPopMatrix();
}

void gr_pixel(int y, unsigned char r, unsigned char g, unsigned char b){
    glBegin(GL_QUADS);
    glColor3ub(r, g, b);
    glVertex2f(0, y);
    glVertex2f(0, y+1);
    glVertex2f(1, y+1);
    glVertex2f(1, y);
    glEnd();
}

void gr_pixels(unsigned char *renderArr){
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 1, 0, k_nPixels, -1, 1);

    for(int y=0;y<k_nPixels;y++){
        gr_pixel(y, renderArr[y*3+0], renderArr[y*3+1], renderArr[y*3+2]);
    }
    glPopMatrix();
}

void gr_lines(line *ls){
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-k_drawD, k_drawD, -k_drawD, k_drawD, -1, 1);

    glBegin(GL_LINES);
    for(int i=0;;i++){
        if(ob_isTerminating(ls[i])){break;}
        glColor3ub(ls[i].r, ls[i].g, ls[i].b);
        glVertex2f(ls[i].x1, ls[i].y1);
        glVertex2f(ls[i].x2, ls[i].y2);
    }
    glEnd();
    glPopMatrix();
    return;
}

void gr_points(point *ps){
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-k_drawD, k_drawD, -k_drawD, k_drawD, -1, 1);

    glPointSize(330.0/k_drawD);
    glBegin(GL_POINTS);
    for(int i=0;;i++){
        if(ob_p_isTerm(ps[i])){break;}
        glColor3ub(ps[i].r, ps[i].g, ps[i].b);
        glVertex2f(ps[i].x, ps[i].y);
    }
    glEnd();
    glPopMatrix();
    return;
}

void gr_draw(GLFWwindow *window, int renderType){
    glClear(GL_COLOR_BUFFER_BIT);
    if(renderType == 0){
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glBegin(GL_QUADS);
        glColor3ub(107, 136, 255);
        glVertex2f(-1.f, -1.f);
        glVertex2f(-1.f, 1.f);
        glVertex2f(1.f, 1.f);
        glVertex2f(1.f, -1.f);
        glEnd();
        //gr_lines(dimScreen_old);
        gr_points(dimScreen);
        glColor3f(1.0, 1.0, 1.0);
        char score[100];
        sprintf(score, "%d", s.coins);
        gr_text(score, -k_drawD+5, k_drawD-5);
    }
    else{gr_pixels(perspScreen);}
}

void gr_init(){
    debug = true;
    perspScreen = salloc(sizeof(unsigned char)*k_nPixels*3);
    for(int i=0; i<k_nPixels*3; i++){perspScreen[i]=0;}
    //dimScreen_old = salloc(sizeof(line)*k_nMaxLinesPerObj * k_nMaxObj);
    //dimScreen_old[0] = k_termLine;
    dimScreen = salloc(sizeof(point)*500*k_nMaxObj);
    dimScreen[0] = p_termPoint;
    //init Font
    font = salloc(sizeof(bool)*5*7*43);
    FILE* f;
    if((f = fopen("../dots.font", "r")) == NULL){
        printf("Error in gr_init(): File not found: ../dots.font\n");
        exit(1);
    }
    int c;
    int i=0;
    while((c = fgetc(f)) != EOF){
        if(c == '\n'){continue;}
        else{font[i++]= (c == '1' ? true : false);}
    }
}

void gr_deinit(){
    free(perspScreen);
    //free(dimScreen_old);
    free(dimScreen);
    free(font);
}
