#include "audio.h"
static pthread_key_t key;
int default_driver;
ao_sample_format format;
static pthread_t mainThread;
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
    ao_initialize();

    memset(&format, 0, sizeof(format));
    format.bits = 16;
    format.channels = 1;
    format.rate = 44100;

    format.byte_format = AO_FMT_LITTLE;
    default_driver = ao_default_driver_id();
    pthread_key_create(&key, NULL);
    au_loadSounds();
    //for(int i=0;i<20;i++){
        //au_play(SND_jump);
    //}
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
    /*
     *ao_shutdown();
     *for(int i=0;i<k_nSounds;i++){
     *    free(sounds[i]);
     *}
     */
}

void* au_threaded(void* arg){
    printf("here\n");
    ao_device* device = ao_open_live(default_driver, &format, NULL);
    printf("here2\n");
    pthread_setspecific(key, ao_open_live(default_driver, &format, NULL));
    pthread_cleanup_push(au_threadCleanup, pthread_getspecific(key));
    //ao_play(device, sounds[(int)arg], sz[(int)arg]);
    ao_close(device);
    ao_play(pthread_getspecific(key), sounds[(int)arg], sz[(int)arg]);
    pthread_cleanup_pop(1);
    return NULL;
}

void au_threadCleanup(void* arg){
    ao_close((ao_device*) arg);
}

void au_play(int snd){
    pthread_t pth;
    pthread_create(&pth, NULL, au_threaded, snd);
    pthread_detach(pth);
}

void au_mainPlay(int snd){
    static bool first = true;
    if(first){
        first = false;
    }
    else{
        au_mainStop();
    }
    pthread_create(&mainThread, NULL, au_threaded, snd);
    pthread_detach(mainThread);
}

void au_mainStop(){
    pthread_cancel(mainThread);
}
