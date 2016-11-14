#include "resources.h"

FILE* rs_getFile(char* fn){
  char *fn_ = salloc(sizeof(char) * (strlen(fn) + 1) + sizeof(k_resources));
  sprintf(fn_, k_resources"%s", fn);
  FILE *f = sfopen(fn_, "r");
  free(fn_);
  return f;
}

FILE* rs_getBFile(char* fn){
  char *fn_ = salloc(sizeof(char) * (strlen(fn) + 1) + sizeof(k_resources));
  sprintf(fn_, k_resources"%s", fn);
  FILE *f = sfopen(fn_, "rb");
  free(fn_);
  return f;
}

