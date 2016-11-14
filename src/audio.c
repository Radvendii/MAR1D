#include "audio.h"
//static pthread_key_t key;
int default_driver;
ao_sample_format format;
ao_device* device;
int piperw[2];
//static pthread_t mainThread;
char* sounds[k_nSounds];
int sz[k_nSounds];
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
  ao_initialize();

  memset(&format, 0, sizeof(format));
  format.bits = 16;
  format.channels = 1;
  format.rate = 44100;

  format.byte_format = AO_FMT_LITTLE;
  pipe(piperw);
  if(fork() == 0){ //This is the fork that does everything. I have to keep it uncontaminated by GLFW.
    close(piperw[1]);
    playDaemon();
    close(piperw[0]);
    exit(0);
  }
  close(piperw[0]);
}

void playDaemon(){
  unsigned int _snd;
  int snd;
  bool main;
  pid_t mainFork;
  pid_t frk;
  while(read(piperw[0], &_snd, sizeof(int)) > 0){
    snd = _snd >> 1;
    main = _snd & 1;
    if(snd == k_killMain){
      if(mainFork){
        kill(mainFork, SIGTERM);
        mainFork = 0;
      }
    }
    else{
      if(main){
        mainFork = fork();
        if(mainFork == 0){
          au_playloop(snd);
          exit(0);
        }
      }
      else{
        if(fork() == 0){
          au_playplay(snd);
          exit(0);
        }
      }
    }
  }
}

void au_playplay(int snd){
  au_initEach();
  ao_play(device, sounds[snd], sz[snd]);
  au_deinitEach();
}

void au_playloop(int snd){
  au_initEach();
  while(1){
    ao_play(device, sounds[snd], sz[snd]);
  }
  au_deinitEach(); //In reality, calls this via signal() in au_initEach()
}

void au_initEach(){
  default_driver = ao_default_driver_id();
  device = ao_open_live(default_driver, &format, NULL);
  signal(SIGTERM, au_deinitEach);
}

void au_loadSounds(){
  for(int i=0;i<k_nSounds;i++){
    sounds[i] = au_loadSound(fileNames[i], &sz[i]);
  }
}

char* au_loadSound(char* fn, int* sz){
  FILE* f = rs_getBFile(fn);
  char *buffer;
  fseek(f, 0L, SEEK_END);
  *sz = ftell(f);
  fseek(f, 0L, SEEK_SET);
  buffer = salloc(*sz * sizeof(char));
  fread(buffer, *sz, 1, f);
  sfclose(f);
  return buffer;
}

void au_deinit(){
  au_mainStop();
  for(int i=0;i<k_nSounds;i++){
    free(sounds[i]);
  }
  close(piperw[1]);
  ao_shutdown();
}

void au_deinitEach(){
  ao_close(device);
  exit(0);
}

void au_play(int snd){
  if(!mute || effects){
    snd = snd << 1;
    write(piperw[1], &snd, sizeof(int));
  }
}
void au_playWait(int snd){
  if(!mute || effects){
    //need to repeat this code because au_deinitEach() calls exit();
    default_driver = ao_default_driver_id();
    device = ao_open_live(default_driver, &format, NULL);
    ao_play(device, sounds[snd], sz[snd]);
    ao_close(device);
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
    au_mainAudio = snd;
    snd = (snd << 1) | 1;
    write(piperw[1], &snd, sizeof(int));
  }
}

void au_mainStop(){
  int sig = (k_killMain << 1) | 1;
  write(piperw[1], &sig, sizeof(int));
}
