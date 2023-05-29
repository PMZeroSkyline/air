#ifndef OBB_H
#define OBB_H

#include "Shape.h"

class OBB : public Shape
{
public:
    Transform worldTransform;
    vec3 c, e;

    OBB() : e(vec3(1.f)) {}
    OBB(const Transform& _worldTransform, const vec3& _c, const vec3& _e) : worldTransform(_worldTransform), c(_c), e(_e) {}

};

#endif