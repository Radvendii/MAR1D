#include "resources.h"

// Prepends the resource directory name and then opens that file as either text or binary

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

