#ifndef _HELPERS_H
#define _HELPERS_H
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <SDL.h>
#include <SDL_mixer.h>
#ifndef _WIN32
#include <syslog.h>
#endif

// comment / uncomment the DEBUG(...) definitions to change how debugs are logged (or not at all)
// NOTE: syslog will not work on windows
// #define DEBUG(...)
// #define DEBUG(s, ...) syslog(LOG_ERR, "%s(%d) "s"\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define DEBUG(s, ...) fprintf(stderr, "%s(%d) "s"\n", __FILE__, __LINE__, ##__VA_ARGS__)

// sprinkle this into the code to see where things are going wrong
#define LINENO DEBUG("")

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

#define SGN(x) (((x) > 0) - ((x) < 0))

#define k_safeDelayStep 200

#define SAFE_DELAY(time)                                \
  for(int t=0; t<(time) && !quit; t+=k_safeDelayStep) { \
    SDL_Delay(k_safeDelayStep);                         \
    wn_processEvents();                                 \
  }

#define SAFE_DELAY_UNTIL(cond)                  \
  while(!(cond) && !quit) {                     \
    SDL_Delay(k_safeDelayStep);                 \
    wn_processEvents();                         \
  }

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

/*
 * APPEND(a, b, c) will produce b, c, a
 * APPEND(a) will produce a
 *
 * this is useful when called in other macros as follows:
 * APPEND(a, ##__VA_ARGS__)
 *
 * because there is no easy way to append something to __VA_ARGS__ while dealing
 * correctly with commas and the case of __VA_ARGS__ being empty
 *
 * watch out for ##__VA_ARGS__ blocking macro expansion
 */
#define APPEND(...) APPEND_EXPAND(__VA_ARGS__)
#define APPEND_EXPAND(...) APPEND_EXPANDED(__VA_ARGS__)
#define APPEND_EXPANDED(x, ...) DROP_ONE(dummy, ##__VA_ARGS__, x)
#define DROP_ONE(first, ...) __VA_ARGS__

// requires that the keycode by bound to key and the state be bound to state
#define KEY_DN(k) (key == conf.keys.k && state == SDL_PRESSED)
#define KEY_UP(k) (key == conf.keys.k && state == SDL_RELEASED)

#define k_defaultKeys                   \
  (keybinds) {                          \
    .forward = SDLK_w,                  \
    .backward = SDLK_s,                 \
    .turn = SDLK_d,                     \
    .run = SDLK_LSHIFT,                 \
    .crouch = SDLK_LCTRL,               \
    .jump = SDLK_SPACE                  \
  }

#define k_defaultConf                   \
  (config) {                            \
    .music = MIX_MAX_VOLUME,            \
    .effects = MIX_MAX_VOLUME,          \
    .lineSize = 30,                     \
    .sensitivity = 10,                  \
    .invertMouseY = false,              \
    .keys = k_defaultKeys,              \
    .debug = false                      \
  }

typedef struct {
  SDL_Keycode forward;
  SDL_Keycode backward;
  SDL_Keycode turn;
  SDL_Keycode run;
  SDL_Keycode crouch;
  SDL_Keycode jump;
} keybinds;

typedef struct {
  int music;
  int effects;
  int lineSize;
  int sensitivity;
  bool invertMouseY;
  keybinds keys;
  bool debug;
} config; // not to be confused with config_t from libconfig

typedef struct {
  unsigned int sizeX;
  unsigned int sizeY;
  unsigned char *data;
  unsigned int texture;
} image;

extern config conf;

extern bool quit; // global variable for exiting all loops and quitting the game

extern int err; // global variable for capturing error codes

char *rprintf(char *, ...);

void sfscanf(FILE*, char*, ...);

// Safe versions of various functions (checks for errors)
FILE* sfopen(char*, char*);
void sfclose(FILE*);

void* salloc(size_t);
void* resalloc(void**, size_t);

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
