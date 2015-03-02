#include "parsing.h"

//TODO: Animations not still images

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
    fscanf(f, "%d\n", &fontSize);
    *font = salloc(sizeof(bool)*fontSize*43);
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
    fscanf(f, "%hhd.%hhd.%hhd\n", &cs[cname].r, &cs[cname].g, &cs[cname].b);
    return;
}

void io_getObj(FILE* f, obj os[127], color cs[127]){ //Make objects have arrays of point arrays
    char oname;
    int nps;
    int size;
    int w, h;
    int nCols;
    int xpos, ypos, xhei, yhei;
    int grav;
    int phys;
    int hid;
    int x,y,i,j;
    char c;
    fscanf(f, "%c%d; ps:%d; pos:%d,%d; dim:%dx%d; cols:%d; grav:%d; phys:%d; hid:%d;", &oname, &nps, &size, &xpos, &ypos, &w, &h, &nCols, &grav, &phys, &hid);
    os[oname].type = oname;
    os[oname].gravity = grav;
    os[oname].physical = phys;
    os[oname].hidden = hid;
    os[oname].active = true;
    os[oname].x = 0;
    os[oname].y = 0;
    os[oname].vx = 0;
    os[oname].vy = 0;
    os[oname].i = 0;
    os[oname].j = 0;
    os[oname].c = 0;
    os[oname].flip = false;
    os[oname].nps = nps;
    os[oname].bb = (box) {.x = xpos, .y = -ypos, .w = w, .h = -h};
    os[oname].ps = salloc(sizeof(point*) * nps);
    os[oname].cols = salloc(sizeof(box) * nCols);
    os[oname].nCols = nCols;
    i=0;
    while(i<nCols){
        fscanf(f, "\nC; pos:%d,%d; dim:%dx%d;", &xpos, &ypos, &xhei, &yhei);
        os[oname].cols[i++] = (box) {.x = xpos, .y = -ypos, .w = xhei, .h = -yhei};
    }
    for(j=0;j<nps;j++){
        y=0; x=0; i=0;
        os[oname].ps[j] = salloc(sizeof(point) * (size+1));
        while(i<size){
            switch(c = fgetc(f)){
                case ' ':
                    x++;
                    break;
                case '\n':
                    y--;
                    x=0;
                    break;
                case '0':
                    os[oname].ps[j][i++] = p_skipPoint;
                    x++;
                    break;
                default:
                    os[oname].ps[j][i++] = (point) {.x = x, .y = y, .r = cs[c].r, .g = cs[c].g, .b = cs[c].b};
                    x++;
                    break;
            }
        }
    os[oname].ps[j][i] = p_termPoint;
    }
}

void io_getLevel(FILE* f, level ls[127], obj os[127]){
    char lname;
    int size;
    char c;
    int y=0,x=0,i=0;
    fscanf(f, "%c:%d", &lname, &size);
    ls[lname] = salloc(sizeof(obj) * (size*3+1));
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
                if(c == '&'){
                    ls[lname][i].y += 7;
                }
                if(c == '7'){
                    ls[lname][i].y -= 3;
                }
                if(c == '?'){
                    ls[lname][i].c = '.';
                    for(int j=0;j<i;j++){
                        if(ls[lname][j].x == ls[lname][i].x && ls[lname][j].y == ls[lname][i].y+16){
                            ls[lname][i].c = ls[lname][j].type;
                            ls[lname][j] = os['.'];
                            break;
                        }
                    }
                }
                if(c == '|' || c == '='){
                    fgetc(f);
                    ls[lname][++i] = os['.'];
                    x++;
                }
                i++;
                x++;
                break;
        }
    }
    ls[lname][i].type = '\0';
    char prop;
    char obj;
    int j;
    fgetc(f);
    while(fgetc(f) == 'P'){
        fscanf(f, ";%d%c", &i, &prop);
        switch(prop){
            case 'h':
                ls[lname][i].hidden = true;
                break;
            case 'v':
                fscanf(f, "%c", &obj);
                ls[lname][i].ps = os[obj].ps;
                ls[lname][i].nps = os[obj].nps;
                break;
            case 'j':
                fscanf(f, "%d", &j);
                ls[lname][i].j = j;
                break;
        }
        fgetc(f);
    }
}

void io_getLevels(level** ls, char* fn){
    FILE *f = io_readFile(fn);
    io_os = salloc(sizeof(obj) * 127);
    for(int i=0;i<127;i++){io_os[i].ps = NULL;}
    io_cs = salloc(sizeof(color)*127);
    *ls = salloc(sizeof(level)*127);
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

