#ifndef LINE_SEGMENT_H
#define LINE_SEGMENT_H

#include "Shape.h"

class LineSegment : public Shape
{
public:
    vec3 a;
    vec3 b;

    LineSegment(vec3 _a, vec3 _b) : a(_a), b(_b) {}

    // virtual float SDF(const vec3& p) override
    // {
    //     vec3 ab = b - a;
    //     vec3 ap = p - a;
    //     float apab = dot(ap, ab);
    //     if (apab <= 0.f)
    //     {
    //         return distance(a, p);
    //     }

    //     vec3 ba = a - b;
    //     vec3 bp = p - b;
    //     float babp = dot(ba, bp);
    //     if (babp <= 0.f)
    //     {
    //         return distance(b, p);
    //     }

    //     float proj = apab / ab.length();
    //     vec3 c = a + ab.normalize() * proj;
    //     return distance(c, p);
    // }
    // virtual shared_ptr<Shape> Transf(const mat4& m) override
    // {
    //     vec4 wa = m * vec4(a, 1.f);
    //     vec4 wb = m * vec4(b, 1.f);
    //     return make_shared<LineSegment>(vec3(wa.x,wa.y,wa.z), vec3(wb.x,wb.y,wb.z));
    // }
};

#endif