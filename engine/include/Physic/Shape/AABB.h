#ifndef AABB_H
#define AABB_H

#include "Shape.h"

class AABB : public Shape
{
public:
    vec3 min;
    vec3 max;

    AABB() {};
    AABB(vec3 _min, vec3 _max) : min(_min), max(_max){}
};

#endif