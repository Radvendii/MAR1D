#include "visual_sounds.h"
#include "audio.h"
#include "parsing.h"
#include "rendering.h"

vs_anim vs_sounds[k_nSounds];

int vs_mainVisual;

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
    VS_FRAME( VS_OBJ('^', 5, 0) ),
    VS_FRAME( VS_OBJ('^', 5, 0) ),
    VS_FRAME( VS_OBJ('^', 5, 0) ),
    VS_FRAME( VS_OBJ('^', 5, 0) ),
    VS_FRAME( VS_OBJ('^', 5, 0) ),
    VS_FRAME( VS_OBJ('^', 5, 1) ),
    VS_FRAME( VS_OBJ('^', 5, 2) ),
    VS_FRAME( VS_OBJ('^', 5, 3) ),
    VS_FRAME( VS_OBJ('^', 5, 4) ),
    VS_FRAME( VS_OBJ('^', 5, 5) ),
    VS_FRAME( VS_OBJ('^', 5, 6) ),
    VS_FRAME( VS_OBJ('^', 5, 7) ),
    VS_FRAME( VS_OBJ('^', 5, 8) ),
    VS_FRAME( VS_OBJ('^', 5, 9) ),
    VS_FRAME( VS_OBJ('^', 5, 10) ),
    VS_FRAME( VS_OBJ('^', 5, 11) ),
    VS_FRAME( VS_OBJ('^', 5, 12) ),
    VS_FRAME( VS_OBJ('^', 5, 13) ),
    VS_FRAME( VS_OBJ('^', 5, 14) ),
    VS_FRAME( VS_OBJ('^', 5, 15) ),
    VS_FRAME( VS_OBJ('^', 5, 16) ),
    VS_FRAME( VS_OBJ('^', 5, 17) ),
    VS_FRAME( VS_OBJ('^', 5, 18) ),
    VS_FRAME( VS_OBJ('^', 5, 19) ),
    VS_FRAME( VS_OBJ('^', 5, 19) ),
    VS_FRAME( VS_OBJ('^', 5, 19) ),
    VS_FRAME( VS_OBJ('^', 5, 19) ),
    VS_FRAME( VS_OBJ('^', 5, 19) )
  );

  // I'm not going to bother making another object type for bigmario
  vs_sounds[SND_jumpbig] = VS_ANIM(
    VS_FRAME( VS_OBJ('^', 5, 0) ),
    VS_FRAME( VS_OBJ('^', 5, 0) ),
    VS_FRAME( VS_OBJ('^', 5, 0) ),
    VS_FRAME( VS_OBJ('^', 5, 0) ),
    VS_FRAME( VS_OBJ('^', 5, 0) ),
    VS_FRAME( VS_OBJ('^', 5, 1) ),
    VS_FRAME( VS_OBJ('^', 5, 2) ),
    VS_FRAME( VS_OBJ('^', 5, 3) ),
    VS_FRAME( VS_OBJ('^', 5, 4) ),
    VS_FRAME( VS_OBJ('^', 5, 5) ),
    VS_FRAME( VS_OBJ('^', 5, 6) ),
    VS_FRAME( VS_OBJ('^', 5, 7) ),
    VS_FRAME( VS_OBJ('^', 5, 8) ),
    VS_FRAME( VS_OBJ('^', 5, 9) ),
    VS_FRAME( VS_OBJ('^', 5, 10) ),
    VS_FRAME( VS_OBJ('^', 5, 11) ),
    VS_FRAME( VS_OBJ('^', 5, 12) ),
    VS_FRAME( VS_OBJ('^', 5, 13) ),
    VS_FRAME( VS_OBJ('^', 5, 14) ),
    VS_FRAME( VS_OBJ('^', 5, 15) ),
    VS_FRAME( VS_OBJ('^', 5, 16) ),
    VS_FRAME( VS_OBJ('^', 5, 17) ),
    VS_FRAME( VS_OBJ('^', 5, 18) ),
    VS_FRAME( VS_OBJ('^', 5, 19) ),
    VS_FRAME( VS_OBJ('^', 5, 19) ),
    VS_FRAME( VS_OBJ('^', 5, 19) ),
    VS_FRAME( VS_OBJ('^', 5, 19) ),
    VS_FRAME( VS_OBJ('^', 5, 19) )
  );

  vs_sounds[SND_coin] = VS_ANIM(
    VS_FRAME( VS_OBJ('c', 5, 20+16) ),
    VS_FRAME( VS_OBJ('c', 5, 20+16) ),
    VS_FRAME( VS_OBJ('c', 5, 20+16) ),
    VS_FRAME( VS_OBJ('c', 5, 20+16) ),
    VS_FRAME( VS_OBJ('c', 5, 20+16) ),
    VS_FRAME( VS_OBJ('c', 5, 20+16) ),
    VS_FRAME( VS_OBJ('c', 5, 20+16) ),
    VS_FRAME( VS_OBJ('c', 5, 20+16) ),
    VS_FRAME( VS_OBJ('c', 5, 20+16) ),
    VS_FRAME( VS_OBJ('c', 5, 20+16) )
  );

  vs_sounds[SND_stomp] = VS_ANIM(
    VS_FRAME( VS_OBJ('e', 0, 0), VS_OBJ('&', 10, 7) ),
    VS_FRAME( VS_OBJ('e', 0, 0), VS_OBJ('&', 10, 7) ),
    VS_FRAME( VS_OBJ('e', 0, 0), VS_OBJ('&', 10, 7) ),
    VS_FRAME( VS_OBJ('e', 0, 0), VS_OBJ('&', 10, 7) ),
    VS_FRAME( VS_OBJ('e', 0, 0), VS_OBJ('&', 10, 7) ),
    VS_FRAME( VS_OBJ('e', 0, 0), VS_OBJ('&', 10, 7) ),
    VS_FRAME( VS_OBJ('e', 0, 0), VS_OBJ('&', 10, 7) ),
    VS_FRAME( VS_OBJ('e', 0, 0), VS_OBJ('&', 10, 7) ),
    VS_FRAME( VS_OBJ('e', 0, 0), VS_OBJ('&', 10, 7) ),
    VS_FRAME( VS_OBJ('e', 0, 0), VS_OBJ('&', 10, 7) ),
    VS_FRAME( VS_OBJ('E', 0, -8), VS_OBJ('7', 10, -3) ),
    VS_FRAME( VS_OBJ('E', 0, -8), VS_OBJ('7', 10, -3) ),
    VS_FRAME( VS_OBJ('E', 0, -8), VS_OBJ('7', 10, -3) ),
    VS_FRAME( VS_OBJ('E', 0, -8), VS_OBJ('7', 10, -3) ),
    VS_FRAME( VS_OBJ('E', 0, -8), VS_OBJ('7', 10, -3) ),
    VS_FRAME( VS_OBJ('E', 0, -8), VS_OBJ('7', 10, -3) ),
    VS_FRAME( VS_OBJ('E', 0, -8), VS_OBJ('7', 10, -3) ),
    VS_FRAME( VS_OBJ('E', 0, -8), VS_OBJ('7', 10, -3) ),
    VS_FRAME( VS_OBJ('E', 0, -8), VS_OBJ('7', 10, -3) ),
    VS_FRAME( VS_OBJ('E', 0, -8), VS_OBJ('7', 10, -3) )
  );

  vs_sounds[SND_blockbreak] = VS_ANIM(
    VS_FRAME( VS_OBJ('#', 5, 20) ),
    VS_FRAME( VS_OBJ('#', 5, 20) ),
    VS_FRAME( VS_OBJ('#', 5, 20) ),
    VS_FRAME( VS_OBJ('#', 5, 20) ),
    VS_FRAME( VS_OBJ('#', 5, 20) ),
    VS_FRAME( VS_OBJ('#', 5, 20) ),
    VS_FRAME( VS_OBJ('#', 5, 20) ),
    VS_FRAME( VS_OBJ('#', 5, 20) ),
    VS_FRAME( VS_OBJ('#', 5, 20) ),
    VS_FRAME( VS_OBJ('#', 5, 20) ),
    VS_FRAME(
      VS_OBJ('b', 5, 22),
      VS_OBJ('b', 5, 14),
      VS_OBJ('b', 5, 22),
      VS_OBJ('b', 5, 14)
    ),
    VS_FRAME(
      VS_OBJ('b', 5, 22),
      VS_OBJ('b', 5, 14),
      VS_OBJ('b', 5, 22),
      VS_OBJ('b', 5, 14)
    ),
    VS_FRAME(
      VS_OBJ('b', 4, 21),
      VS_OBJ('b', 4, 13),
      VS_OBJ('b', 6, 21),
      VS_OBJ('b', 6, 13)
    ),
    VS_FRAME(
      VS_OBJ('b', 4, 21),
      VS_OBJ('b', 4, 13),
      VS_OBJ('b', 6, 21),
      VS_OBJ('b', 6, 13)
    ),
    VS_FRAME(
      VS_OBJ('b', 3, 20),
      VS_OBJ('b', 3, 11),
      VS_OBJ('b', 7, 20),
      VS_OBJ('b', 7, 11)
    ),
    VS_FRAME(
      VS_OBJ('b', 3, 20),
      VS_OBJ('b', 3, 11),
      VS_OBJ('b', 7, 20),
      VS_OBJ('b', 7, 11)
    ),
    VS_FRAME(
      VS_OBJ('b', 2, 18),
      VS_OBJ('b', 2, 9),
      VS_OBJ('b', 8, 18),
      VS_OBJ('b', 8, 9)
    ),
    VS_FRAME(
      VS_OBJ('b', 1, 16),
      VS_OBJ('b', 1, 7),
      VS_OBJ('b', 9, 16),
      VS_OBJ('b', 9, 7)
    ),
    VS_FRAME(
      VS_OBJ('b', 0, 13),
      VS_OBJ('b', 0, 4),
      VS_OBJ('b', 10, 13),
      VS_OBJ('b', 10, 4)
    )
  );
  vs_sounds[SND_pipe] = VS_ANIM(
    VS_FRAME( VS_OBJ('@', 6, 20), VS_OBJ('=', 0, 4), VS_OBJ('|', 0, -12),
              VS_OBJ('@', 25, 0), VS_OBJ(']', 41, 3), VS_OBJ('_', 57, 3)),
    VS_FRAME( VS_OBJ('@', 6, 20), VS_OBJ('=', 0, 4), VS_OBJ('|', 0, -12),
              VS_OBJ('@', 25, 0), VS_OBJ(']', 41, 3), VS_OBJ('_', 57, 3)),
    VS_FRAME( VS_OBJ('@', 6, 20), VS_OBJ('=', 0, 4), VS_OBJ('|', 0, -12),
              VS_OBJ('@', 25, 0), VS_OBJ(']', 41, 3), VS_OBJ('_', 57, 3)),
    VS_FRAME( VS_OBJ('@', 6, 20), VS_OBJ('=', 0, 4), VS_OBJ('|', 0, -12),
              VS_OBJ('@', 25, 0), VS_OBJ(']', 41, 3), VS_OBJ('_', 57, 3)),
    VS_FRAME( VS_OBJ('@', 6, 20), VS_OBJ('=', 0, 4), VS_OBJ('|', 0, -12),
              VS_OBJ('@', 25, 0), VS_OBJ(']', 41, 3), VS_OBJ('_', 57, 3)),
    VS_FRAME( VS_OBJ('@', 6, 20), VS_OBJ('=', 0, 4), VS_OBJ('|', 0, -12),
              VS_OBJ('@', 25, 0), VS_OBJ(']', 41, 3), VS_OBJ('_', 57, 3)),
    VS_FRAME( VS_OBJ('@', 6, 20), VS_OBJ('=', 0, 4), VS_OBJ('|', 0, -12),
              VS_OBJ('@', 25, 0), VS_OBJ(']', 41, 3), VS_OBJ('_', 57, 3)),
    VS_FRAME( VS_OBJ('@', 6, 20), VS_OBJ('=', 0, 4), VS_OBJ('|', 0, -12),
              VS_OBJ('@', 25, 0), VS_OBJ(']', 41, 3), VS_OBJ('_', 57, 3)),
    VS_FRAME( VS_OBJ('@', 6, 20), VS_OBJ('=', 0, 4), VS_OBJ('|', 0, -12),
              VS_OBJ('@', 25, 0), VS_OBJ(']', 41, 3), VS_OBJ('_', 57, 3)),
    VS_FRAME( VS_OBJ('@', 6, 20), VS_OBJ('=', 0, 4), VS_OBJ('|', 0, -12),
              VS_OBJ('@', 25, 0), VS_OBJ(']', 41, 3), VS_OBJ('_', 57, 3)),
    VS_FRAME( VS_OBJ('@', 6, 19), VS_OBJ('=', 0, 4), VS_OBJ('|', 0, -12),
              VS_OBJ('@', 26, 0), VS_OBJ(']', 41, 3), VS_OBJ('_', 57, 3)),
    VS_FRAME( VS_OBJ('@', 6, 18), VS_OBJ('=', 0, 4), VS_OBJ('|', 0, -12),
              VS_OBJ('@', 27, 0), VS_OBJ(']', 41, 3), VS_OBJ('_', 57, 3)),
    VS_FRAME( VS_OBJ('@', 6, 17), VS_OBJ('=', 0, 4), VS_OBJ('|', 0, -12),
              VS_OBJ('@', 28, 0), VS_OBJ(']', 41, 3), VS_OBJ('_', 57, 3)),
    VS_FRAME( VS_OBJ('@', 6, 16), VS_OBJ('=', 0, 4), VS_OBJ('|', 0, -12),
              VS_OBJ('@', 29, 0), VS_OBJ(']', 41, 3), VS_OBJ('_', 57, 3)),
    VS_FRAME( VS_OBJ('@', 6, 15), VS_OBJ('=', 0, 4), VS_OBJ('|', 0, -12),
              VS_OBJ('@', 30, 0), VS_OBJ(']', 41, 3), VS_OBJ('_', 57, 3)),
    VS_FRAME( VS_OBJ('@', 6, 14), VS_OBJ('=', 0, 4), VS_OBJ('|', 0, -12),
              VS_OBJ('@', 31, 0), VS_OBJ(']', 41, 3), VS_OBJ('_', 57, 3)),
    VS_FRAME( VS_OBJ('@', 6, 13), VS_OBJ('=', 0, 4), VS_OBJ('|', 0, -12),
              VS_OBJ('@', 33, 0), VS_OBJ(']', 41, 3), VS_OBJ('_', 57, 3)),
    VS_FRAME( VS_OBJ('@', 6, 12), VS_OBJ('=', 0, 4), VS_OBJ('|', 0, -12),
              VS_OBJ('@', 34, 0), VS_OBJ(']', 41, 3), VS_OBJ('_', 57, 3)),
    VS_FRAME( VS_OBJ('@', 6, 11), VS_OBJ('=', 0, 4), VS_OBJ('|', 0, -12),
              VS_OBJ('@', 35, 0), VS_OBJ(']', 41, 3), VS_OBJ('_', 57, 3)),
    VS_FRAME( VS_OBJ('@', 6, 10), VS_OBJ('=', 0, 4), VS_OBJ('|', 0, -12),
              VS_OBJ('@', 36, 0), VS_OBJ(']', 41, 3), VS_OBJ('_', 57, 3)),
    VS_FRAME( VS_OBJ('@', 6, 9), VS_OBJ('=', 0, 4), VS_OBJ('|', 0, -12),
              VS_OBJ('@', 37, 0), VS_OBJ(']', 41, 3), VS_OBJ('_', 57, 3)),
    VS_FRAME( VS_OBJ('@', 6, 8), VS_OBJ('=', 0, 4), VS_OBJ('|', 0, -12),
              VS_OBJ('@', 38, 0), VS_OBJ(']', 41, 3), VS_OBJ('_', 57, 3)),
    VS_FRAME( VS_OBJ('@', 6, 7), VS_OBJ('=', 0, 4), VS_OBJ('|', 0, -12),
              VS_OBJ('@', 39, 0), VS_OBJ(']', 41, 3), VS_OBJ('_', 57, 3)),
    VS_FRAME( VS_OBJ('@', 6, 6), VS_OBJ('=', 0, 4), VS_OBJ('|', 0, -12),
              VS_OBJ('@', 40, 0), VS_OBJ(']', 41, 3), VS_OBJ('_', 57, 3)),
    VS_FRAME( VS_OBJ('@', 6, 5), VS_OBJ('=', 0, 4), VS_OBJ('|', 0, -12),
              VS_OBJ('@', 41, 0), VS_OBJ(']', 41, 3), VS_OBJ('_', 57, 3))
  );
}

void vs_play(int snd) {
  vs_sounds[snd].cur = vs_sounds[snd].fs; // set current frame to first frame
}

void vs_mainPlay(int snd) {
  vs_mainStop();
  vs_mainVisual = snd;
  vs_play(snd);
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
    float shift = (float) conf.lineSize * k_drawD2 / k_perspWindowW + 5; // rescale and add buffer
    glOrtho(-k_drawD2 - shift, k_drawD2 - shift, -k_drawD2, k_drawD2, -1, 1);
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
