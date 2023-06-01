#ifndef PLANE_H
#define PLANE_H

#include "Shape.h"

class Plane : public Shape
{
public:
    vec3 p;
    vec3 n;

    Plane(vec3 _p, vec3 _n) : p(_p), n(_n) {}
};

#endif