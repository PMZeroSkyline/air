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
vector<vec3> RandomUnitSphere(int count)
{
    vector<vec3> out;
    uniform_real_distribution<float> d(-1.f, 1.f);
    default_random_engine e;
    while (out.size() != count)
    {
        vec3 v = vec3(d(e), d(e), d(e));
        if (v.length() <= 1.f)
        {
            out.push_back(v);
        }
    }
    return out;
}
vector<vec3> Pow(const vector<vec3>& in, float power)
{
    vector<vec3> out;
    out.resize(in.size());
    for (int i = 0; i < in.size(); i++)
    {
        out[i] = Pow(in[i], power);
    }
    return out;
}
#endif