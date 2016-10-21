#include "helpers.h"

void* salloc(size_t size){ //Named such after "Safe allocate"
  void *ptr = malloc(size);
  if(ptr == NULL){printf("Error in salloc(): malloc() failed with size %lu\n", size);exit(1);}
  else{return ptr;}
}

FILE* sfopen(char* s, char* m){
  FILE* f;
  if((f = fopen(s, m)) == NULL){printf("Error in sfopen(): File not found: %s\n", s);exit(1);}
  return f;
}

void sfclose(FILE* f){
  if(fclose(f) == EOF){printf("Error in sfclose(): fclose() returned EOF\n");exit(1);}
  return;
}

void* resalloc(void* ptr, size_t size){ //TODO: make this take in a void**
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

int intcmp(const void *aa, const void *bb){
  const int *a = aa, *b = bb;
  return (*a < *b) ? -1 : (*a > *b);
}

void intsort(int* l, int s){
  qsort(l, s, sizeof(int), intcmp);
}

double linInterp(double y1, double y2, double x1, double x2, double val){
  return (y2-y1)/(x2-x1)*val+y2;
}

