#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape
{
public:
    vec3 c;
    float r;
    
    Sphere() : r(.5f) {};
    Sphere(vec3 _c, float _r) : c(_c), r(_r){}

};

#endif