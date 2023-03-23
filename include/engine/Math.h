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
	vec4() : x(0), y(0), z(0), w(0) {};
	vec4(float v) : x(v), y(v), z(v), w(v) {};
	vec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {};
	float &operator[](size_t i);
	float &operator[](size_t i) const;
	vec4 &operator+=(const vec4 &v);
	vec4 &operator-=(const vec4 &v);
	vec4 &operator*=(const vec4 &v);
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
	mat4() {}
	mat4(const vec4 &x, const vec4 &y, const vec4 &z, const vec4 &w)
	{
		row[0] = x;
		row[1] = y;
		row[2] = z;
		row[3] = w;
	}
	vec4 &operator[](size_t i) const;
};

float dot(const vec4 &x, const vec4 &y)
{
	return x.x * y.x +
		   x.y * y.y +
		   x.z * y.z +
		   x.w * y.w;
}
float &vec3::operator[](size_t i)
{
	return *( (float*)this + i );
}
float &vec4::operator[](size_t i)
{
	return *( (float*)this + i );
}
float &vec4::operator[](size_t i) const
{
	return *( (float*)this + i );
}
float &quat::operator[](size_t i)
{
	return *( (float*)this + i );
}
vec4 &mat4::operator[](size_t i) const
{
	return *( (vec4*)this + i );
}
vec4 &vec4::operator+=(const vec4 &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return (*this);
}
vec4 &vec4::operator-=(const vec4 &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return (*this);
}
vec4 &vec4::operator*=(const vec4 &v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
	return (*this);
}
vec4 &vec4::operator/=(const vec4 &v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;
	return (*this);
}
vec4 operator+(const vec4 &x, const vec4 &y)
{
	return vec4(x.x + y.x,
			    x.y + y.y,
				x.z + y.z,
			    x.w + y.w);
}
vec4 operator-(const vec4 &x, const vec4 &y)
{
	return vec4(x.x - y.x,
			    x.y - y.y,
				x.z - y.z,
			    x.w - y.w);
}
vec4 operator*(const vec4 &x, const vec4 &y)
{
	return vec4(x.x * y.x,
			    x.y * y.y,
				x.z * y.z,
			    x.w * y.w);
}
vec4 operator/(const vec4 &x, const vec4 &y)
{
	return vec4(x.x / y.x,
				x.y / y.y,
				x.z / y.z,
				x.w / y.w);
}
mat4 operator*(const mat4 &x, float y)
{
	mat4 r;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			r[i][j] = x[i][j] * y;
		}
	}
	return r;
}
vec4 operator*(const mat4 &x, const vec4 &y)
{
	return vec4(
		dot(x[0], y),
		dot(x[1], y),
		dot(x[3], y),
		dot(x[4], y)
	);
}
mat4 operator*(const mat4 &x, const mat4 &y)
{
	mat4 r;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			r[i][j] = dot(x[i], y[j]);
		}
	}
	return r;
}
bool epsilonEqual(float x, const float y, float epsilon)
{
	return abs(x - y) < epsilon;
}
bool epsilonNotEqual(float x, const float y, float epsilon)
{
	return abs(x - y) >= epsilon;
}
float determinant(const mat4 &m)
{
	float subFactor00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
	float subFactor01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
	float subFactor02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
	float subFactor03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
	float subFactor04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
	float subFactor05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];

	vec4 detCof(
		+ (m[1][1] * subFactor00 - m[1][2] * subFactor01 + m[1][3] * subFactor02),
		- (m[1][0] * subFactor00 - m[1][2] * subFactor03 + m[1][3] * subFactor04),
		+ (m[1][0] * subFactor01 - m[1][1] * subFactor03 + m[1][3] * subFactor05),
		- (m[1][0] * subFactor02 - m[1][1] * subFactor04 + m[1][2] * subFactor05)
	);
	return m[0][0] * detCof[0] + m[0][1] * detCof[1] +
		   m[0][2] * detCof[2] + m[0][3] * detCof[3];
	return 0;
}
mat4 inverse(const mat4 &m)
{
	float Coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
	float Coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
	float Coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

	float Coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
	float Coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
	float Coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

	float Coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
	float Coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
	float Coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

	float Coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
	float Coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
	float Coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

	float Coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
	float Coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
	float Coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

	float Coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
	float Coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
	float Coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

	vec4 Fac0(Coef00, Coef00, Coef02, Coef03);
	vec4 Fac1(Coef04, Coef04, Coef06, Coef07);
	vec4 Fac2(Coef08, Coef08, Coef10, Coef11);
	vec4 Fac3(Coef12, Coef12, Coef14, Coef15);
	vec4 Fac4(Coef16, Coef16, Coef18, Coef19);
	vec4 Fac5(Coef20, Coef20, Coef22, Coef23);

	vec4 Vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
	vec4 Vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
	vec4 Vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
	vec4 Vec3(m[1][3], m[0][3], m[0][3], m[0][3]);

	vec4 Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
	vec4 Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
	vec4 Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
	vec4 Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

	vec4 SignA(+1, -1, +1, -1);
	vec4 SignB(-1, +1, -1, +1);
	mat4 Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

	vec4 Row0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

	vec4 Dot0(m[0] * Row0);
	float Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

	float OneOverDeterminant = 1 / Dot1;

	return Inverse * OneOverDeterminant;
}
mat4 transpose(const mat4 &m)
{
	mat4 r;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			r[i][j] = m[j][i];
		}
	}
	return r;
}
bool decompose(const mat4 &ModelMatrix, vec3 &Scale, quat &Orientation, vec3 &Translation, vec3 &Skew, vec4 &Perspective)
{
	mat4 LocalMatrix(ModelMatrix);
	if (epsilonEqual(LocalMatrix[3][3], 0, numeric_limits<float>::min()))
	{
		LOG("failed to decompose matrix !");
		return false;
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			LocalMatrix[i][j] /= LocalMatrix[3][3];
		}
	}
	mat4 PerspectiveMatrix(LocalMatrix);
	for (int i = 0; i < 3; i++)
	{
		PerspectiveMatrix[i][3] = 0;
		PerspectiveMatrix[3][3] = 1;
	}
	if (epsilonEqual(determinant(PerspectiveMatrix), 0, numeric_limits<float>::epsilon()))
	{
		LOG("failed to decompose matrix !");
		return false;
	}
	if(
		epsilonNotEqual(LocalMatrix[0][3], 0, numeric_limits<float>::epsilon()) ||
		epsilonNotEqual(LocalMatrix[1][3], 0, numeric_limits<float>::epsilon()) ||
		epsilonNotEqual(LocalMatrix[2][3], 0, numeric_limits<float>::epsilon()))
	{
		// rightHandSide is the right hand side of the equation.
		vec4 RightHandSide;
		RightHandSide[0] = LocalMatrix[0][3];
		RightHandSide[1] = LocalMatrix[1][3];
		RightHandSide[2] = LocalMatrix[2][3];
		RightHandSide[3] = LocalMatrix[3][3];
		mat4 InversePerspectiveMatrix = inverse(PerspectiveMatrix);//   inverse(PerspectiveMatrix, inversePerspectiveMatrix);
		mat4 TransposedInversePerspectiveMatrix = transpose(InversePerspectiveMatrix);//   transposeMatrix4(inversePerspectiveMatrix, transposedInversePerspectiveMatrix);

		Perspective = TransposedInversePerspectiveMatrix * RightHandSide;
		//  v4MulPointByMatrix(rightHandSide, transposedInversePerspectiveMatrix, perspectivePoint);

		// Clear the perspective partition
		LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = 0;
		LocalMatrix[3][3] = 1;
	}
	else
	{
		// No perspective.
		Perspective = vec4(0, 0, 0, 1);
	}
	return false;
}
#endif