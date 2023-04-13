#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Core/Math/Math.h"

class Transform
{
public:
	vec3 translation;
	quat rotation;
	vec3 scaling;

	Transform() : scaling(1.f) {};
	Transform(const mat4 &m)
	{
		vec3 Skew; vec4 Perspective;
		m.decompose(scaling, rotation, translation, Skew, Perspective);
	}
	mat4 ToMatrix()
	{
		return mat4().translate(translation) * mat4(rotation) * mat4().scale(scaling);
	}
};
ostream &operator<<(ostream &os, const Transform &t)
{
	os << "translation\t" << t.translation;
	os << "rotation\t" << degrees(t.rotation.toEuler());
	os << "scaling\t\t" << t.scaling;
	return os;
}
#endif