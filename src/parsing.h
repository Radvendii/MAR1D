#ifndef _PARSING_H
#define _PARSING_H
#include "helpers.h"
#include "objects.h"
#include "resources.h"
#ifdef _WIN32
#define LIBCONFIG_STATIC
#endif
#include <libconfig.h>

color* io_cs;
obj* io_os;

typedef struct {
  unsigned long sizeX;
  unsigned long sizeY;
  unsigned char *data;
  int texture;
} image;

// TODO: not all of these should be public functions

int io_getFont(bool**, char*);

void io_getColor(FILE*, color[127]);

void io_getLevel(FILE*, level[127], obj[127]);

void io_getObj(FILE*, obj[127], color[127]);

// TODO: this is named terribly. There should be another function io_readLevels that's exposed. this should be private and take in FILE*
void io_getLevels(level**, char*);

void io_readConfig(config *);
void io_writeConfig(config);

image io_getImage(char*);

#endif
