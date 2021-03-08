#include "parsing.h"

color* io_cs;
obj* io_os;

// recording of the current playthrough (but with x and y axis flipped)
image io_recording;

// this does not extend the data memory, so io_recording must be allocated with enough space to begin with.
void io_recAddFrame(unsigned char *frame) {
  // increase allocated size by doubling, so that we don't have to do it every frame
  static int maxSizeY = 0;
  if (maxSizeY < io_recording.sizeY+1) {
    maxSizeY *= 2;
    // just to make sure (there's an issue with the first iteration)
    BOUND_BELOW(maxSizeY, io_recording.sizeY+1);
    resalloc((void **) &io_recording.data, maxSizeY * io_recording.sizeX * 3 * sizeof(unsigned char));
  }

  memcpy(io_recording.data + io_recording.sizeX * io_recording.sizeY * 3, frame, sizeof(unsigned char) * k_nPixels * 3);


  io_recording.sizeY++;
}

void io_resetRec() {
  gr_unbindImage(&io_recording);
  io_recording.sizeY = 0;
  io_recording.sizeX = k_nPixels;
}

void io_writeRec() {
  char *fn = rs_getRecFn();
  image playthrough = io_flippedImage(io_recording);
  io_writeImPng(fn, playthrough);
  free(fn);
  free(playthrough.data);
}

// the returned image has separately allocated data. caller is responsible for freeing.
image io_flippedImage(image im) {
  image new;
  new.sizeX = im.sizeY;
  new.sizeY = im.sizeX;
  new.texture = 0;
  new.data = salloc(sizeof(unsigned char) * new.sizeX * new.sizeY * 3);
  for(int i=0; i < new.sizeY; i++) {
    for (int j=0; j < new.sizeX; j++) {
      memcpy(new.data + (i * new.sizeX + j) * 3, im.data + (j * im.sizeX + i) * 3, sizeof(unsigned char) * 3);
    }
  }
  return new;
}

void io_writeImPng(const char *fn, image im) {
  // need alpha channel
  unsigned char *buf = salloc(im.sizeX * im.sizeY * 4 * sizeof(unsigned char));
  for (int j=0; j < im.sizeY; j++) {
    for (int i=0; i < im.sizeX; i++) {
      // copy RGB but flip vertically because that's how lodepng wants it
      memcpy(buf + (j * im.sizeX + i) * 4, im.data + ((im.sizeY - j) * im.sizeX + i) * 3, sizeof(unsigned char) * 3);
      buf[(j * im.sizeX + i) * 4 + 3] = (unsigned char) -1; //max alpha
    }
  }

  unsigned error = lodepng_encode32_file(fn, buf, im.sizeX, im.sizeY);
  if(error) {
    DEBUG("Error writing png \"%s\" %u: %s", fn, error, lodepng_error_text(error));
  }

  free(buf);
}

// Reads a font specification from a file. 
// The file should start with a number specifying the size of each character in pixels.
// Then it should have a grid for each character in the ascii character set, where 1 represents the pixel being on and 0 (or anything else) represents it being off.
int io_getFont(bool** font, char* fn){
  FILE* f = rs_getFile(fn);
  int c;
  int i=0;
  int fontSize;

  sfscanf(f, "%d\n", &fontSize);

  *font = salloc(sizeof(bool)*fontSize*128);

  while((c = fgetc(f)) != EOF){
    if(c == '\n'){continue;}
    else{(*font)[i++] = (c == '1' ? true : false);}
  }

  sfclose(f);
  return fontSize;
}

// Parses a color. Color data should start with a C and then be followed by a character specifying the name of the color, then the R, G, B values from 0-255 separated by '.'s.
// e.g. "CG:0.171.0" assigns a shade of green to the character 'G'
void io_getColor(FILE* f, color *c){
  sfscanf(f, ":");
  sfscanf(f, "%hhd.%hhd.%hhd\n", &c->r, &c->g, &c->b);
}

