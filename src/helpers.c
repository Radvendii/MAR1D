#include "helpers.h"

config conf;

bool quit; // global variable for exiting all loops and quitting the game

int err; // global variable for capturing error codes

// wrapper to return a formatted string of the correct size
// caller is responsible for freeing result
char *vrprintf(char *fmtstr, va_list ap) { // Named such after "Return printf"
  va_list aq;
  va_copy(aq, ap);
  char *str = NULL;

  size_t needed = vsnprintf(str, 0, fmtstr, aq) + 1;
  str = salloc(needed);

  vsprintf(str, fmtstr, ap);

  va_end(aq);
  return str;
}

char *rprintf(char *fmtstr, ...) {
  va_list ap;
  va_start(ap, fmtstr);
  char *ret = vrprintf(fmtstr, ap);
  va_end(ap);
  return ret;
}

void sfscanf(FILE *f, char *fmtstr, ...) {
  va_list vargs;
  va_start(vargs, fmtstr);

  int nFmts = 0;
  // count how many results to expect
  for(char *c = fmtstr; *c != '\0'; c++) {
    if (*c == '%') { // format specifier!
      if (*(c+1) == '%') { // ...or not
        c++;
      }
      else {
        nFmts++;
      }
    }
  }
#ifdef _WIN32
  /*
   * NOTE: For some reason wine can't handle the vfscanf line unless this line
   *       is run somewhere in the program first (it doesn't have to be
   *       immediately before, I just wanted to keep them together). It runs
   *       fine on windows without, but not through wine.
   */
  sscanf(" ", " ");
#endif
  err = vfscanf(f, fmtstr, vargs);
  if (err != nFmts) {
    DEBUG("Error: malformed file (or format string)");
    exit(EXIT_FAILURE);
  }

  va_end(vargs);
}

void* salloc(size_t size){ //Named such after "Safe allocate"
  void *ptr = malloc(size);
  if(ptr == NULL){DEBUG("salloc() failed with size %lu", (unsigned long)size);exit(EXIT_FAILURE);}
  else{return ptr;}
}

FILE* sfopen(char* s, char* m){
  FILE* f;
  if((f = fopen(s, m)) == NULL){DEBUG("File not found: %s", s);exit(EXIT_FAILURE);}
  return f;
}

void sfclose(FILE* f){
  if(fclose(f) == EOF){DEBUG("fclose() returned EOF");exit(EXIT_FAILURE);}
  return;
}

// unlike realloc, this also changes the pointer you pass in
void* resalloc(void** ptr, size_t size){
  void *ret;
  if(*ptr == NULL){
    ret = salloc(size);
  }
  else{
    ret = realloc(*ptr, size);
    if(!ret || !*ptr){DEBUG("resalloc() failed with size %lu", (unsigned long)size);exit(EXIT_FAILURE);}
  }
  *ptr = ret;
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

double _smoothInterp(double x) {
  // (1-cos(x*pi)+1)/2 smoothly interpolates between (0,0) and (1,1)
  return (1 - cos(x*pi)) / 2;
}

// shifting the intial portion of a cos curve
double smoothInterp(double y0, double y1, double x0, double x1, double x) {
  return y0 + _smoothInterp((x - x0) / (x1 - x0)) * (y1 - y0);
}
