#ifndef OBB_H
#define OBB_H

#include "Shape.h"

class OBB : public Shape
{
public:
    Transform t;

    OBB() {}
    OBB(const Transform& _t) : t(_t) {}

    // virtual float SDF(const vec3 &p) override
    // {
    //     vec4 l = mat4(t.rotation).inverse() * vec4((t.translation - p), 1.f);
	//     vec3 lp = vec3(l.x, l.y, l.z);

    //     vec3 d = abs(lp) - t.scaling;

	//     float inside = min(max(d.x, max(d.y, d.z)), 0.f);
	//     float outside = max(d, 0.f).length();

    //     return inside + outside;
    // }
    // virtual shared_ptr<Shape> Transf(const mat4& m) override
    // {
    //     mat4 w = m * t.ToMatrix();
    //     return make_shared<OBB>(w);
    // }
};

#endif