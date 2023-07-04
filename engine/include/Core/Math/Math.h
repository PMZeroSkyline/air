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
void NormalOrientedHemisphere(int count, vector<vec3>& out)
{
    uniform_real_distribution<float> d(0.f, 1.f);
    default_random_engine e;
    out.resize(count);
    for (int i = 0; i < count; i++)
    {
        float scale = (float)i / (float)count;
        scale = lerp(.1f, 1.f, scale * scale);
        vec3 v(d(e)*2.f-1.f, d(e)*2.f-1.f, d(e));
        out[i] = v.normalize() * d(e) * scale;
    }
}
void RandomKernel2D(int count, vector<vec3>& out)
{
    uniform_real_distribution<float> d(0.f, 1.f);
    default_random_engine e;
    out.resize(count);
    for (unsigned int i = 0; i < count; i++)
    {
        out[i] = vec3(d(e)*2.f-1.f, d(e)*2.f-1.f, 0.f);
    }  
}
vector<vec3> NormalOrientedHemisphere(int count)
{
    vector<vec3> out;
    NormalOrientedHemisphere(count, out);
    return out;
}
vector<vec3> RandomKernel2D(int count)
{
    vector<vec3> out;
    RandomKernel2D(count, out);
    return out;
}
vector<vec3> RandomSphere(int count)
{
    vector<vec3> out;
    out.resize(count);
    uniform_real_distribution<float> d(0.f, 1.f);
    default_random_engine e;
    for (size_t i = 0; i < count; i++)
    {
        out[i] = vec3(d(e), d(e), d(e));
    }
    return out;
}
#endif