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
    "blockbreak.raw",
    "blockhit.raw",
    "boom.raw",
    "bowserfall.raw",
    "bridgebreak.raw",
    "bulletbill.raw",
    "castle-fast.raw",
    "castle.raw",
    "castleend.raw",
    "coin.raw",
    "death.raw",
    "fire.raw",
    "fireball.raw",
    "gameover.raw",
    "intermission.raw",
    "jump.raw",
    "jumpbig.raw",
    "konami.raw",
    "levelend.raw",
    "lowtime.raw",
    "mushroomappear.raw",
    "mushroomeat.raw",
    "oneup.raw",
    "overworld-fast.raw",
    "overworld.raw",
    "pause.raw",
    "pipe.raw",
    "portal1open.raw",
    "portal2open.raw",
    "portalenter.raw",
    "portalfizzle.raw",
    "princessmusic.raw",
    "rainboom.raw",
    "scorering.raw",
    "shot.raw",
    "shrink.raw",
    "stab.raw",
    "starmusic-fast.raw",
    "starmusic.raw",
    "stomp.raw",
    "swim.raw",
    "underground-fast.raw",
    "underground.raw",
    "underwater-fast.raw",
    "underwater.raw",
    "vine.raw"
};
FILE* au_readBFile(char* fn){
    char fn_[100];
    sprintf(fn_, "./resources/sounds/%s", fn);
    return sfopen(fn_, "rb");
}

void au_init(){
    au_loadSounds();
    ao_initialize();

    memset(&format, 0, sizeof(format));
    format.bits = 16;
    format.channels = 1;
    format.rate = 44100;

    format.byte_format = AO_FMT_LITTLE;
    default_driver = ao_default_driver_id();
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
        if(snd == k_killMain && mainFork){
            kill(mainFork, SIGTERM);
            mainFork = 0;
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
    au_deinitEach(); //Calls this via signal() in au_initEach()
}

void au_initEach(){
    device = ao_open_live(default_driver, &format, NULL);
    signal(SIGTERM, au_deinitEach);
}

void au_loadSounds(){
    for(int i=0;i<k_nSounds;i++){
        sounds[i] = au_loadSound(fileNames[i], &sz[i]);
    }
}

char* au_loadSound(char* fn, int* sz){
    FILE* f = au_readBFile(fn);
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
    for(int i=0;i<k_nSounds;i++){
        free(sounds[i]);
    }
    au_mainStop();
    close(piperw[1]);
    kill(-1, SIGKILL);
    ao_shutdown();
}

void au_deinitEach(){
    ao_close(device);
}

void au_play(int snd){
    snd = snd << 1;
    write(piperw[1], &snd, sizeof(int));
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
    static bool first = true;
    if(first){
        first = false;
    }
    else{
        au_mainStop();
    }
    au_mainAudio = snd;
    snd = (snd << 1) | 1;
    write(piperw[1], &snd, sizeof(int));
}

void au_mainStop(){
    pid_t mainFrk;
    int sig = k_killMain << 1;
    write(piperw[1], &sig, sizeof(int));
}
