#ifndef CAPSULE_H
#define CAPSULE_H

#include "Shape.h"
#include "LineSegment.h"

class Capsule : public Shape
{
public:
    vec3 a, b;
    float r;

    Capsule() : a(vec3(0.f)), b(vec3(0.f, 0.f, 1.f)), r(0.5f) {}
    Capsule(vec3 _a, vec3 _b, float _r) : a(_a), b(_b), r(_r) {}

    // virtual float SDF(const vec3& p) override
    // {
    //     LineSegment l = LineSegment(a, b);
    //     float d = l.SDF(p);
    //     return d - r;
    // }

    // virtual shared_ptr<Shape> Transf(const mat4& m) override
    // {
    //     vec4 wa = m * vec4(a, 1.f);
    //     vec4 wb = m * vec4(b, 1.f);
    //     return make_shared<Capsule>(vec3(wa.x,wa.y,wa.z), vec3(wb.x,wb.y,wb.z), r);
    // }
};

#endif