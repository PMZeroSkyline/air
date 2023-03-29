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
    mat4 ToMatrix()
    {
        mat4 T = translate(mat4(1.f), translation);
        mat4 R = toMat4(rotation);
        mat4 S = scale(mat4(1.f), scaling);
        return T * R * S;
    }
};

#endif