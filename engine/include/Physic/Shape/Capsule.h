#ifndef CAPSULE_H
#define CAPSULE_H

#include "Shape.h"

class Capsule : public Shape
{
public:
    vec3 a, b;
    float r;

    Capsule() : r(0.f) {}
    Capsule(vec3 _a, vec3 _b, float _r) : a(_a), b(_b), r(_r) {}

};

#endif