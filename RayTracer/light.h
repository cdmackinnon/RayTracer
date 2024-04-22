#ifndef LIGHT_H
#define LIGHT_H


#include "vec.h"
#include "rc.h"

typedef struct {
   VEC_T loc;
} LIGHT_T;

COLOR_T illuminate (RAY_T ray, LIGHT_T light, COLOR_T obj_color, VEC_T int_pt, VEC_T normal);

#endif

