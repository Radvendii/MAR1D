#ifndef _PARSING_H
#define _PARSING_H
#include "helpers.h"
#include "objects.h"

color* io_cs;
obj* io_os;

typedef struct {
    unsigned long sizeX;
    unsigned long sizeY;
    unsigned char *data;
} image;

FILE* io_readFile(char*);
int io_getFont(bool**, char*);

void io_getColor(FILE*,color[127]);

void io_getLevel(FILE*, level[127], obj[127]);

void io_getObj(FILE*, obj[127], color[127]);
void io_getLevels(level**, char*);

image* loadTexture();
int getImage(image*, char*);

#endif
