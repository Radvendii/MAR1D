#ifndef _HELPERS_H
#define _HELPERS_H
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

// switch the DEBUG(...) definitions to enable / disable debugging logs
#define DEBUG(...)                              // DEBUG OFF
#undef  DEBUG
#define DEBUG(...) fprintf(stderr, __VA_ARGS__) // DEBUG ON

// sprinkle this into the code to see where things are going wrong
#define LINENO DEBUG("%s(%d)\n", __FILE__, __LINE__)

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

#define SQUARE(x) ((x) * (x))

#define SGN(x) ((x) > 0 - (x) < 0)

/*
 * arrays can be initialized with int[] foo = {1, 2, 3} syntax, but this puts
 * the array on the stack. This macro allows you to use similar syntax to
 * initialize an array on the heap.
 *
 * note that type is not the array or pointer type, but the type of the elements
 *
 * NOTE: If this becomes useful and commonly used, it would be good to find a
 * shorter name for it
 *
 * invoked as
 * int *foo = INIT_ARR_ON_HEAP(int, {1, 2, 3});
 */
#define INIT_ARR_ON_HEAP(type, ...) _INIT_ARR_ON_HEAP(type, ((type[]) __VA_ARGS__))
#define _INIT_ARR_ON_HEAP(type, arr) (type *) heap_copy(arr, sizeof(arr))

// TODO: `volume`, `volumeEffects`, `volumeMusic` instead of `mute` and `effects`
typedef struct {
  int music;
  int effects;
  int lineSize;
  int sensitivity;
  bool invertMouseY;
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

/*
 * makes a copy of obj on the heap, and returns a pointer to the copy.
 * size is the size of the object
 */
void *heap_copy(void *obj, size_t size);

double linInterp(double, double, double, double, double);
double subLinInterp(double, double, double, double, double);
double smoothInterp(double, double, double, double, double);

#endif
