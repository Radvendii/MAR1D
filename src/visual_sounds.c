#include "visual_sounds.h"
#include "audio.h"
#include "objects.h"
#include "parsing.h"
#include "rendering.h"

// TODO: maybe these data structures should have fields for how long the lists
//       are (like in the menu code)

// objects store their position as well, which is the only property we'll use
// frames are rendered in order, from back to front.
typedef obj* vs_frame;

typedef struct {
  vs_frame *fs; // pointer to the beginning of the animation
  vs_frame *cur; // pointer to the current frame of animation
} vs_anim;

vs_anim vs_sounds[k_nSounds];

int vs_mainVisual;

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
  INIT_ARR_ON_HEAP(obj, {                    \
    APPEND(                                     \
      (obj) { .type = '\0' },                   \
      ##__VA_ARGS__                             \
    )                                           \
  })

#define VS_OBJ(...) vs_obj(__VA_ARGS__)

/* vs_frame vs_frame(int n, ...) { */
/*   va_list ap; */
/*   va_start(ap, dummy); */

/*   vs_frame ret = salloc((n + 1) * sizeof(obj)); */

/*   char c; */
/*   int x; */
/*   int y; */

/*   for (int i=0; i<n; i++) { */
/*     c = va_arg(ap, char); */
/*     x = va_arg(ap, int); */
/*     y = va_arg(ap, int); */

/*     ret[i] = vs_obj(c, x, y); */
/*   } */
/*   ret[n].type = '\0'; */

/*   va_end(ap); */
/*   retrun ret; */
/* } */

obj vs_obj(char c, int x, int y) {
  obj ret = io_os[c];
  ret.x = x;
  ret.y = y;
  return ret;
}

void vs_init() {
  for (int i=0; i < k_nSounds; i++) {
    vs_sounds[i].cur = NULL;
    vs_sounds[i].fs = NULL;
  }
  vs_mainVisual = SND_none;

  vs_sounds[SND_jump] = VS_ANIM(
    VS_FRAME( VS_OBJ('@', 0, 0) ),
    VS_FRAME( VS_OBJ('@', 0, 5) ),
    VS_FRAME( VS_OBJ('@', 0, 10) ),
    VS_FRAME( VS_OBJ('@', 0, 15) ),
    VS_FRAME( VS_OBJ('@', 0, 20) )
  );
}

void vs_play(int snd) {
  vs_sounds[snd].cur = vs_sounds[snd].fs; // set current frame to first frame
}

void vs_mainPlay(int snd) {
  vs_mainStop();
  vs_mainVisual = snd;
}

void vs_mainStop() {
  vs_sounds[vs_mainVisual].cur = NULL;
  vs_mainVisual = SND_none;
}

void vs_draw() {
  for (int i=0; i < k_nSounds; i++) {
    if (!vs_sounds[i].cur) { // not playing
      continue;
    }
    // draw vs_sounds[i].cur
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    // shifted over so that positive numbers land to the right of the game area
    glOrtho(-k_drawD2 - conf.lineSize, k_drawD2 - conf.lineSize, -k_drawD2, k_drawD2, -1, 1);
    for (obj *o = *vs_sounds[i].cur; o->type != '\0'; o++) {
        gr_image(&o->frames[0].im, RECT_LTWH(o->x + o->bb.x, o->y + o->bb.y, o->bb.w, -(o->bb.h)));
    }
    glPopMatrix();
  }
}

void vs_update() {
  for (int i=0; i < k_nSounds; i++) {
    if (!vs_sounds[i].cur) { // not playing
      continue;
    }

    // increment the animation
    vs_sounds[i].cur++;
    if (*(vs_sounds[i].cur) == NULL) { // we've reached the end of the animation
      if (i == vs_mainVisual) { // this is the main audio loop
        vs_sounds[i].cur = vs_sounds[i].fs; // restart the animation
      }
      else {
        vs_sounds[i].cur = NULL; // we're done
      }
    }
  }
}

void vs_deinit() {
  for (int i=0; i < k_nSounds; i++) {
    for (vs_frame *f = vs_sounds[i].fs; f && *f; f++) {
      free(*f);
      *f = NULL;
    }
    free(vs_sounds[i].fs);
    vs_sounds[i].cur = NULL;
    vs_sounds[i].fs = NULL;
  }
}
