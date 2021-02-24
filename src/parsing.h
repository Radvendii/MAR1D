#ifndef _PARSING_H
#define _PARSING_H
#include "helpers.h"
#include "objects.h"
#include "resources.h"
#include "graphics.h"
#include "lodepng.h"
#include <libconfig.h>

extern color* io_cs;
extern obj* io_os;

// TODO: not all of these should be public functions

int io_getFont(bool**, char*);

void io_getColor(FILE*, color*);

void io_getLevel(FILE*, level*, obj[127]);

void io_getObj(FILE*, obj*, char, color[127]);

// TODO: this is named terribly. There should be another function io_readLevels that's exposed. this should be private and take in FILE*
void io_getLevels(level**, char*);

void io_readConfig(config *);
void io_writeConfig(config);

image io_getImage(char*);

void io_recAddFrame(unsigned char *frame);
void io_resetRec();
void io_writeRec(char *fn);
image io_flippedImage(image);
void io_writeImPng(const char *fn, image);

#endif
