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
    vs_sounds[i].animFrame = 0;
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

  vs_sounds[SND_blockhit] = VS_ANIM(
    VS_FRAME( VS_OBJ('#', 5, 36) ),
    VS_FRAME( VS_OBJ('#', 5, 36) ),
    VS_FRAME( VS_OBJ('#', 5, 36) ),
    VS_FRAME( VS_OBJ('#', 5, 36) ),
    VS_FRAME( VS_OBJ('#', 5, 36) ),
    VS_FRAME( VS_OBJ('#', 5, 36) ),
    VS_FRAME( VS_OBJ('#', 5, 36) ),
    VS_FRAME( VS_OBJ('#', 5, 36) ),
    VS_FRAME( VS_OBJ('#', 5, 36) ),
    VS_FRAME( VS_OBJ('#', 5, 36) ),
    VS_FRAME( VS_OBJ('#', 5, 36) ),
    VS_FRAME( VS_OBJ('#', 5, 36) ),
    VS_FRAME( VS_OBJ('#', 5, 36) ),
    VS_FRAME( VS_OBJ('#', 5, 36) ),
    VS_FRAME( VS_OBJ('#', 5, 36) ),
    VS_FRAME( VS_OBJ('#', 5, 36) ),
    VS_FRAME( VS_OBJ('#', 5, 36) ),
    VS_FRAME( VS_OBJ('#', 5, 36) ),
    VS_FRAME( VS_OBJ('#', 5, 36) ),
    VS_FRAME( VS_OBJ('#', 5, 36) )
  );

  int base_y = 36;
  int base_x = 5;
  vs_sounds[SND_blockbreak] = VS_ANIM(
    VS_FRAME( VS_OBJ('#', base_x, base_y) ),
    VS_FRAME( VS_OBJ('#', base_x, base_y) ),
    VS_FRAME( VS_OBJ('#', base_x, base_y) ),
    VS_FRAME( VS_OBJ('#', base_x, base_y) ),
    VS_FRAME( VS_OBJ('#', base_x, base_y) ),
    VS_FRAME( VS_OBJ('#', base_x, base_y) ),
    VS_FRAME( VS_OBJ('#', base_x, base_y) ),
    VS_FRAME( VS_OBJ('#', base_x, base_y) ),
    VS_FRAME( VS_OBJ('#', base_x, base_y) ),
    VS_FRAME( VS_OBJ('#', base_x, base_y) ),
    VS_FRAME(
      VS_OBJ('b', base_x, base_y+2),
      VS_OBJ('b', base_x, base_y-2),
      VS_OBJ('b', base_x, base_y+2),
      VS_OBJ('b', base_x, base_y-2)
    ),
    VS_FRAME(
      VS_OBJ('b', base_x, base_y+2),
      VS_OBJ('b', base_x, base_y-2),
      VS_OBJ('b', base_x, base_y+2),
      VS_OBJ('b', base_x, base_y-2)
    ),
    VS_FRAME(
      VS_OBJ('b', base_x-1, base_y+1),
      VS_OBJ('b', base_x-1, base_y-3),
      VS_OBJ('b', base_x+1, base_y+1),
      VS_OBJ('b', base_x+1, base_y-3)
    ),
    VS_FRAME(
      VS_OBJ('b', base_x-1, base_y+1),
      VS_OBJ('b', base_x-1, base_y-3),
      VS_OBJ('b', base_x+1, base_y+1),
      VS_OBJ('b', base_x+1, base_y-3)
    ),
    VS_FRAME(
      VS_OBJ('b', base_x-2, base_y+0),
      VS_OBJ('b', base_x-2, base_y-4),
      VS_OBJ('b', base_x+2, base_y+0),
      VS_OBJ('b', base_x+2, base_y-4)
    ),
    VS_FRAME(
      VS_OBJ('b', base_x-2, base_y+0),
      VS_OBJ('b', base_x-2, base_y-4),
      VS_OBJ('b', base_x+2, base_y+0),
      VS_OBJ('b', base_x+2, base_y-4)
    ),
    VS_FRAME(
      VS_OBJ('b', base_x-3, base_y-1),
      VS_OBJ('b', base_x-3, base_y-5),
      VS_OBJ('b', base_x+3, base_y-1),
      VS_OBJ('b', base_x+3, base_y-5)
    ),
    VS_FRAME(
      VS_OBJ('b', base_x-3, base_y-1),
      VS_OBJ('b', base_x-3, base_y-5),
      VS_OBJ('b', base_x+3, base_y-1),
      VS_OBJ('b', base_x+3, base_y-5)
    ),
    VS_FRAME(
      VS_OBJ('b', base_x-4, base_y-2),
      VS_OBJ('b', base_x-4, base_y-6),
      VS_OBJ('b', base_x+4, base_y-2),
      VS_OBJ('b', base_x+4, base_y-6)
    ),
    VS_FRAME(
      VS_OBJ('b', base_x-5, base_y-3),
      VS_OBJ('b', base_x-5, base_y-7),
      VS_OBJ('b', base_x+5, base_y-3),
      VS_OBJ('b', base_x+5, base_y-7)
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

  vs_sounds[SND_death] = VS_ANIM(
    VS_FRAME( VS_OBJ('%', 0, 5) ),
    VS_FRAME( VS_OBJ('%', 0, 5) ),
    VS_FRAME( VS_OBJ('%', 0, 5) ),
    VS_FRAME( VS_OBJ('%', 0, 5) ),
    VS_FRAME( VS_OBJ('%', 0, 5) ),
    VS_FRAME( VS_OBJ('%', 0, 5) ),
    VS_FRAME( VS_OBJ('%', 0, 5) ),
    VS_FRAME( VS_OBJ('%', 0, 5) ),
    VS_FRAME( VS_OBJ('%', 0, 5) ),
    VS_FRAME( VS_OBJ('%', 0, 5) ),
    VS_FRAME( VS_OBJ('%', 0, 5) ),
    VS_FRAME( VS_OBJ('%', 0, 5) ),
    VS_FRAME( VS_OBJ('%', 0, 5) ),
    VS_FRAME( VS_OBJ('%', 0, 5) ),
    VS_FRAME( VS_OBJ('%', 0, 5) ),
    VS_FRAME( VS_OBJ('%', 0, 5) ),
    VS_FRAME( VS_OBJ('%', 0, 5) ),
    VS_FRAME( VS_OBJ('%', 0, 6) ),
    VS_FRAME( VS_OBJ('%', 0, 6) ),
    VS_FRAME( VS_OBJ('%', 0, 7) ),
    VS_FRAME( VS_OBJ('%', 0, 8) ),
    VS_FRAME( VS_OBJ('%', 0, 9) ),
    VS_FRAME( VS_OBJ('%', 0, 10) ),
    VS_FRAME( VS_OBJ('%', 0, 11) ),
    VS_FRAME( VS_OBJ('%', 0, 12) ),
    VS_FRAME( VS_OBJ('%', 0, 13) ),
    VS_FRAME( VS_OBJ('%', 0, 14) ),
    VS_FRAME( VS_OBJ('%', 0, 15) ),
    VS_FRAME( VS_OBJ('%', 0, 15) ),
    VS_FRAME( VS_OBJ('%', 0, 16) ),
    VS_FRAME( VS_OBJ('%', 0, 16) ),
    VS_FRAME( VS_OBJ('%', 0, 17) ),
    VS_FRAME( VS_OBJ('%', 0, 17) ),
    VS_FRAME( VS_OBJ('%', 0, 18) ),
    VS_FRAME( VS_OBJ('%', 0, 18) ),
    VS_FRAME( VS_OBJ('%', 0, 19) ),
    VS_FRAME( VS_OBJ('%', 0, 19) ),
    VS_FRAME( VS_OBJ('%', 0, 20) ),
    VS_FRAME( VS_OBJ('%', 0, 20) ),
    VS_FRAME( VS_OBJ('%', 0, 20) ),
    VS_FRAME( VS_OBJ('%', 0, 19) ),
    VS_FRAME( VS_OBJ('%', 0, 19) ),
    VS_FRAME( VS_OBJ('%', 0, 18) ),
    VS_FRAME( VS_OBJ('%', 0, 18) ),
    VS_FRAME( VS_OBJ('%', 0, 17) ),
    VS_FRAME( VS_OBJ('%', 0, 17) ),
    VS_FRAME( VS_OBJ('%', 0, 16) ),
    VS_FRAME( VS_OBJ('%', 0, 16) ),
    VS_FRAME( VS_OBJ('%', 0, 15) ),
    VS_FRAME( VS_OBJ('%', 0, 15) ),
    VS_FRAME( VS_OBJ('%', 0, 14) ),
    VS_FRAME( VS_OBJ('%', 0, 13) ),
    VS_FRAME( VS_OBJ('%', 0, 12) ),
    VS_FRAME( VS_OBJ('%', 0, 11) ),
    VS_FRAME( VS_OBJ('%', 0, 10) ),
    VS_FRAME( VS_OBJ('%', 0, 9) ),
    VS_FRAME( VS_OBJ('%', 0, 8) ),
    VS_FRAME( VS_OBJ('%', 0, 7) ),
    VS_FRAME( VS_OBJ('%', 0, 6) ),
    VS_FRAME( VS_OBJ('%', 0, 5) ),
    VS_FRAME( VS_OBJ('%', 0, 4) ),
    VS_FRAME( VS_OBJ('%', 0, 3) ),
    VS_FRAME( VS_OBJ('%', 0, 2) ),
    VS_FRAME( VS_OBJ('%', 0, 1) ),
    VS_FRAME( VS_OBJ('%', 0, 0) ),
    VS_FRAME( VS_OBJ('%', 0, -1) ),
    VS_FRAME( VS_OBJ('%', 0, -2) ),
    VS_FRAME( VS_OBJ('%', 0, -3) ),
    VS_FRAME( VS_OBJ('%', 0, -4) ),
    VS_FRAME( VS_OBJ('%', 0, -5) ),
    VS_FRAME( VS_OBJ('%', 0, -6) ),
    VS_FRAME( VS_OBJ('%', 0, -7) ),
    VS_FRAME( VS_OBJ('%', 0, -8) ),
    VS_FRAME( VS_OBJ('%', 0, -9) )
  );

  vs_sounds[SND_fireball] = VS_ANIM(
    VS_FRAME( VS_OBJ('o', 7, 0) ),
    VS_FRAME( VS_OBJ('o', 7, 0) ),
    VS_FRAME( VS_OBJ('o', 7, 0) ),
    VS_FRAME( VS_OBJ('o', 7, 0) ),
    VS_FRAME( VS_OBJ('o', 7, 0) ),
    VS_FRAME( VS_OBJ('o', 7, 0) ),
    VS_FRAME( VS_OBJ('o', 7, 0) ),
    VS_FRAME( VS_OBJ('o', 7, 0) ),
    VS_FRAME( VS_OBJ('o', 7, 0) ),
    VS_FRAME( VS_OBJ('o', 7, 0) ),
    VS_FRAME( VS_OBJ('o', 8, 0) ),
    VS_FRAME( VS_OBJ('o', 8, 0) ),
    VS_FRAME( VS_OBJ('o', 9, 0) ),
    VS_FRAME( VS_OBJ('o', 9, 0) ),
    VS_FRAME( VS_OBJ('o', 10, 0) )
  );

  // SND_gameover
  // SND_levelend
  // SND_lowtime
  // SND_mushroomappear
  // SND_mushroomeat
  // SND_oneup
  // SND_scorering
  // SND_shrink
}

void vs_play(int snd) {
  vs_sounds[snd].cur = vs_sounds[snd].fs; // set current frame to first frame
  vs_sounds[snd].animFrame = 0;
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
        gr_image(&o->frames[vs_sounds[i].animFrame / k_animFreq % o->nFrames].im, RECT_LTWH(o->x + o->bb.x, o->y + o->bb.y, o->bb.w, -(o->bb.h)));
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
    vs_sounds[i].animFrame++;
    if (*(vs_sounds[i].cur) == NULL) { // we've reached the end of the animation
      if (i == vs_mainVisual) { // this is the main audio loop
        vs_sounds[i].cur = vs_sounds[i].fs; // restart the animation
      }
      else { // we're done
        vs_sounds[i].cur = NULL;
        vs_sounds[i].animFrame = 0;
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
