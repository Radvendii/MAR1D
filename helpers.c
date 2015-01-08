#include "helpers.h"
void* salloc(size_t size){ //Named such after "Safe allocate"
    void *ptr = malloc(size);
    if(ptr == NULL){printf("Error in salloc(): malloc() failed with size %lu", size);exit(1);}
    else{return ptr;}
}
