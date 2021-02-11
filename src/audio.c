#include "audio.h"
Mix_Chunk *sounds[k_nSounds];


// sound that we are waiting to complete
// -1 indicates no sound
int au_waiting;

int au_mainAudio;

#define k_soundsDir k_resourceDir"sounds/"

// This must match the `#define`d SND_ values in audio.h
const char *soundFileNames[] = {
  k_soundsDir"blockbreak.wav",
  k_soundsDir"blockhit.wav",
  k_soundsDir"boom.wav",
  k_soundsDir"bowserfall.wav",
  k_soundsDir"bridgebreak.wav",
  k_soundsDir"bulletbill.wav",
  k_soundsDir"castle-fast.wav",
  k_soundsDir"castle.wav",
  k_soundsDir"castleend.wav",
  k_soundsDir"coin.wav",
  k_soundsDir"death.wav",
  k_soundsDir"fire.wav",
  k_soundsDir"fireball.wav",
  k_soundsDir"gameover.wav",
  k_soundsDir"intermission.wav",
  k_soundsDir"jump.wav",
  k_soundsDir"jumpbig.wav",
  k_soundsDir"konami.wav",
  k_soundsDir"levelend.wav",
  k_soundsDir"lowtime.wav",
  k_soundsDir"mushroomappear.wav",
  k_soundsDir"mushroomeat.wav",
  k_soundsDir"oneup.wav",
  k_soundsDir"overworld-fast.wav",
  k_soundsDir"overworld.wav",
  k_soundsDir"pause.wav",
  k_soundsDir"pipe.wav",
  k_soundsDir"portal1open.wav",
  k_soundsDir"portal2open.wav",
  k_soundsDir"portalenter.wav",
  k_soundsDir"portalfizzle.wav",
  k_soundsDir"princessmusic.wav",
  k_soundsDir"rainboom.wav",
  k_soundsDir"scorering.wav",
  k_soundsDir"shot.wav",
  k_soundsDir"shrink.wav",
  k_soundsDir"stab.wav",
  k_soundsDir"starmusic-fast.wav",
  k_soundsDir"starmusic.wav",
  k_soundsDir"stomp.wav",
  k_soundsDir"swim.wav",
  k_soundsDir"underground-fast.wav",
  k_soundsDir"underground.wav",
  k_soundsDir"underwater-fast.wav",
  k_soundsDir"underwater.wav",
  k_soundsDir"vine.wav"
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
  err = Mix_PlayChannel(snd, sounds[snd], 0);
  if(err == -1) {
    printf("Unable to play sound file %s: %s\n", soundFileNames[snd], Mix_GetError());
    exit(EXIT_FAILURE);
  }
  Mix_Volume(snd, conf.effects);
}

// returns ms it took to play
int au_playWait(int snd){
  int startTime = SDL_GetTicks();
  if(au_waiting != -1) {
    printf("Can't wait for more than one audio at once. How did you even get here?\n");
    exit(EXIT_FAILURE);
  }
  err = Mix_PlayChannel(snd, sounds[snd], 0);
  if(err == -1) {
    printf("Unable to play sound file %s: %s\n", soundFileNames[snd], Mix_GetError());
    exit(EXIT_FAILURE);
  }
  Mix_Volume(snd, conf.effects);
  au_waiting = snd;
  while (au_waiting != -1 && !quit) {
    SDL_Delay(200);
  }
  return SDL_GetTicks() - startTime;
}

void au_lowTime(){
  au_mainStop();
  au_play(SND_lowtime);
}

void au_mainPlay(int snd){
  au_mainStop();
  if (snd == SND_continueMain) {
    snd = au_mainAudio;
  }
  else {
    au_mainAudio = snd;
  }
  err = Mix_PlayChannel(snd, sounds[snd], -1);
  Mix_Volume(snd, conf.music);
  if(err == -1) {
    printf("Unable to play main sound file %s: %s\n", soundFileNames[snd], Mix_GetError());
    exit(EXIT_FAILURE);
  }
}

void au_mainStop(){
  Mix_HaltChannel(au_mainAudio);
}
