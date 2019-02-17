#include "audio.h"
int default_driver;
ALCdevice *device;
ALCcontext *context;
int sz[k_nSounds];
ALuint buffers[k_nSounds];
ALuint mainSource;
ALuint sources[k_nSounds];
char fileNames[k_nSounds][40] = {
  "sounds/blockbreak.raw",
  "sounds/blockhit.raw",
  "sounds/boom.raw",
  "sounds/bowserfall.raw",
  "sounds/bridgebreak.raw",
  "sounds/bulletbill.raw",
  "sounds/castle-fast.raw",
  "sounds/castle.raw",
  "sounds/castleend.raw",
  "sounds/coin.raw",
  "sounds/death.raw",
  "sounds/fire.raw",
  "sounds/fireball.raw",
  "sounds/gameover.raw",
  "sounds/intermission.raw",
  "sounds/jump.raw",
  "sounds/jumpbig.raw",
  "sounds/konami.raw",
  "sounds/levelend.raw",
  "sounds/lowtime.raw",
  "sounds/mushroomappear.raw",
  "sounds/mushroomeat.raw",
  "sounds/oneup.raw",
  "sounds/overworld-fast.raw",
  "sounds/overworld.raw",
  "sounds/pause.raw",
  "sounds/pipe.raw",
  "sounds/portal1open.raw",
  "sounds/portal2open.raw",
  "sounds/portalenter.raw",
  "sounds/portalfizzle.raw",
  "sounds/princessmusic.raw",
  "sounds/rainboom.raw",
  "sounds/scorering.raw",
  "sounds/shot.raw",
  "sounds/shrink.raw",
  "sounds/stab.raw",
  "sounds/starmusic-fast.raw",
  "sounds/starmusic.raw",
  "sounds/stomp.raw",
  "sounds/swim.raw",
  "sounds/underground-fast.raw",
  "sounds/underground.raw",
  "sounds/underwater-fast.raw",
  "sounds/underwater.raw",
  "sounds/vine.raw"
};

void au_init(bool _mute, bool _effects){
  mute = _mute;
  effects = _effects;
  au_loadSounds();

  alGetError();
  device = alcOpenDevice(NULL);
  if (!device) {
    printf("Failed to load audio device.");
    exit(1);
  }
  context = alcCreateContext(device, NULL);
  alcMakeContextCurrent(context);
  alGenSources((ALuint)1, &mainSource);
  /* alSourcef(mainSource, AL_PITCH, 1); */
  /* alSourcef(mainSource, AL_GAIN, 1); */
  /* alSource3f(mainSource, AL_POSITION, 0, 0, 0); */
  /* alSource3f(mainSource, AL_VELOCITY, 0, 0, 0); */
  alSourcei(mainSource, AL_LOOPING, AL_TRUE);
  alGenSources((ALuint)k_nSounds, sources);
}

void au_loadSounds(){
  for(int i=0;i<k_nSounds;i++){
    FILE* f = rs_getBFile(fileNames[i]);
    ALvoid *data;
    fseek(f, 0L, SEEK_END);
    sz[i] = ftell(f);
    fseek(f, 0L, SEEK_SET);
    data = salloc(sz[i] * sizeof(char));
    fread(data, sz[i], 1, f);
    sfclose(f);
    alBufferData(buffers[i], AL_FORMAT_MONO16, data, (ALsizei)*sz, 44100);
  }
}

void au_deinit(){
  au_mainStop();
  alDeleteSources(1, &mainSource);
  alDeleteSources(k_nSounds, sources);
  alDeleteBuffers(k_nSounds, buffers);
  alcMakeContextCurrent(NULL);
  alcDestroyContext(context);
  alcCloseDevice(device);
}

ALuint firstAvailableSource() {
  ALint source_state;
  while(true){
    for(int i=0;i < k_nSounds; i++) {
      alGetSourcei(sources[i], AL_SOURCE_STATE, &source_state);
      if(source_state != AL_PLAYING){
        return i;
      }
    }
  }
}

void au_play(int snd){
  if(!mute || effects){
    ALuint source = firstAvailableSource();
    alSourcei(source, AL_BUFFER, buffers[snd]);
    alSourcePlay(source);
  }
}
void au_playWait(int snd){
  if(!mute || effects){
    ALint source_state;
    ALuint source = firstAvailableSource();
    alSourcei(source, AL_BUFFER, buffers[snd]);
    alSourcePlay(source);
    do {
      alGetSourcei(source, AL_SOURCE_STATE, &source_state);
    } while (source_state == AL_PLAYING);
  }
}

void au_lowTime(){
  au_mainStop();
  au_play(SND_lowtime);
  if(fork() == 0){
    sleep(3);
    au_mainPlay(au_mainAudio-1);
    exit(0);
  }
}

void au_mainPlay(int snd){
  if(!mute){
    au_mainStop();
    alSourcei(mainSource, AL_BUFFER, buffers[snd]);
    alSourcePlay(mainSource);
  }
}

void au_mainStop(){
  alSourceStop(mainSource);
}
