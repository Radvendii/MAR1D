#include "parsing.h"

// Reads a font specification from a file. 
// The file should start with a number specifying the size of each character in pixels.
// Then it should have a grid for each character in the ascii character set, where 1 represents the pixel being on and 0 (or anything else) represents it being off.
int io_getFont(bool** font, char* fn){
  FILE* f = rs_getFile(fn);
  int c;
  int i=0;
  int fontSize;

  fscanf(f, "%d\n", &fontSize);

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
void io_getColor(FILE* f, color cs[127]){ //cs must be an array 127 big. One for each character
  char cname;
  color c;
  fscanf(f, "%c:", &cname);
  fscanf(f, "%hhd.%hhd.%hhd\n", &cs[cname].r, &cs[cname].g, &cs[cname].b);
  return;
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
void io_getObj(FILE* f, obj os[127], color cs[127]){ //TODO: Make objects have arrays of point arrays
  char oname;
  int nps;
  int size;
  int w, h;
  int nCols;
  int xpos, ypos, xhei, yhei;
  int grav;
  int phys;
  int hid;
  int x,y,i,j;
  char c;
  fscanf(f, "%c%d; ps:%d; pos:%d,%d; dim:%dx%d; cols:%d; grav:%d; phys:%d; hid:%d;", &oname, &nps, &size, &xpos, &ypos, &w, &h, &nCols, &grav, &phys, &hid);
  os[oname].type = oname;
  os[oname].gravity = grav;
  os[oname].physical = phys;
  os[oname].hidden = hid;
  os[oname].active = true;
  os[oname].x = 0;
  os[oname].y = 0;
  os[oname].vx = 0;
  os[oname].vy = 0;
  os[oname].i = 0;
  os[oname].j = 0;
  os[oname].c = 0;
  os[oname].animFrame = 0;
  os[oname].flip = false;
  os[oname].onScreen = false;
  os[oname].nps = nps;
  os[oname].bb = (box) {.x = xpos, .y = -ypos, .w = w, .h = -h};
  os[oname].ps = salloc(sizeof(point*) * nps);
  os[oname].cols = salloc(sizeof(box) * nCols);
  os[oname].nCols = nCols;
  i=0;
  while(i<nCols){
    fscanf(f, "\nC; pos:%d,%d; dim:%dx%d;", &xpos, &ypos, &xhei, &yhei);
    os[oname].cols[i++] = (box) {.x = xpos, .y = -ypos, .w = xhei, .h = -yhei};
  }
  for(j=0;j<nps;j++){
    y=0; x=0; i=0;
    os[oname].ps[j] = salloc(sizeof(point) * (size+1));
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
        os[oname].ps[j][i++] = p_skipPoint;
        x++;
        break;
      default:
        os[oname].ps[j][i++] = (point) {.x = x, .y = y, .c = c};
        x++;
        break;
      }
    }
    os[oname].ps[j][i] = p_termPoint;
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
void io_getLevel(FILE* f, level ls[127], obj os[127]){
  char lname;
  int size;
  char c;
  int y=0,x=0,i=0;
  fscanf(f, "%c:%d", &lname, &size);
  ls[lname] = salloc(sizeof(obj) * (size*3+1));
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
      ls[lname][i] = os[c];
      ls[lname][i].x = x*16;
      ls[lname][i].y = y*16;
      //TODO: These should not be hard-coded
      if(c == '&'){
        ls[lname][i].y += 7;
      }
      if(c == '7'){
        ls[lname][i].y -= 3;
      }
      if(c == '?'){
        ls[lname][i].c = '.';
        for(int j=0;j<i;j++){
          if(ls[lname][j].x == ls[lname][i].x && ls[lname][j].y == ls[lname][i].y+16){
            ls[lname][i].c = ls[lname][j].type;
            ls[lname][j] = os['.']; //TODO: This shouldn't be hard-coded if the object itself is defined in the data file
            break;
          }
        }
      }
      if(c == '|' || c == '='){
        fgetc(f);
        x++;
        ls[lname][++i] = os['.'];
        ls[lname][i].x = x*16;
        ls[lname][i].y = y*16;
      }
      i++;
      x++;
      break;
    }
  }
  ls[lname][i].type = '\0';

  // Parse the properties
  char prop;
  char obj;
  int j;
  fgetc(f);
  while(fgetc(f) == 'P'){ // Stands for "Property" //TODO: better syntax for this
    fscanf(f, ";%d%c", &i, &prop);
    switch(prop){
    case 'h':
      ls[lname][i].hidden = true;
      break;
    case 'v': // Transforms the visual appearance of the object
      fscanf(f, "%c", &obj);
      ls[lname][i].ps = os[obj].ps;
      ls[lname][i].nps = os[obj].nps;
      break;
    case 'j':
      fscanf(f, "%d", &j);
      ls[lname][i].j = j;
      break;
    case 'c':
      fscanf(f, "%c", &c);
      ls[lname][i].c = c;
      break;
    }
    fgetc(f);
  }
}

// The main entry point for parsing the data file. Writes to the object and color maps is_os and io_cs.
// Data is specified by beginning with a C for a color, an O for an object and an L for a level.
// Parsing of each type of data is handled by the functions above.
//TODO: Put parsing of the char after the data specifier in this function, then simplify the functions above.
void io_getLevels(level** ls, char* fn){
  FILE *f = rs_getFile(fn);
  io_os = salloc(sizeof(obj) * 127);
  for(int i=0;i<127;i++){io_os[i].ps = NULL;}
  io_cs = salloc(sizeof(color)*127);
  *ls = salloc(sizeof(level)*127);
  char c;
  for(int i=0;i<127;i++){(*ls)[i] = NULL;}
  while((c = fgetc(f)) != EOF){
    switch(c){
    case '\n':
      break;
    case 'C':
      io_getColor(f, io_cs);
      break;
    case 'O':
      io_getObj(f, io_os, io_cs);
      break;
    case 'L':
      io_getLevel(f, *ls, io_os);
      break;
    default:
      while(fgetc(f) != '\n');
      break;
    }
  }
  sfclose(f);
  return;
}

// Functions for loading the menuscreen texture.
//TODO: Clean this up and give these functions better names.
image * loadTexture(){
  image *image1;

  image1 = (image *) salloc(sizeof(image));

  if (!getImage(image1, "menuscreen.bmp")) {
    exit(1);
  }

  return image1;
}

int getImage(image *image, char *fn) {
  FILE *file = rs_getBFile(fn);
  unsigned long size; // size of the image in bytes.
  char temp; // temporary color storage for bgr-rgb conversion.

  image->sizeX = 256;
  image->sizeY = 240;

  size = image->sizeX * image->sizeY * 3;

  // seek past the bitmap header.
  fseek(file, 54, SEEK_CUR);

  // read the data.
  image->data = (unsigned char *) malloc(size);

  if (image->data == NULL) {
    printf("Error allocating memory for color-corrected image data");
    sfclose(file);
    return 0;
  }

  if (fread(image->data, size, 1, file) != 1) {
    printf("Error reading image data from %s.\n", fn);
    sfclose(file);
    return 0;
  }

  for (int i=0;i<size;i+=3) { // reverse all of the colors. (bgr -> rgb)
    temp = image->data[i];
    image->data[i] = image->data[i+2];
    image->data[i+2] = temp;
  }

  sfclose(file);
  return 1;
}
