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

    // virtual float SDF(const vec3& p) override
    // {
    //     return distance(c, p) - r;
    // }

    // virtual shared_ptr<Shape> Transf(const mat4& m) override
    // {
    //     vec4 w = m * vec4(c, 1.f);
    //     return make_shared<Sphere>(vec3(w.x, w.y, w.z), r);
    // }
};

#endif