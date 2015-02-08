#ifndef _PARSING_H
#define _PARSING_H
#include "helpers.h"
#include "objects.h"

FILE* io_readFile(char*);
int io_getFont(bool**, char*);
void io_getColors(color**, char*);
void io_getPObjs(pObj**, color*, char*);

void io_getColor(FILE*,color[127]);
void io_getPObj(FILE*, color[127], pObj[127]);

void io_getBox(FILE* f, box[127]);
void io_getBoxes(box** bs, char* fn);

#endif
