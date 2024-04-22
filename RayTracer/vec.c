#include "vec.h"
#include <math.h>

VEC_T normalize(VEC_T v) {
    double divisor = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    VEC_T output;
    output.x = v.x / divisor;
    output.y = v.y / divisor;
    output.z = v.z / divisor;
    return output;
}
