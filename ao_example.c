#include "helpers.h"
#include <ao/ao.h>

#define BUF_SIZE 4096

int main(){
    FILE* jumpfile = fopen("../sounds/jump.raw", "rb");
    ao_device *device;
    ao_sample_format format;
    int default_driver;
    char *buffer;
    int buf_size;
    int sample;
    float freq = 440.0;
    int i;

    /* -- Initialize -- */

    fprintf(stderr, "libao example program\n");

    ao_initialize();

    /* -- Setup for default driver -- */

    default_driver = ao_default_driver_id();

    memset(&format, 0, sizeof(format));
    format.bits = 16;
    format.channels = 1;
    format.rate = 44100;
    format.byte_format = AO_FMT_LITTLE;

    /* -- Open driver -- */
    device = ao_open_live(default_driver, &format, NULL /* no options */);
    if (device == NULL) {
        fprintf(stderr, "Error opening device.\n");
        return 1;
    }

    /* -- Play some stuff -- */
    buf_size = format.bits/8 * format.channels * format.rate;
    fseek(jumpfile, 0L, SEEK_END);
    int sz = ftell(jumpfile);
    fseek(jumpfile, 0L, SEEK_SET);
    buffer = calloc(sz, sizeof(char));
    fread(buffer, sz, 1, jumpfile);

    ao_play(device, buffer, sz);

    /* -- Close and shutdown -- */
    ao_close(device);
    fclose(jumpfile);

    ao_shutdown();

    return (0);
}
