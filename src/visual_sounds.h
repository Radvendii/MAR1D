#ifndef _VISUAL_SOUNDS_H
#define _VISUAL_SOUNDS_H

#include "helpers.h"

void vs_init();
void vs_update();
void vs_draw();
void vs_deinit();

void vs_play(int snd);
void vs_mainPlay(int snd);
void vs_mainStop();

#endif
