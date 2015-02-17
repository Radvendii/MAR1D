#include "parsing.h"

FILE* io_readFile(char* fn){
    char fn_[100];
    sprintf(fn_, "%s%s", "../", fn);
    return sfopen(fn_, "r");
}

int io_getFont(bool** font, char* fn){
    FILE* f = io_readFile(fn);
    int c;
    int i=0;
    int fontSize;
    fscanf(f, "%i\n", &fontSize);
    *font = resalloc(*font, sizeof(bool)*fontSize*43);
    while((c = fgetc(f)) != EOF){
        if(c == '\n'){continue;}
        else{(*font)[i++]= (c == '1' ? true : false);}
    }
    sfclose(f);
    return fontSize;
}

void io_getColor(FILE* f, color cs[127]){//cs must be an array 127 big
    char cname;
    color c;
    fscanf(f, "%c:", &cname);
    fscanf(f, "%hhi.%hhi.%hhi\n", &cs[cname].r, &cs[cname].g, &cs[cname].b);
    return;
}

void io_getObj(FILE* f, obj os[127], color cs[127]){
    char oname;
    int size;
    int w, h;
    int nCols;
    int xpos, ypos, xhei, yhei;
    int x,y,i;
    char c;
    fscanf(f, "%c; ps:%i; dim:%ix%i; cols:%i;", &oname, &size, &w, &h, &nCols);
    os[oname].type[0] = oname;
    os[oname].type[1] = '\0';
    os[oname].x = 0;
    os[oname].y = 0;
    os[oname].vx = 0;
    os[oname].vy = 0;
    os[oname].i = 0;
    os[oname].bb = (box) {.x = 0, .y = 0, .w = w, .h = -h};
    os[oname].ps = resalloc(os[oname].ps, sizeof(point) * (size+1));
    os[oname].cols = salloc(sizeof(box) * nCols);
    os[oname].nCols = nCols;
    i=0;
    while(i<nCols){
        fscanf(f, "\nC; pos:%i,%i; dim:%ix%i;", &xpos, &ypos, &xhei, &yhei);
        os[oname].cols[i++] = (box) {.x = xpos, .y = -ypos, .w = xhei, .h = -yhei};
    }
    y=0; x=0; i=0;
    while(i<size){
        switch(c = fgetc(f)){
            case ' ':
                x++;
                break;
            case '\n':
                y--;
                x=0;
                break;
            default:
                os[oname].ps[i++] = (point) {.x = x, .y = y, .r = cs[c].r, .g = cs[c].g, .b = cs[c].b};
                x++;
                break;
        }
    }
    os[oname].ps[i] = p_termPoint;
}

void io_getLevel(FILE* f, level ls[127], obj os[127]){
    char lname;
    int size;
    char c;
    int y=0,x=0,i=0;
    fscanf(f, "%c:%i", &lname, &size);
    ls[lname] = resalloc(ls[lname], sizeof(obj) * (size*3+1));
    while(i<size){
        switch(c = fgetc(f)){
            case ' ':
                x++;
                break;
            case '\n':
                y--;
                x=0;
                break;
            default:
                ls[lname][i] = os[c];
                ls[lname][i].x = x*16;
                ls[lname][i].y = y*16;
                i++;
                x++;
                break;
        }
    }
    ls[lname][i].type[0] = '\0';
    ls[lname][i].type[1] = '\0';
}

void io_getLevels(level** ls, char* fn){
    FILE *f = io_readFile(fn);
    io_os = salloc(sizeof(obj) * 127);
    io_cs = salloc(sizeof(color)*127);
    *ls = resalloc(*ls, sizeof(level)*127);
    char c;
    for(int i=0;i<127;i++){(*ls)[i] = NULL;}
    while((c = fgetc(f)) != EOF){
        switch(c){
            case '\n':
                break;
            case 'C':
                io_getColor(f, io_cs);
                break;
            case 'O':
                io_getObj(f, io_os, io_cs);
                break;
            case 'L':
                io_getLevel(f, *ls, io_os);
                break;
            default:
                while(fgetc(f) != '\n');
                break;
        }
    }
    return;
}

