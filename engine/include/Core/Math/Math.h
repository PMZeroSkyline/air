#ifndef MATH_H
#define MATH_H

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "quat.h"
#include "mat4.h"

template<typename T>
vec3t<T> QuatToEuler(const quatt<T>& q)
{
    return degrees(q.toEuler());
}
template<typename T>
quatt<T> EulerToQuat(const vec3t<T>& e)
{
    return quatt<T>(radians(e));
}



#endif