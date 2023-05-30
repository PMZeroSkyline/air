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
    return q.ToEuler();
}
template<typename T>
quatt<T> EulerToQuat(const vec3t<T>& e)
{
    return quatt<T>(e);
}
template<typename T>
quatt<T> EulerToQuat(T roll, T pitch, T yaw)
{
    return EulerToQuat(vec3t<T>(roll, pitch, yaw));
}
vec3 ToVec3(const vec4 &v)
{   
    return vec3(v.x, v.y, v.z);
}



#endif