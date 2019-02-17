#ifndef _AUDIO_H
#define _AUDIO_H
#include "helpers.h"
#include "windowing.h"
#include <signal.h>
#include <unistd.h>
#include <AL/al.h>
#include <AL/alc.h>
#include "resources.h"
/* #include <pthread.h> */
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
#define SND_portal1open 27
#define SND_portal2open 28
#define SND_portalenter 29
#define SND_portalfizzle 30
#define SND_princessmusic 31
#define SND_rainboom 32
#define SND_scorering 33
#define SND_shot 34
#define SND_shrink 35
#define SND_stab 36
#define SND_starmusic_fast 37
#define SND_starmusic 38
#define SND_stomp 39
#define SND_swim 40
#define SND_underground_fast 41
#define SND_underground 42
#define SND_underwater_fast 43
#define SND_underwater 44
#define SND_vine 45
#define k_nSounds 46
#define k_killMain k_nSounds

int au_mainAudio;
bool mute, effects;

void au_init(bool, bool);
void au_deinit();
void au_play(int);
void au_playWait(int);
void au_playplay(int);
void au_mainPlay(int);
void au_lowTime();
void au_mainStop();
void au_loadSounds();
void au_initEach();
void au_deinitEach();

#endif
