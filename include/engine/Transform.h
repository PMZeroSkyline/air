#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Math.h"

class Transform
{
public:
	vec3 translation;
	quat rotation;
	vec3 scale;

	Transform() : scale(1.f) {};
	Transform(const mat4 &ModelMatrix)
	{
		
	}
};

#endif