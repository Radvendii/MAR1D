#ifndef _HELPERS_H
#define _HELPERS_H
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define pi (22/7.0)

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define BOUND_BELOW(x, min) if ( (x) < (min) ) { (x) = (min); }
#define BOUND_ABOVE(x, max) if ( (x) > (max) ) { (x) = (max); }

// precondition: min <= max
// postcondition: min <= x <= max
#define BOUND(x, min, max)                        \
     if ( (x) < (min) ) { (x) = (min); }          \
     else if ( (x) > (max) ) { (x) = (max); }

// TODO: `volume`, `volumeEffects`, `volumeMusic` instead of `mute` and `effects`
typedef struct {
  bool mute;
  bool effects;
  int lineSize;
  int sensitivity;
  bool reverseMouseY;
} config; // not to be confused with config_t from libconfig

config conf;

bool quit; // global variable for exiting all loops and quitting the game

int err; // global variable for capturing error codes

// Safe versions of various functions (checks for errors)
FILE* sfopen(char*, char*);
void sfclose(FILE*);

void* salloc(size_t);
void* resalloc(void*, size_t);

void intsort(int*, int); // Sorts a list of ints with specified length.
                         // This is never used and I'm not sure why it's here.

double linInterp(double, double, double, double, double);

#endif
