#include "helpers.h"
#include <ao/ao.h>

ao_device *device;
int default_driver;
ao_sample_format format;
void au_init(){
    ao_initialize();

    memset(&format, 0, sizeof(format));
    format.bits = 16;
    format.channels = 1;
    format.rate = 44100;

    format.byte_format = AO_FMT_LITTLE;
    device = ao_open_live(default_driver, &format, NULL /* no options */);
    default_driver = ao_default_driver_id();
}

void au_deinit(){
    ao_close(device);
    ao_shutdown();
}

int main(int argc, char** argv){
    au_init();
    char fn[50];
    sprintf(fn, "../sounds/%s.raw", argv[1]);
    FILE* jumpfile = fopen(fn, "rb");
    char *buffer;
    /* -- Play some stuff -- */
    fseek(jumpfile, 0L, SEEK_END);
    int sz = ftell(jumpfile);
    fseek(jumpfile, 0L, SEEK_SET);
    buffer = calloc(sz, sizeof(char));
    fread(buffer, sz, 1, jumpfile);

    ao_play(device, buffer, sz);

    /* -- Close and shutdown -- */
    fclose(jumpfile);

    au_deinit();

    return (0);
}
