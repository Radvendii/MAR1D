#include "helpers.h"
void* salloc(size_t size){ //Named such after "Safe allocate"
    void *ptr = malloc(size);
    if(ptr == NULL){printf("Error in salloc(): malloc() failed with size %lu\n", size);exit(1);}
    else{return ptr;}
}

void* resalloc(void* ptr, size_t size){
    void *ret;
    if(ptr == NULL){
        ret = salloc(size);
    }
    else{
        ret = realloc(ptr, size);
        if(ret == NULL){printf("Error #1 in resalloc(): realloc() failed with size %lu\n", size);exit(1);}
        if(ptr == NULL){printf("Error #2 in resalloc(): realloc() failed with size %lu\n", size);exit(1);}
    }
    return ret;
}
