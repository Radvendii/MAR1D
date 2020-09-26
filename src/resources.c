#include "resources.h"

void rs_init(){

  configDir = NULL;

#ifdef _WIN32
  // On windows, just use the directory of the program as a config dir
  configDir = getcwd(NULL, 128);
#else
  // On real operating systems, use a proper config dir.
  // XDG_CONFIG_HOME if it exists, otherwise ~/.config/

  char *xdg_config_home = getenv("XDG_CONFIG_HOME");
  char *home = getenv("HOME");

  if(xdg_config_home){
    configDir = salloc(strlen(xdg_config_home) + 1);
    strcpy(configDir, xdg_config_home);
  }
  else if(home){
    configDir = salloc(strlen(home) + sizeof("/.config"));
    strcpy(configDir, home);
    strcat(configDir, "/.config");
  }
  else {
    configDir = salloc(strlen("~.config"));
    strcpy(configDir, "~/.config");
  }
#endif

  if(!configDir) {
    printf("Could not fetch configuration directory. Aborting.\n");
    exit(EXIT_FAILURE);
  }

}

void rs_deinit(){
  free(configDir);
  configDir = NULL;
}

// Prepends the resource directory name and then opens that file as either text or binary
FILE* rs_getFile(char* fn){
  char *fn_ = salloc(strlen(fn) + 1 + sizeof(k_resourceDir));
  sprintf(fn_, k_resourceDir"%s", fn);
  FILE *f = sfopen(fn_, "r");
  free(fn_);
  return f;
}

FILE* rs_getBFile(char* fn){
  char *fn_ = salloc(strlen(fn) + 1 + sizeof(k_resourceDir));
  sprintf(fn_, k_resourceDir"%s", fn);
  FILE *f = sfopen(fn_, "rb");
  free(fn_);
  return f;
}

//

FILE* rs_getConfigRead(){
  char *fn_ = salloc(strlen(configDir) + 2 + sizeof(k_configFileName));
  sprintf(fn_, "%s/"k_configFileName, configDir);
  FILE *f = fopen(fn_, "r"); // fopen not sfopen because in this case we don't care that much if it fails
  free(fn_);
  return f;
}

FILE* rs_getConfigWrite(){
  char *fn_ = salloc(strlen(configDir) + 2 + sizeof(k_configFileName));
  sprintf(fn_, "%s/"k_configFileName, configDir);
  FILE *f = fopen(fn_, "w"); // fopen not sfopen because in this case we don't care that much if it fails
  free(fn_);
  return f;
}
