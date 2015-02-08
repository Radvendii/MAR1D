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

void io_getColors(color** cs, char *fn){
    FILE *f = io_readFile(fn);
    char c;
    *cs = resalloc(*cs, sizeof(color)*127);
    while((c = fgetc(f)) != EOF){
        switch(c){
            case '\n':
                break;
            case 'C':
                io_getColor(f, *cs);
                break;
            default:
                while(fgetc(f) != '\n');
                break;
        }
    }
    return;
}

void io_getPObj(FILE* f, color cs[127], pObj os[127]){
    char oname;
    char c;
    int size;
    fscanf(f, "%c:%i/%*ix%*i", &oname, &size);
    os[oname] = resalloc(os[oname], sizeof(point) * (size+1));
    int y=0, x=0, i=0;
    while(i<size){
        switch(c = fgetc(f)){
            case ' ':
                x++;
                break;
            case '\n':
                y--;
                x=0;
                break;
            default: //TODO: locally defined colors
                os[oname][i++] = (point) {.x = x, .y = y, .r = cs[c].r, .g = cs[c].g, .b = cs[c].b};
                x++;
                break;
        }
    }
    os[oname][i] = p_termPoint;
}

void io_getPObjs(pObj** os, color* cs, char* fn){
    FILE *f = io_readFile(fn);
    char c;
    color cs_l[127];
    *os = resalloc(*os, sizeof(pObj)*127);
    for(int i=0;i<127;i++){(*os)[i] = NULL;}
    while((c = fgetc(f)) != EOF){
        switch(c){
            case '\n':
                break;
            case 'C':
                io_getColor(f, cs_l);
                break;
            case 'O':
                io_getPObj(f, cs, *os);
                break;
            default:
                while(fgetc(f) != '\n');
                break;
        }
    }
    return;
}

void io_getBox(FILE* f, box bs[127]){
    char oname;
    int w, h;
    fscanf(f, "%c:%*i/%ix%i\n", &oname, &w, &h);
    bs[oname] = (box) {.x = 0, .y = 0, .w = w, .h = -h};
}

void io_getBoxes(box** bs, char* fn){
    FILE *f = io_readFile(fn);
    char c;
    *bs = resalloc(*bs, sizeof(box)*127);
    while((c = fgetc(f)) != EOF){
        switch(c){
            case '\n':
                break;
            case 'O':
                io_getBox(f, *bs);
                break;
            default:
                while(fgetc(f) != '\n');
                break;
        }
    }
    return;
}

void io_getLevel(FILE* f, level ls[127]){
    char lname;
    char c;
    int size;
    fscanf(f, "%c:%i", &lname, &size);
    ls[lname] = resalloc(ls[lname], sizeof(int) * (size*3+1));
    int y=0, x=0, i=0;
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
                ls[lname][i*3] = c;
                ls[lname][i*3+1] = x*16;
                ls[lname][i*3+2] = y*16;
                i++;
                x++;
                break;
        }
    }
    ls[lname][i*3] = '\0';
}

void io_getLevels(level** ls, char* fn){
    FILE *f = io_readFile(fn);
    char c;
    *ls = resalloc(*ls, sizeof(level)*127);
    for(int i=0;i<127;i++){(*ls)[i] = NULL;}
    while((c = fgetc(f)) != EOF){
        switch(c){
            case '\n':
                break;
            case 'L':
                io_getLevel(f, *ls);
                break;
            default:
                while(fgetc(f) != '\n');
                break;
        }
    }
    return;
}
