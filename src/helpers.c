#include "helpers.h"

void* salloc(size_t size){ //Named such after "Safe allocate"
  void *ptr = malloc(size);
  if(ptr == NULL){printf("Error in salloc(): malloc() failed with size %lu\n", size);exit(EXIT_FAILURE);}
  else{return ptr;}
}

FILE* sfopen(char* s, char* m){
  FILE* f;
  if((f = fopen(s, m)) == NULL){printf("Error in sfopen(): File not found: %s\n", s);exit(EXIT_FAILURE);}
  return f;
}

void sfclose(FILE* f){
  if(fclose(f) == EOF){printf("Error in sfclose(): fclose() returned EOF\n");exit(EXIT_FAILURE);}
  return;
}

void* resalloc(void* ptr, size_t size){ //TODO: make this take in a void**
  void *ret;
  if(ptr == NULL){
    ret = salloc(size);
  }
  else{
    ret = realloc(ptr, size);
    if(ret == NULL){printf("Error #1 in resalloc(): realloc() failed with size %lu\n", size);exit(EXIT_FAILURE);}
    if(ptr == NULL){printf("Error #2 in resalloc(): realloc() failed with size %lu\n", size);exit(EXIT_FAILURE);}
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

/*
 * makes a copy of obj on the heap, and returns a pointer to the copy.
 * size is the size of the object
 */
void *heap_copy(void *obj, size_t size) {
  void *ret = malloc(size);
  memcpy(ret, obj, size);
  return ret;
}

double linInterp(double y0, double y1, double x0, double x1, double x){
  return (y1-y0)/(x1-x0)*(x-x0)+y0;
}

double subLinInterp(double y0, double y1, double x0, double x1, double x){
  double a = (x1 - x0) / SQUARE((y1 - y0));
  return sqrt((x - x0) / a) + y0;

}
