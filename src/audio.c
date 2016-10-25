#include "audio.h"
pid_t musicF;
pid_t frk;
struct sigaction sa;
char fileNames[k_nSounds][40] = {
  "blockbreak.wav",
  "blockhit.wav",
  "boom.wav",
  "bowserfall.wav",
  "bridgebreak.wav",
  "bulletbill.wav",
  "castle-fast.wav",
  "castle.wav",
  "castleend.wav",
  "coin.wav",
  "death.wav",
  "fire.wav",
  "fireball.wav",
  "gameover.wav",
  "intermission.wav",
  "jump.wav",
  "jumpbig.wav",
  "konami.wav",
  "levelend.wav",
  "lowtime.wav",
  "mushroomappear.wav",
  "mushroomeat.wav",
  "oneup.wav",
  "overworld-fast.wav",
  "overworld.wav",
  "pause.wav",
  "pipe.wav",
  "portal1open.wav",
  "portal2open.wav",
  "portalenter.wav",
  "portalfizzle.wav",
  "princessmusic.wav",
  "rainboom.wav",
  "scorering.wav",
  "shot.wav",
  "shrink.wav",
  "stab.wav",
  "starmusic-fast.wav",
  "starmusic.wav",
  "stomp.wav",
  "swim.wav",
  "underground-fast.wav",
  "underground.wav",
  "underwater-fast.wav",
  "underwater.wav",
  "vine.wav"
};

void au_init(){
  sa.sa_handler = killfrk;
  sigemptyset (&sa.sa_mask);
  sa.sa_flags = 0;
}

void au_deinit(){
  au_mainStop();
}

void au_lowTime(){
  au_mainStop();
  au_play(SND_lowtime);
  if(fork() == 0){
    sleep(3);
    au_mainPlay(--au_mainAudio);
    exit(0);
  }
}

void killfrk(int signo){
  kill(frk, SIGTERM);
  exit(0);
}

pid_t au_playplay(int snd, bool loop){
  char cmd[100];
  sprintf(cmd, "./resources/sounds/%s", fileNames[snd]);
  pid_t frk = fork();
  if(frk == 0){
    execl("./resources/play", "./resources/play", k_player, cmd, (loop ? "1" : ""), NULL);
      //first argument twice to pass the name as first parameter
    exit(1);
  }
  else{
    return frk;
  }
}

pid_t au_play(int snd){
  return au_playplay(snd, false);
}

void au_playWait(int snd){
  pid_t frk = au_play(snd);
  int sig;
  waitpid(frk, &sig, 0);
}

void au_mainPlay(int snd){
  static bool first = true;
  if(first){
    first = false;
  }
  else{
    au_mainStop();
  }
  au_mainAudio = snd;
  musicF = au_playplay(snd, true);
}

void au_mainStop(){
  char cmd[100];
  sprintf(cmd, "kill -9 $(pgrep -P %d) %d", musicF, musicF);
  system(cmd);
}
