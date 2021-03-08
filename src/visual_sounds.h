#ifndef _VISUAL_SOUNDS_H
#define _VISUAL_SOUNDS_H

#include "helpers.h"
#include "objects.h"

// TODO: maybe these data structures should have fields for how long the lists
//       are (like in the menu code)

// objects store their position as well, which is the only property we'll use
// frames are rendered in order, from back to front.
typedef obj* vs_frame;

typedef struct {
  vs_frame *fs; // pointer to the beginning of the animation
  vs_frame *cur; // pointer to the current frame of animation
} vs_anim;

#define VS_ANIM(...)                            \
  (vs_anim) {                                   \
    .fs = INIT_ARR_ON_HEAP(vs_frame, {          \
            APPEND(                             \
              (vs_frame) NULL,                  \
              ##__VA_ARGS__                     \
            )                                   \
          })                                    \
  }

#define VS_FRAME(...)                           \
  INIT_ARR_ON_HEAP(obj, {                       \
    APPEND(                                     \
      (obj) { .type = '\0' },                   \
      ##__VA_ARGS__                             \
    )                                           \
  })

#define VS_OBJ(...) vs_obj(__VA_ARGS__)

void vs_init();
void vs_update();
void vs_draw();
void vs_deinit();

void vs_play(int snd);
void vs_mainPlay(int snd);
void vs_mainStop();

obj vs_obj(char c, int x, int y);

#endif