// Parses an object. Object data should start with an O, followed by a character specifying the name of the object.
// It must then have a number, specifying the number of frames of animation the object has. Then a number of attributes:
// ps: The number of points in each frame. (this must be constant between frames. Use the '0' point to fill in points when needed)
// pos: The position of the top left corner of the bounding box, relative to the top left corner of the object.
// dim: The dimensions of the bounding box.
// cols: The number of colision boxes, for specifying different parts of the object that can act in different ways when they collide with something.
// grav: 1 if the object should fall with gravity, 0 otherwise
// phys: 1 if the object is physical and should colide with things, 0 otherwise
// hid: 1 if the object should be hidden / invisible, 0 otherwise.
// The next lines contain the specifications of the collision boxes. They have a C, and then `pos` and `dim`, just as above
// For example, the speicification of the fireball object looks as follows:
// Oo4; ps:23; pos:0,0; dim:8x8; cols:3; grav:1; phys:0; hid:0;
// C; pos:0,5; dim:1x1;
// C; pos:7,5; dim:1x1;
// C; pos:1,7; dim:5x1;
//   1111
//  r    1
// r   . 1
// r     1
// r   11
// r  r
//  r r ,
//   r,, ,
//   rrrr
//  r    r
//  r     1
// rrr    1
// ,  1   1
//  , 1   1
// ,   111
//   ,
// , ,,r
//  , r r
//    r  r
//  11   r
// 1   . r
// 1     r
// 1    r
//  1111
//      ,
//  111   ,
// 1   1 ,
// 1   1  ,
// 1    rrr
// 1     r
//  r    r
//   rrrr
void io_getObj(FILE* f, obj *o, char oname, color cs[CHAR_MAX]) {
  int nFrames;
  int size;
  int w, h;
  int nCols;
  int xpos, ypos, xhei, yhei;
  int grav;
  int phys;
  int hid;
  int x,y,i,j;
  char c;
  sfscanf(f, "%d; ps:%d; pos:%d,%d; dim:%dx%d; cols:%d; grav:%d; phys:%d; hid:%d;", &nFrames, &size, &xpos, &ypos, &w, &h, &nCols, &grav, &phys, &hid);
  *o = (obj) {
    .type = oname,
    .gravity = grav,
    .physical = phys,
    .hidden = hid,
    .active = true,
    .flip = false,
    .onScreen = false,
    .bb = (box) {
      .x = xpos,
      .y = -ypos,
      .w = w,
      .h = -h
    },
    .nFrames = nFrames,
    .frames = salloc(sizeof(objFrame) * nFrames),
    .nCols = nCols,
    .cols = salloc(sizeof(box) * nCols)
  };
  i=0;
  for (i=0; i < nCols; i++) {
    sfscanf(f, "\nC; pos:%d,%d; dim:%dx%d;", &xpos, &ypos, &xhei, &yhei);
    o->cols[i] = (box) {.x = xpos, .y = -ypos, .w = xhei, .h = -yhei};
  }
  for(j=0;j<nFrames;j++){
    y=0; x=0; i=0;
    char imFn[64];
    sprintf(imFn, "2D/%c_%d.png", oname, j);
    o->frames[j].im = io_getImage(imFn);
    o->frames[j].ps = salloc(sizeof(point) * (size+1));
    while(i<size){
      switch(c = fgetc(f)){
      case ' ':
        x++;
        break;
      case '\n':
        y--;
        x=0;
        break;
      case '0':
        o->frames[j].ps[i++] = p_skipPoint;
        x++;
        break;
      default:
        o->frames[j].ps[i++] = (point) {.x = x, .y = y, .c = c};
        x++;
        break;
      }
    }
    o->frames[j].ps[i] = p_termPoint;
  }
}

