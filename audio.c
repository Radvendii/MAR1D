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
    sprintf(fn_, "../sounds/%s", fn);
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
        playDaemon();
        exit(0);
    }
    close(piperw[0]);
}

void playDaemon(){
    int snd;
    close(piperw[1]);
    while(read(piperw[0], &snd, sizeof(int)) > 0){
        if(fork() == 0){
            au_initEach();
            ao_play(device, sounds[snd], sz[snd]);
            au_deinitEach();
            exit(0);
        }
    }
    close(piperw[0]);
}

void au_initEach(){
    device = ao_open_live(default_driver, &format, NULL);
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
    ao_shutdown();
}

void au_deinitEach(){
    ao_close(device);
}

void au_play(int snd){
    write(piperw[1], &snd, sizeof(int));
}

void au_mainPlay(int snd){
    static bool first = true;
    if(first){
        first = false;
    }
    else{
        au_mainStop();
    }
    au_play(snd);
    //pthread_create(&mainThread, NULL, au_threaded, snd);
    //pthread_detach(mainThread);
}

void au_mainStop(){
    //pthread_cancel(mainThread);
}
