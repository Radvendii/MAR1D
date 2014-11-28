#ifndef _OBJECT_H
#define _OBJECT_H

#define k_nPixels 500 //resolution of screen
#define k_FOV 60 //Field of vision (degrees)
#define k_nMaxLinesPerObj 10 //maximum number of lines per object
#define k_nMaxObj 50 //maximum number of objects in a world

//labeling all the types of objects (enum)
enum objType {
    objGround
}


//objects possible in the scene
const int ground[k_nMaxLinesPerObj * 7] = { //two coordinates define a line (frist four) and color (last three)
    -7,  7,  2,  7, 231, 95, 19,
     2,  7,  3,  7, 0,   0,  0 ,
     3,  7,  6,  7, 231, 95, 19,
     6,  7,  7,  7, 0,   0,  0 ,
    -7,  7, -7, -2, 231, 95, 19,
    -7, -2, -7, -3, 0,   0,  0 ,
    -7, -3, -7, -6, 231, 95, 19,
    -7, -6, -7, -7, 0,   0,  0   };

#endif
