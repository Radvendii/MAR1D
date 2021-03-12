#ifndef _AUDIO_H
#define _AUDIO_H
#include "helpers.h"
#include "windowing.h"
#include "visual_sounds.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include "resources.h"
#define SND_none -1
#define SND_blockbreak 0
#define SND_blockhit 1
#define SND_boom 2
#define SND_bowserfall 3
#define SND_bridgebreak 4
#define SND_bulletbill 5
#define SND_castle_fast 6
#define SND_castle 7
#define SND_castleend 8
#define SND_coin 9
#define SND_death 10
#define SND_fire 11
#define SND_fireball 12
#define SND_gameover 13
#define SND_intermission 14
#define SND_jump 15
#define SND_jumpbig 16
#define SND_konami 17
#define SND_levelend 18
#define SND_lowtime 19
#define SND_mushroomappear 20
#define SND_mushroomeat 21
#define SND_oneup 22
#define SND_overworld_fast 23
#define SND_overworld 24
#define SND_pause 25
#define SND_pipe 26
#define SND_princessmusic 27
#define SND_rainboom 28
#define SND_scorering 29
#define SND_shot 30
#define SND_shrink 31
#define SND_stab 32
#define SND_starmusic_fast 33
#define SND_starmusic 34
#define SND_stomp 35
#define SND_swim 36
#define SND_underground_fast 37
#define SND_underground 38
#define SND_underwater_fast 39
#define SND_underwater 40
#define SND_vine 41
#define k_nSounds 42
#define SND_continueMain k_nSounds

#define k_mix_frequency MIX_DEFAULT_FREQUENCY
#define k_mix_format AUDIO_S16SYS
#define k_mix_channels 2
#define k_mix_chunksize 512

extern int au_waiting;

void au_init();
void au_update();
void au_channelFinished(int);
void au_deinit();
void au_play(int);
void au_playWait(int);
void au_mainPlay(int);
void au_lowTime();
void au_mainStop();
void au_loadSounds();
void playDaemon();

#endif
