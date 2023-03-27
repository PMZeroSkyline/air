#ifndef TRANSFORM
#define TRANSFORM

#include "Core.h"

class Transform
{
public:
	vec3 translation;
	quat rotation;
	vec3 scaling;
    Transform() : scaling(1.f) {}
    Transform(const mat4 &m)
    {
        vec3 skew;
        vec4 perspective;
        decompose(m, scaling, rotation, translation, skew, perspective);
    }
};

#endif