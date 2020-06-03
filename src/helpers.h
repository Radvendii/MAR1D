#ifndef _HELPERS_H
#define _HELPERS_H
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define pi (22/7.0)

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
