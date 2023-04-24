#ifndef MATH_H
#define MATH_H

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "quat.h"
#include "mat4.h"

template<typename T>
vec3t<T> QuatToEuler(const quatt<T>& q, bool isCastToDegrees = true)
{
    return isCastToDegrees ? degrees(q.ToEuler()) : q.ToEuler();
}
template<typename T>
quatt<T> EulerToQuat(const vec3t<T>& e, bool isCastToRadians = true)
{
    return quatt<T>(isCastToRadians ? radians(e) : e);
}



#endif