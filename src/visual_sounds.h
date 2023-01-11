#ifndef _VISUAL_SOUNDS_H
#define _VISUAL_SOUNDS_H

#include "helpers.h"
#include "objects.h"

#define VS_ANIM(...)                                                 \
  (vs_anim) {                                                        \
    .nFs = sizeof((vs_frame[]) { __VA_ARGS__ }) / sizeof(vs_frame),  \
    .cur = -1,                                                       \
    .fs = INIT_ARR_ON_HEAP(vs_frame, { __VA_ARGS__ })                \
  }

#define VS_FRAME(...)                                                \
  INIT_ARR_ON_HEAP(vs_obj, {                                         \
    APPEND(                                                          \
      (vs_obj) { .type = '\0' },                                     \
      ##__VA_ARGS__                                                  \
    )                                                                \
  })

#define VS_OBJ(_type, _x, _y) ((vs_obj) { .type = _type, .x = _x, .y = _y })

// a much simplified object. we can get the detailed data from io_os when we need it; no need to duplicate.
typedef struct {
  char type;
  int x;
  int y;
} vs_obj;

typedef vs_obj* vs_frame;

typedef struct {
  vs_frame *fs; // pointer to the beginning of the animation
  int nFs; // number of frames in the animation
  int cur; // each object steps through an animation
} vs_anim;

void vs_init();
void vs_update();
void vs_draw();
void vs_deinit();

void vs_play(int snd);
void vs_mainPlay(int snd);
void vs_mainStop();

#endif
