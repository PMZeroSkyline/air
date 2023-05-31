#ifndef CAPSULE_H
#define CAPSULE_H

#include "Shape.h"
#include "LineSegment.h"

class Capsule : public Shape
{
public:
    vec3 a, b;
    float r;

    Capsule() : a(vec3(0.f, 0.f, .5f)), b(vec3(0.f, 0.f, 1.5f)), r(0.5f) {}
    Capsule(vec3 _a, vec3 _b, float _r) : a(_a), b(_b), r(_r) {}
};

#endif