// Parses a level
// Levels should be specified by an L followed by a character label for the level.
// Then the number of objects in the level.
// Then a grid, specifying where objects of each type are.
// There is special syntax for question mark blocks. The character above that block will not be an actual object in the scene, but will rather be the object contained in the block.
// The character immediately following a pipe character is ignored, so that you can specify pipes as two-wide.
// e.g.
// ==
// ||
// ||
// ||
// After the level, there can be any number of property definitions. This is the character P followed by the number object in the scene that should have this property set, and then the property, and then the value the property should be set to.
// h: object should be Hidden
// v: object should Visually look like the object specified (char)
// j: the `.j` attribute should be set as specified (int)
// c: the `.c` attribute should be set as specified (char)
void io_getLevel(FILE* f, level *l, obj os[CHAR_MAX]){
  int size;
  char c;
  sfscanf(f, ":%d", &size);
  int y=0,x=0,i=0;
  *l = salloc(sizeof(obj) * (size*3+1));
  while(i<size){
    switch(c = fgetc(f)){
    case ' ':
      x++;
      break;
    case '\n':
      y--;
      x=0;
      break;
    default:
      (*l)[i] = os[c];
      (*l)[i].x = x*16;
      (*l)[i].y = y*16;
      //TODO: These should not be hard-coded
      if(c == '&'){
        (*l)[i].y += 7;
      }
      if(c == '7'){
        (*l)[i].y -= 3;
      }
      if(c == '?'){
        (*l)[i].c = '.';
        for(int j=0;j<i;j++){
          if((*l)[j].x == (*l)[i].x && (*l)[j].y == (*l)[i].y+16){
            (*l)[i].c = (*l)[j].type;
            (*l)[j] = os['.']; //TODO: This shouldn't be hard-coded if the object itself is defined in the data file
            break;
          }
        }
      }
      if(c == '|' || c == '='){
        fgetc(f);
        x++;
        (*l)[++i] = os['.'];
        (*l)[i].x = x*16;
        (*l)[i].y = y*16;
      }
      i++;
      x++;
      break;
    }
  }
  (*l)[i].type = '\0';

  // Parse the properties
  char prop;
  char obj;
  int j;
  fgetc(f);
  while(fgetc(f) == 'P'){ // Stands for "Property" //TODO: better syntax for this
    sfscanf(f, ";%d%c", &i, &prop);
    switch(prop){
    case 'h':
      (*l)[i].hidden = true;
      break;
    case 'v': // Transforms the visual appearance of the object
      sfscanf(f, "%c", &obj);
      (*l)[i].frames = os[obj].frames;
      (*l)[i].nFrames = os[obj].nFrames;
      break;
    case 'j':
      sfscanf(f, "%d", &j);
      (*l)[i].j = j;
      break;
    case 'c':
      sfscanf(f, "%c", &c);
      (*l)[i].c = c;
      break;
    }
    fgetc(f);
  }
}

// The main entry point for parsing the data file. Writes to the object and color maps is_os and io_cs.
// Data is specified by beginning with a C for a color, an O for an object and an L for a level.
// Parsing of each type of data is handled by the functions above.
void io_getLevels(level** ls, char* fn){
  FILE *f = rs_getFile(fn);
  // TODO: who the fuck owns io_os / io_cs?? if it's io_ then it should be parsing.c, but we don't even have an init() or deinit() function
  io_os = salloc(sizeof(obj) * CHAR_MAX);
  memset(io_os, 0, sizeof(obj) * CHAR_MAX);
  io_cs = salloc(sizeof(color) * CHAR_MAX);
  memset(io_cs, 0, sizeof(color) * CHAR_MAX);
  *ls = salloc(sizeof(level) * CHAR_MAX);
  memset(*ls, 0, sizeof(level *) * CHAR_MAX);
  char c;
  char name = '\0';
  while((c = fgetc(f)) != EOF){
    if (c == 'C' || c == 'O' || c == 'L') {
      name = fgetc(f);
    }
    switch(c){
    case '\n':
      break;
    case 'C':
      io_getColor(f, io_cs + name);
      break;
    case 'O':
      io_getObj(f, io_os + name, name, io_cs);
      break;
    case 'L':
      io_getLevel(f, *ls + name, io_os);
      break;
    default:
      while(fgetc(f) != '\n');
      break;
    }
  }
  sfclose(f);
  return;
}

