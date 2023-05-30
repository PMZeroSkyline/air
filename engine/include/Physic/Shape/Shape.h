#ifndef SHAPE_H
#define SHAPE_H

#include "Core/Math/Math.h"
#include "Core/Transform/Transform.h"

class Shape
{
public:
    virtual ~Shape(){}

    // virtual float SDF(const vec3 &p)
    // {
    //     return 0.f;
    // }

    // virtual shared_ptr<Shape> Transf(const mat4& m)
    // {
    //     return make_shared<Shape>();
    // }
};

#endif