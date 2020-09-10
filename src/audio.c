#include "audio.h"
Mix_Chunk *sounds[k_nSounds];

#define k_soundsDir k_resourceDir"sounds/"

// This must match the `#define`d SND_ values in audio.h
const char *soundFileNames[] = {
  k_soundsDir"blockbreak.ogg",
  k_soundsDir"blockhit.ogg",
  k_soundsDir"boom.ogg",
  k_soundsDir"bowserfall.ogg",
  k_soundsDir"bridgebreak.ogg",
  k_soundsDir"bulletbill.ogg",
  k_soundsDir"castle-fast.ogg",
  k_soundsDir"castle.ogg",
  k_soundsDir"castleend.ogg",
  k_soundsDir"coin.ogg",
  k_soundsDir"death.ogg",
  k_soundsDir"fire.ogg",
  k_soundsDir"fireball.ogg",
  k_soundsDir"gameover.ogg",
  k_soundsDir"intermission.ogg",
  k_soundsDir"jump.ogg",
  k_soundsDir"jumpbig.ogg",
  k_soundsDir"konami.ogg",
  k_soundsDir"levelend.ogg",
  k_soundsDir"lowtime.ogg",
  k_soundsDir"mushroomappear.ogg",
  k_soundsDir"mushroomeat.ogg",
  k_soundsDir"oneup.ogg",
  k_soundsDir"overworld-fast.ogg",
  k_soundsDir"overworld.ogg",
  k_soundsDir"pause.ogg",
  k_soundsDir"pipe.ogg",
  k_soundsDir"portal1open.ogg",
  k_soundsDir"portal2open.ogg",
  k_soundsDir"portalenter.ogg",
  k_soundsDir"portalfizzle.ogg",
  k_soundsDir"princessmusic.ogg",
  k_soundsDir"rainboom.ogg",
  k_soundsDir"scorering.ogg",
  k_soundsDir"shot.ogg",
  k_soundsDir"shrink.ogg",
  k_soundsDir"stab.ogg",
  k_soundsDir"starmusic-fast.ogg",
  k_soundsDir"starmusic.ogg",
  k_soundsDir"stomp.ogg",
  k_soundsDir"swim.ogg",
  k_soundsDir"underground-fast.ogg",
  k_soundsDir"underground.ogg",
  k_soundsDir"underwater-fast.ogg",
  k_soundsDir"underwater.ogg",
  k_soundsDir"vine.ogg"
};

// Janky workaround because there's no way to play one sound after another with SDL
int lowtime_done = false;

void au_init(){
  au_waiting = -1;

  err = Mix_OpenAudio(k_mix_frequency, k_mix_format, k_mix_channels, k_mix_chunksize);
  if(err == -1){
    printf("Failed to open audio: %s\n", Mix_GetError());
    exit(EXIT_FAILURE);
  }
  err = Mix_AllocateChannels(k_nSounds);
  if(err != k_nSounds){
    printf("Couldn't allocate %d mixing channels. Got %d instead.", k_nSounds, err);
    exit(EXIT_FAILURE);
  }

  Mix_ChannelFinished(au_channelFinished);

  au_loadSounds();
}

void au_channelFinished(int channel) {
  if(channel == au_waiting) {
    au_waiting = -1;
  }

  switch(channel){
    case SND_lowtime:
      // Low time just finished playing.
      // We cannot directly play the main audio here (see Mix_ChannelFinished documentation)
      // So we set lowtime_done flag to signal to restart mainAudio
      lowtime_done = true;
      break;
    default:
      break;
  }
}

void au_update() {
  while(au_waiting != -1) {
    SDL_Delay(200);
  }

  if(lowtime_done) {
    au_mainPlay(--au_mainAudio);// play the low-time version of the current music (always one less)
    lowtime_done = false; // reset flag
  }
}

void au_loadSounds(){
  memset(sounds, 0, sizeof(Mix_Chunk *) * k_nSounds);
  for(int i=0;i<k_nSounds;i++){
    sounds[i] = Mix_LoadWAV(soundFileNames[i]);
    if(!sounds[i]) {
      printf("Unable to load sound file %s: %s\n", soundFileNames[i], SDL_GetError());
      exit(EXIT_FAILURE);
    }
  }
}

void au_deinit(){
  // Stop all audio
  Mix_HaltChannel(-1);
  // Stop main audio separately in case it is ever implemented differently
  au_mainStop();

  for(int i=0;i<k_nSounds;i++){
    Mix_FreeChunk(sounds[i]);
  }
  Mix_CloseAudio();

}

void au_play(int snd){
  if(!conf.mute || conf.effects){
    err = Mix_PlayChannel(snd, sounds[snd], 0);
    if(err == -1) {
      printf("Unable to play sound file %s: %s\n", soundFileNames[snd], Mix_GetError());
      exit(EXIT_FAILURE);
    }
  }
}

// TODO: this seems to not be working
void au_playWait(int snd){
  if(au_waiting != -1) {
    printf("Can't wait for more than one audio at once. How did you even get here?\n");
    exit(EXIT_FAILURE);
  }
  if(!conf.mute || conf.effects){
    err = Mix_PlayChannel(snd, sounds[snd], 0);
    if(err == -1) {
      printf("Unable to play sound file %s: %s\n", soundFileNames[snd], Mix_GetError());
      exit(EXIT_FAILURE);
    }
    au_waiting = snd;
  }
}

void au_lowTime(){
  au_mainStop();
  au_play(SND_lowtime);
}

void au_mainPlay(int snd){
  if(!conf.mute){
    au_mainStop();
    au_mainAudio = snd;
    err = Mix_PlayChannel(snd, sounds[snd], -1);
    if(err == -1) {
      printf("perspWindow has focus? %d\n", SDL_GetWindowFlags(perspWindow) & SDL_WINDOW_INPUT_FOCUS);
      printf("Unable to play main sound file %s: %s\n", soundFileNames[snd], Mix_GetError());
      exit(EXIT_FAILURE);
    }
  }
}

void au_mainStop(){
  Mix_HaltChannel(au_mainAudio);
}