// Reads the config, and reutrns the config object.
void io_readConfig(config *c){
  FILE *f = rs_getConfigRead();
  if(!f){
    DEBUG("Unable to read configuration file. Using default.");
    return; // return without modifying c
  }
  config_t conf;
  config_init(&conf);
  config_read(&conf, f);

  // If any options are not specified in the config file, they will simply not be modified
  config_lookup_int(&conf, "music", &(c->music));
  config_lookup_int(&conf, "effects", &(c->effects));
  config_lookup_int(&conf, "lineSize", &(c->lineSize));
  config_lookup_int(&conf, "sensitiviy", &(c->sensitivity));
  config_lookup_bool(&conf, "invertMouseY", (int*) &(c->invertMouseY));
  config_lookup_bool(&conf, "visualSounds", (int*) &(c->visualSounds));
  config_lookup_bool(&conf, "debug", (int*) &(c->debug));

  const char *key = NULL;

#define GET_KEY(k)                                  \
  do {                                              \
    config_lookup_string(&conf, "keys."#k, &key);   \
    c->keys.k = key ? SDL_GetKeyFromName(key) : 0;  \
    key = NULL;                                     \
  } while (0)

  GET_KEY(forward);
  GET_KEY(backward);
  GET_KEY(turn);
  GET_KEY(run);
  GET_KEY(crouch);
  GET_KEY(jump);


  fclose(f);
  config_destroy(&conf);
}

// Writes a config to the config file
void io_writeConfig(config c){
  FILE *f = rs_getConfigWrite();
  if(!f){
    DEBUG("Unable to write configuration file.");
    return;
  }
  config_t conf;
  config_setting_t *root, *set;
  config_init(&conf);

  root = config_root_setting(&conf);

  set = config_setting_add(root, "music", CONFIG_TYPE_INT);
  config_setting_set_int(set, c.music);

  set = config_setting_add(root, "effects", CONFIG_TYPE_INT);
  config_setting_set_int(set, c.effects);

  set = config_setting_add(root, "lineSize", CONFIG_TYPE_INT);
  config_setting_set_int(set, c.lineSize);

  set = config_setting_add(root, "sensitivity", CONFIG_TYPE_INT);
  config_setting_set_int(set, c.sensitivity);

  set = config_setting_add(root, "visualSounds", CONFIG_TYPE_BOOL);
  config_setting_set_bool(set, c.visualSounds);

  set = config_setting_add(root, "invertMouseY", CONFIG_TYPE_BOOL);
  config_setting_set_bool(set, c.invertMouseY);

  /*
   * only write debug back to the config if it's true. I want this to be a fairly
   * inaccessible option, so it shouldn't be just showing up in people's configs
   * TODO: possibly have the same setup for all the options. Only write them
   *       back if they're different than the default.
   */
  if (c.debug) {
    set = config_setting_add(root, "debug", CONFIG_TYPE_BOOL);
    config_setting_set_bool(set, c.debug);
  }

  config_setting_t *keys;

  keys = config_setting_add(root, "keys", CONFIG_TYPE_GROUP);

#define SET_KEY(k)                                              \
  do {                                                          \
    if (c.keys.k) {                                             \
      set = config_setting_add(keys, #k, CONFIG_TYPE_STRING);   \
      config_setting_set_string(set, SDL_GetKeyName(c.keys.k)); \
    }                                                           \
  } while (0)

  SET_KEY(forward);
  SET_KEY(backward);
  SET_KEY(turn);
  SET_KEY(run);
  SET_KEY(crouch);
  SET_KEY(jump);

  config_write(&conf, f);

  fclose(f);
  config_destroy(&conf);
}

image io_getImage(char *fn) {
  image im;

  char *fn_ = rs_getFn(fn);

  unsigned error = lodepng_decode32_file(&im.data, &im.sizeX, &im.sizeY, fn_);
  if(error) {
    DEBUG("Error reading png \"%s\" %u: %s", fn, error, lodepng_error_text(error));
  }

  free(fn_);

  im.texture = 0;

  return im;
}
