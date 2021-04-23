#include "audio.h"
Mix_Chunk *sounds[k_nSounds];


// sound that we are waiting to complete
// -1 indicates no sound
int au_waiting;

int au_mainAudio;

#define k_soundsDir "sounds/"
#define k_maxSoundFileNameLen 512

// This must match the `#define`d SND_ values in audio.h
const char *soundFileNames[] = {
  "blockbreak.ogg",
  "blockhit.ogg",
  "boom.ogg",
  "bowserfall.ogg",
  "bridgebreak.ogg",
  "bulletbill.ogg",
  "castle-fast.ogg",
  "castle.ogg",
  "castleend.ogg",
  "coin.ogg",
  "death.ogg",
  "fire.ogg",
  "fireball.ogg",
  "gameover.ogg",
  "intermission.ogg",
  "jump.ogg",
  "jumpbig.ogg",
  "konami.ogg",
  "levelend.ogg",
  "lowtime.ogg",
  "mushroomappear.ogg",
  "mushroomappear.ogg", // SND_fireflowerappear
  "mushroomeat.ogg",
  "mushroomeat.ogg", // SND_fireflowereat
  "oneup.ogg",
  "overworld-fast.ogg",
  "overworld.ogg",
  "pause.ogg",
  "pipe.ogg",
  "princessmusic.ogg",
  "rainboom.ogg",
  "scorering.ogg",
  "shot.ogg",
  "shrink.ogg",
  "stab.ogg",
  "starmusic-fast.ogg",
  "starmusic.ogg",
  "stomp.ogg",
  "stomp.ogg", // SND_goombastomp
  "stomp.ogg", // SND_koopastomp
  "swim.ogg",
  "underground-fast.ogg",
  "underground.ogg",
  "underwater-fast.ogg",
  "underwater.ogg",
  "vine.ogg"
};

// Janky workaround because there's no way to play one sound after another with SDL
int lowtime_done = false;

void au_init(){
  au_waiting = SND_none;

  err = Mix_OpenAudio(k_mix_frequency, k_mix_format, k_mix_channels, k_mix_chunksize);
  if(err == -1){
    DEBUG("Failed to open audio: %s", Mix_GetError());
    exit(EXIT_FAILURE);
  }
  err = Mix_AllocateChannels(k_nSounds);
  if(err != k_nSounds){
    DEBUG("Couldn't allocate %d mixing channels. Got %d instead.", k_nSounds, err);
    exit(EXIT_FAILURE);
  }

  Mix_ChannelFinished(au_channelFinished);

  au_loadSounds();
}

void au_channelFinished(int channel) {
  if(channel == au_waiting) {
    au_waiting = SND_none;
  }

  if(channel == SND_lowtime) {
      // Low time just finished playing.
      // We cannot directly play the main audio here (see Mix_ChannelFinished documentation)
      // So we set lowtime_done flag to signal to restart mainAudio
      lowtime_done = true;
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
  char *soundFilePath = salloc(strlen(rs_resourceDir) + strlen(k_soundsDir) + k_maxSoundFileNameLen + 2);
  sprintf(soundFilePath, "%s/%s", rs_resourceDir, k_soundsDir);
  int soundFileOffset = strlen(soundFilePath);
  for(int i=0;i<k_nSounds;i++){
    strcpy(soundFilePath + soundFileOffset, soundFileNames[i]);
    sounds[i] = Mix_LoadWAV(soundFilePath);
    if(!sounds[i]) {
      DEBUG("Unable to load sound file %s: %s", soundFileNames[i], SDL_GetError());
      exit(EXIT_FAILURE);
    }
  }
  free(soundFilePath);
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
    DEBUG("Unable to play sound file %s: %s", soundFileNames[snd], Mix_GetError());
    exit(EXIT_FAILURE);
  }
  Mix_Volume(snd, conf.effects);
  vs_play(snd);
}

// returns ms it took to play
void au_playWait(int snd){
  if(au_waiting != SND_none) {
    DEBUG("Can't wait for more than one audio at once. How did you even get here?");
    exit(EXIT_FAILURE);
  }
  au_play(snd);
  au_waiting = snd;
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
    DEBUG("Unable to play main sound file %s: %s", soundFileNames[snd], Mix_GetError());
    exit(EXIT_FAILURE);
  }
  vs_mainPlay(snd);
}

bool au_playing(int snd){
  if (snd == SND_none) {
    return !Mix_Playing(-1);
  }
  else {
    return (au_mainAudio == snd || Mix_Playing(snd));
  }
}

void au_mainStop(){
  Mix_HaltChannel(au_mainAudio);
  vs_mainStop();
}
