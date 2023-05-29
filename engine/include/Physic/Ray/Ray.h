#ifndef RAY_H
#define RAY_H

#include "Core/Math/Math.h"

class Ray
{
public:
    vec3 p;
    vec3 d;
    float t;

    Ray(vec3 _p, vec3 _d, float _t) : p(_p), d(_d), t(_t) {}
};

#endif