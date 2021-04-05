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

  // TODO; This is a terrible way to program animations.
  //       1) it should probably be read in from a file
  //       2) it should probably have keyframes and shit not just every single frame
  //       3) it certainly shouldn't store a separate copy of every object for each frame
  //       one idea might be to make the animation just be a 2D view of certain actual game objects for a moment
  //       this has it's own problems, but it's an idea.
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

  vs_sounds[SND_goombastomp] = VS_ANIM(
    VS_FRAME( VS_OBJ('e', 5, 0) ),
    VS_FRAME( VS_OBJ('e', 5, 0) ),
    VS_FRAME( VS_OBJ('e', 5, 0) ),
    VS_FRAME( VS_OBJ('e', 5, 0) ),
    VS_FRAME( VS_OBJ('e', 5, 0) ),
    VS_FRAME( VS_OBJ('e', 5, 0) ),
    VS_FRAME( VS_OBJ('e', 5, 0) ),
    VS_FRAME( VS_OBJ('e', 5, 0) ),
    VS_FRAME( VS_OBJ('e', 5, 0) ),
    VS_FRAME( VS_OBJ('e', 5, 0) ),
    VS_FRAME( VS_OBJ('E', 5, -8) ),
    VS_FRAME( VS_OBJ('E', 5, -8) ),
    VS_FRAME( VS_OBJ('E', 5, -8) ),
    VS_FRAME( VS_OBJ('E', 5, -8) ),
    VS_FRAME( VS_OBJ('E', 5, -8) ),
    VS_FRAME( VS_OBJ('E', 5, -8) ),
    VS_FRAME( VS_OBJ('E', 5, -8) ),
    VS_FRAME( VS_OBJ('E', 5, -8) ),
    VS_FRAME( VS_OBJ('E', 5, -8) ),
    VS_FRAME( VS_OBJ('E', 5, -8) )
  );

  vs_sounds[SND_koopastomp] = VS_ANIM(
    VS_FRAME( VS_OBJ('&', 5, 7) ),
    VS_FRAME( VS_OBJ('&', 5, 7) ),
    VS_FRAME( VS_OBJ('&', 5, 7) ),
    VS_FRAME( VS_OBJ('&', 5, 7) ),
    VS_FRAME( VS_OBJ('&', 5, 7) ),
    VS_FRAME( VS_OBJ('&', 5, 7) ),
    VS_FRAME( VS_OBJ('&', 5, 7) ),
    VS_FRAME( VS_OBJ('&', 5, 7) ),
    VS_FRAME( VS_OBJ('&', 5, 7) ),
    VS_FRAME( VS_OBJ('&', 5, 7) ),
    VS_FRAME( VS_OBJ('7', 5, -3) ),
    VS_FRAME( VS_OBJ('7', 5, -3) ),
    VS_FRAME( VS_OBJ('7', 5, -3) ),
    VS_FRAME( VS_OBJ('7', 5, -3) ),
    VS_FRAME( VS_OBJ('7', 5, -3) ),
    VS_FRAME( VS_OBJ('7', 5, -3) ),
    VS_FRAME( VS_OBJ('7', 5, -3) ),
    VS_FRAME( VS_OBJ('7', 5, -3) ),
    VS_FRAME( VS_OBJ('7', 5, -3) ),
    VS_FRAME( VS_OBJ('7', 5, -3) )
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

  vs_sounds[SND_mushroomappear] = VS_ANIM(
    VS_FRAME( VS_OBJ('r', 5, 20+16), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16+1), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16+1), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16+2), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16+2), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16+3), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16+3), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16+4), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16+4), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16+5), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16+5), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16+6), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16+6), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16+7), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16+7), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16+8), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16+9), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16+10), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16+10), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16+11), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16+11), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16+12), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16+12), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16+13), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16+13), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16+14), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16+14), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16+15), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16+15), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16+16), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('r', 5, 20+16+16), VS_OBJ('?', 5, 20+16) )
  );

  vs_sounds[SND_fireflowerappear] = VS_ANIM(
    VS_FRAME( VS_OBJ('R', 5, 20+16), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16+1), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16+1), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16+2), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16+2), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16+3), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16+3), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16+4), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16+4), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16+5), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16+5), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16+6), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16+6), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16+7), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16+7), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16+8), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16+9), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16+10), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16+10), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16+11), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16+11), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16+12), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16+12), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16+13), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16+13), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16+14), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16+14), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16+15), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16+15), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16+16), VS_OBJ('?', 5, 20+16) ),
    VS_FRAME( VS_OBJ('R', 5, 20+16+16), VS_OBJ('?', 5, 20+16) )
  );

