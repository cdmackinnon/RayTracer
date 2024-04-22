#ifndef RC_H
#define RC_H
#include "vec.h"

typedef struct {
    VEC_T origin;
    VEC_T direction;
}RAY_T;

typedef struct {
    double R;
    double G;
    double B;
}COLOR_T;

typedef struct{
    VEC_T center;
    double radius;
}SPHERE_T;

//linked list nodes for storing input spheres and their colors
typedef struct obj {
    SPHERE_T sphere;
    COLOR_T color;
    struct obj *next;
}OBJ_T;


#endif /* RC_H */
