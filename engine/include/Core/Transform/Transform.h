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
	
	bool operator==(const Transform& rhs) const
	{
		return ((translation == rhs.translation) && (rotation == rhs.rotation) && (scaling == rhs.scaling));
	}
	bool operator!=(const Transform& rhs) const
	{
		return !(*this == rhs);
	}
	mat4 ToMatrix()
	{
		vec3 euler = rotation.ToEuler();
		mat4 Rx = mat4().rotate(vec3(1,0,0), euler.x);
		mat4 Ry = mat4().rotate(vec3(0,1,0), euler.z);
		mat4 Rz = mat4().rotate(vec3(0,0,1), euler.y);
		mat4 R = Ry * Rx * Rz;
		mat4 T = TranslationMatrix(translation);
		mat4 S = ScaleMatrix(scaling);
		return T * R * S;
		//return TranslationMatrix(translation) * mat4(rotation) * ScaleMatrix(scaling);
	}
};
ostream &operator<<(ostream &os, const Transform &t)
{
	os << "translation\t" << t.translation;
	os << "rotation\t" << degrees(t.rotation.ToEuler());
	os << "scaling\t\t" << t.scaling;
	return os;
}
#endif