#define _TMP_FLAG(x, y)                         \
  VS_OBJ('<', x, y),                            \
    VS_OBJ('!', x, y-16*1),                     \
    VS_OBJ('!', x, y-16*2),                     \
    VS_OBJ('!', x, y-16*3),                     \
    VS_OBJ('!', x, y-16*4),                     \
    VS_OBJ('!', x, y-16*5),                     \
    VS_OBJ('!', x, y-16*6),                     \
    VS_OBJ('!', x, y-16*7),                     \
    VS_OBJ('X', x, y-16*8)

  vs_sounds[SND_levelend] = VS_ANIM(
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3-1) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3-1) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3-2) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3-2) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3-3) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3-3) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3-4) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3-4) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3-5) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3-5) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3-6) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3-6) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3-7) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3-7) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3-8) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3-8) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3-9) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3-9) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3-10) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3-10) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3-11) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3-11) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3-12) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3-12) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3-13) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3-13) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3-14) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3-14) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3-15) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*3-15) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2-1) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2-1) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2-2) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2-2) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2-3) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2-3) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2-4) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2-4) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2-5) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2-5) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2-6) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2-6) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2-7) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2-7) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2-8) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2-8) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2-9) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2-9) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2-10) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2-10) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2-11) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2-11) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2-12) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2-12) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2-13) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2-13) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2-14) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2-14) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2-15) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*2-15) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1-1) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1-1) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1-2) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1-2) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1-3) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1-3) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1-4) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1-4) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1-5) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1-5) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1-6) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1-6) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1-7) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1-7) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1-8) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1-8) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1-9) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1-9) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1-10) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1-10) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1-11) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1-11) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1-12) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1-12) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1-13) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1-13) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1-14) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1-14) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1-15) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*1-15) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0-1) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0-1) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0-2) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0-2) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0-3) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0-3) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0-4) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0-4) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0-5) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0-5) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0-6) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0-6) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0-7) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0-7) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0-8) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0-8) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0-9) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0-9) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0-10) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0-10) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0-11) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0-11) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0-12) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0-12) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0-13) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0-13) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0-14) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0-14) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0-15) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*0-15) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)-1) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)-1) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)-2) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)-2) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)-3) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)-3) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)-4) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)-4) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)-5) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)-5) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)-6) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)-6) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)-7) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)-7) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)-8) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)-8) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)-9) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)-9) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)-10) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)-10) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)-11) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)-11) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)-12) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)-12) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)-13) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)-13) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)-14) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)-14) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)-15) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-1)-15) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)-1) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)-1) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)-2) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)-2) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)-3) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)-3) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)-4) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)-4) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)-5) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)-5) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)-6) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)-6) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)-7) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)-7) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)-8) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)-8) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)-9) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)-9) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)-10) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)-10) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)-11) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)-11) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)-12) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)-12) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)-13) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)-13) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)-14) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)-14) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)-15) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-2)-15) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-3)) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-3)) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-3)) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-3)) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-3)) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-3)) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-3)) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-3)) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-3)) ),
    VS_FRAME(_TMP_FLAG(5+10, 16*4), VS_OBJ('@', 5, 16*(-3)) )
  );

#undef _TMP_FLAG

  vs_sounds[SND_scorering] = VS_ANIM(
    VS_FRAME( VS_OBJ('c', 5, 20+0) ),
    VS_FRAME( VS_OBJ('c', 5, 20+1) ),
    VS_FRAME( VS_OBJ('c', 5, 20+2) ),
    VS_FRAME( VS_OBJ('c', 5, 20+3) ),
    VS_FRAME( VS_OBJ('c', 5, 20+4) ),
    VS_FRAME( VS_OBJ('c', 5, 20+5) ),
    VS_FRAME( VS_OBJ('c', 5, 20+6) ),
    VS_FRAME( VS_OBJ('c', 5, 20+7) ),
    VS_FRAME( VS_OBJ('c', 5, 20+8) ),
    VS_FRAME( VS_OBJ('c', 5, 20+9) ),
    VS_FRAME( VS_OBJ('c', 5, 20+10) ),
    VS_FRAME( VS_OBJ('c', 5, 20+11) ),
    VS_FRAME( VS_OBJ('c', 5, 20+12) ),
    VS_FRAME( VS_OBJ('c', 5, 20+13) ),
    VS_FRAME( VS_OBJ('c', 5, 20+14) ),
    VS_FRAME( VS_OBJ('c', 5, 20+15) ),
    VS_FRAME( VS_OBJ('c', 5, 20+16) )
  );

  vs_sounds[SND_oneup] = VS_ANIM(
    VS_FRAME( VS_OBJ('g', 5, 0) ),
    VS_FRAME( VS_OBJ('g', 5, 0) ),
    VS_FRAME( VS_OBJ('g', 5, 0) ),
    VS_FRAME( VS_OBJ('g', 5, 0) ),
    VS_FRAME( VS_OBJ('g', 5, 0) ),
    VS_FRAME( VS_OBJ('g', 5, 0) ),
    VS_FRAME( VS_OBJ('g', 5, 0) ),
    VS_FRAME( VS_OBJ('g', 5, 0) ),
    VS_FRAME( VS_OBJ('g', 5, 0) ),
    VS_FRAME( VS_OBJ('g', 5, 0) )
  );

  vs_sounds[SND_gameover] = VS_ANIM(
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) ),
    VS_FRAME( VS_OBJ('%', 5, 0), VS_OBJ('%', 5+32, 0), VS_OBJ('%', 5+64, 0) )
  );
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
