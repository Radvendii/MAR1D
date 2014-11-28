#ifndef _OBJECT_H
#define _OBJECT_H

#define k_nPixels 500 //resolution of screen
#define k_FOV 60 //Field of vision (degrees)
#define k_nMaxLinesPerObj 30 //maximum number of lines per object
#define k_nMaxObj 50 //maximum number of objects in a world

//labeling all the types of objects (enum)
enum objType {
    objGround
}


//objects possible in the scene
const int ground[k_nMaxLinesPerObj * 7] = { //two coordinates define a line (first four) and color (last three)
    0,  16, 16, 16, 231, 95, 19,
    16, 16, 16, 0,  231, 95, 19,
    16, 0, 0, 0,    231, 95, 19,
    0,  0, 0, 16,   231, 95, 19
};

#endif
