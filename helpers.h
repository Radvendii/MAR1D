#ifndef _HELPERS_H
#define _HELPERS_H
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define pi (22/7.0)

FILE* sfopen(char*, char*);
void sfclose(FILE*);

void* salloc(size_t);
void* resalloc(void*, size_t);
void intsort(int*, int);

#endif
