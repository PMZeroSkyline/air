#ifndef MATH_H
#define MATH_H

#include "Core.h"

struct u8vec4
{
	unsigned char x,y,z,w;
};
struct u16vec4
{
	unsigned short x,y,z,w;
};
struct uvec4
{
	unsigned int x,y,z,w;
};
struct ivec4
{
	int x,y,z,w;
	ivec4() : x(0), y(0), z(0), w(0) {};
	ivec4(const u8vec4 &v) : x(v.x), y(v.y), z(v.z), w(v.w) {};
};
struct vec2
{
	float x,y;
};
struct vec3
{
	float x,y,z;
	vec3() : x(0), y(0), z(0) {};
	vec3(float v) : x(v), y(v), z(v) {};
	float &operator[](size_t i);

};
struct vec4
{
	float x,y,z,w;
	float &operator[](size_t i);
	vec4 &operator/=(const vec4 &v);
};
struct quat
{
	float x,y,z,w;
	float &operator[](size_t i);
};
struct mat3
{
	vec3 row[3];
};
struct mat4
{
	vec4 row[4];
	vec4 &operator[](size_t i);
};


float &vec3::operator[](size_t i)
{
	return *( (float*)this + i );
}
float &vec4::operator[](size_t i)
{
	return *( (float*)this + i );
}
float &quat::operator[](size_t i)
{
	return *( (float*)this + i );
}
vec4 &mat4::operator[](size_t i)
{
	return *( (vec4*)this + i );
}
vec4 &vec4::operator/=(const vec4 &v)
{

}
#endif