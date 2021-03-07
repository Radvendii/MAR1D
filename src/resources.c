#include "resources.h"

char *configDir;
char *rs_resourceDir;

void rs_init(){

  configDir = NULL;

#ifdef _WIN32
  // On windows, just use the directory of the program as a config dir
  configDir = strdup("");
#else
  // On real operating systems, use a proper config dir.
  // XDG_CONFIG_HOME if it exists, otherwise ~/.config/

  char *xdg_config_home = getenv("XDG_CONFIG_HOME");
  char *home = getenv("HOME");

  if(xdg_config_home){
    configDir = salloc(strlen(xdg_config_home) + 2);
    sprintf(configDir, "%s/", xdg_config_home);
  }
  else if(home){
    configDir = salloc(strlen(home) + sizeof("/.config/") + 1);
    sprintf(configDir, "%s/.config/", home);
  }
  else {
    configDir = strdup("~/.config/");
  }
#endif

  rs_resourceDir = getenv("MAR1D_RESOURCE_DIR");
  if (!rs_resourceDir) {
    rs_resourceDir = k_resourceDir;
  }
}

void rs_deinit(){
  free(configDir);
  configDir = NULL;
}

char *rs_getFn(char *fn){
  char *fn_ = salloc(strlen(fn) + 2 + strlen(rs_resourceDir));
  sprintf(fn_, "%s/%s", rs_resourceDir, fn);
  return(fn_);
}

// Prepends the resource directory name and then opens that file as either text or binaryj
FILE* rs_getFile(char* fn){
  char *fn_ = rs_getFn(fn);
  FILE *f = sfopen(fn_, "r");
  free(fn_);
  return f;
}

FILE* rs_getBFile(char* fn){
  char *fn_ = rs_getFn(fn);
  FILE *f = sfopen(fn_, "rb");
  free(fn_);
  return f;
}

//

FILE* rs_getConfigRead(){
  char *fn_ = salloc(strlen(configDir) + 2 + sizeof(k_configFileName));
  sprintf(fn_, "%s"k_configFileName, configDir);
  FILE *f = fopen(fn_, "r"); // fopen not sfopen because in this case we don't care that much if it fails
  free(fn_);
  return f;
}

FILE* rs_getConfigWrite(){
  char *fn_ = salloc(strlen(configDir) + 2 + sizeof(k_configFileName));
  sprintf(fn_, "%s"k_configFileName, configDir);
  FILE *f = fopen(fn_, "w"); // fopen not sfopen because in this case we don't care that much if it fails
  free(fn_);
  return f;
}

// get the file name to write the game recording
// by default it gets written to the current directory
// (executable directory on windows)
// directory can be specified with the MAR1D_RECORDING_DIR environment variable
char* rs_getRecFn(){
  char *recdir = NULL;
  recdir = getenv("MAR1D_RECORDING_DIR");
  char *fn_ = salloc((recdir ? strlen(recdir) : 0) + 3 + sizeof(k_recFileName) + sizeof(k_recFileExt) + 32); // 10^32 > INT_MAX so we should be good
  FILE *f;
  // find the first file file we haven't already created.
  for(int i=0; i < INT_MAX; i++) {
    sprintf(fn_, "%s%s"k_recFileName"_%d"k_recFileExt, (recdir ? recdir : ""), (recdir ? "/" : ""), i);
    if (f = fopen(fn_, "r")) {
      fclose(f);
    }
    else {
      break;
    }
  }
  return fn_;
}
