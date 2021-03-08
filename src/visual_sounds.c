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
    VS_FRAME( VS_OBJ('@', 0, 0) ),
    VS_FRAME( VS_OBJ('@', 0, 1) ),
    VS_FRAME( VS_OBJ('@', 0, 2) ),
    VS_FRAME( VS_OBJ('@', 0, 3) ),
    VS_FRAME( VS_OBJ('@', 0, 4) ),
    VS_FRAME( VS_OBJ('@', 0, 5) ),
    VS_FRAME( VS_OBJ('@', 0, 6) ),
    VS_FRAME( VS_OBJ('@', 0, 7) ),
    VS_FRAME( VS_OBJ('@', 0, 8) ),
    VS_FRAME( VS_OBJ('@', 0, 9) ),
    VS_FRAME( VS_OBJ('@', 0, 10) ),
    VS_FRAME( VS_OBJ('@', 0, 11) ),
    VS_FRAME( VS_OBJ('@', 0, 12) ),
    VS_FRAME( VS_OBJ('@', 0, 13) ),
    VS_FRAME( VS_OBJ('@', 0, 14) ),
    VS_FRAME( VS_OBJ('@', 0, 15) ),
    VS_FRAME( VS_OBJ('@', 0, 16) ),
    VS_FRAME( VS_OBJ('@', 0, 17) ),
    VS_FRAME( VS_OBJ('@', 0, 18) ),
    VS_FRAME( VS_OBJ('@', 0, 19) )
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
