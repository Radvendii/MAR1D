#ifndef _PARSING_H
#define _PARSING_H
#include "helpers.h"
#include "objects.h"

FILE* io_readFile(char*);
int io_getFont(bool**, char*);
void io_getColors(color**, char*);

void io_getColor(FILE*,color[127]);

void io_getLevel(FILE*, level[127]);
void io_getLevels(level**, char*);

void io_getObjs(obj**, color[127], char*);

#endif
