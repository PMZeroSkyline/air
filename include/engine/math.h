#ifndef MATH_H
#define MATH_H
#include <iostream>
#include <cmath>
#include <initializer_list>
using std::ostream;
using std::endl;

inline float lerp(float a, float b, float w)
{
	return a + (b - a) * w;
}
float radians(float degrees)
{
	return ((degrees * M_PI ) / 180.);
}
float degrees(float radians)
{
	return (( radians * 180. ) / M_PI);
}
float mod(float x, float y)
{
	float o = fmod(x, y);
	o = o < 0 ? o + y : o;
	return o;
}
int sgn(float val) 
{
    return (0 < val) - (val < 0);
}

struct vec2
{
	float x,y;
	vec2() : x(0), y(0){}
    vec2(float v) : x(v), y(v){}
	vec2(float _x, float _y) : x(_x), y(_y){}
	vec2 &operator=(const vec2 &v)
	{
		memcpy(this, &v, sizeof(float)*2);
		return *this;
	}
	float &operator[](size_t i)
	{
		return *(((float*)this)+i);
	}
	const float &operator[](size_t i) const
	{
		return *(((float*)this)+i);
	}
	friend ostream &operator<<(ostream &os, const vec2 v)
	{
		for (size_t i = 0; i < 2; i++)
			os << v[i] << ",";
		return os;
	}
	const vec2 operator-() const
	{
		return vec2(-x, -y);
	}
    vec2 &operator+=(const vec2 &v)
    {
		for (int i = 0; i < 2; i++)
			(*this)[i] += v[i];
        return *this;
    }
	vec2 &operator-=(const vec2 &v)
    {
		for (int i = 0; i < 2; i++)
			(*this)[i] -= v[i];
        return *this;
    }
	vec2 &operator*=(const vec2 &v)
    {
		for (int i = 0; i < 2; i++)
			(*this)[i] *= v[i];
        return *this;
    }
	vec2 &operator/=(const vec2 &v)
    {
		for (int i = 0; i < 2; i++)
			(*this)[i] /= v[i];
        return *this;
    }
	float length() const
	{
		return sqrt(x*x+y*y);
	}
	inline friend vec2 operator+(const vec2 &u, const vec2 &v)
	{
		return vec2(u.x + v.x, u.y + v.y);
	}
	inline friend vec2 operator-(const vec2 &u, const vec2 &v)
	{
		return vec2(u.x - v.x, u.y - v.y);
	}
	inline friend vec2 operator*(const vec2 &u, const vec2 &v)
	{
		return vec2(u.x * v.x, u.y * v.y);
	}
	inline friend vec2 operator/(const vec2 &u, const vec2 &v)
	{
		return vec2(u.x / v.x, u.y / v.y);
	}
	vec2 normalize() const
	{
		return (*this) / length();
	}
};
float dot(const vec2 &u, const vec2 &v)
{
	return (u.x*v.x + u.y*v.y);
}

struct vec4;
struct vec3
{
	float x,y,z;
	vec3() : x(0), y(0), z(0){}
	vec3(float v) : x(v), y(v), z(v) {}
	vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z){}
	vec3(const vec4 &v);
	vec3 &operator=(const vec3 &v)
	{
		memcpy(this, &v, sizeof(float)*3);
		return *this;
	}
	float &operator[](size_t t)
	{
		return *((float*)this+t);
	}
	const float &operator[](size_t t) const
	{
		return *((float*)this+t);
	}
	const vec3 operator-() const
	{
        return vec3(-x, -y, -z);
	}
    vec3 &operator+=(const vec3 &v)
    {
		for (int i = 0; i < 3; i++)
			(*this)[i] += v[i];
        return *this;
    }
	vec3 &operator-=(const vec3 &v)
    {
		for (int i = 0; i < 3; i++)
			(*this)[i] -= v[i];
        return *this;
    }
	vec3 &operator*=(const vec3 &v)
    {
		for (int i = 0; i < 3; i++)
			(*this)[i] *= v[i];
        return *this;
    }
	vec3 &operator/=(const vec3 &v)
    {
		for (int i = 0; i < 3; i++)
			(*this)[i] /= v[i];
        return *this;
    }
	vec3 operator%(float v) const
    {
		vec3 r;
		r.x = mod(x, v);
		r.y = mod(y, v);
		r.z = mod(z, v);
        return r;
    }
	friend ostream &operator<<(ostream &os, const vec3 v)
	{
		for (size_t i = 0; i < 3; i++)
			os << v[i] << ",";
		return os;
	}
	inline friend vec3 operator+(const vec3 &u, const vec3 &v)
	{
		return vec3(u.x + v.x, u.y + v.y, u.z + v.z);
	}
	inline friend vec3 operator-(const vec3 &u, const vec3 &v)
	{
		return vec3(u.x - v.x, u.y - v.y, u.z - v.z);
	}
	inline friend vec3 operator*(const vec3 &u, const vec3 &v)
	{
		return vec3(u.x * v.x, u.y * v.y, u.z * v.z);
	}
	inline friend vec3 operator/(const vec3 &u, const vec3 &v)
	{
		return vec3(u.x / v.x, u.y / v.y, u.z / v.z);
	}
	vec3 normalize() const
	{
		return (*this) / length();
	}
	float length() const
	{
		return sqrt(x*x+y*y+z*z);
	}
};
float dot(const vec3 &u, const vec3 &v)
{
	return (u.x*v.x + u.y*v.y + u.z*v.z);
}
vec3 cross(const vec3 &u, const vec3 &v)
{
	return vec3(
		u.y*v.z-u.z*v.y,
		u.z*v.x-u.x*v.z,
		u.x*v.y-u.x*v.x
	);
}
vec3 lerp(const vec3 &a, const vec3 &b, float w)
{
	return vec3(lerp(a.x, b.x, w), lerp(a.x, b.x, w), lerp(a.x, b.x, w));
}
vec3 radians(const vec3 degrees)
{
	return vec3(radians(degrees.x), radians(degrees.y), radians(degrees.z));
}
vec3 degrees(const vec3 &radians)
{
	return vec3(degrees(radians.x), degrees(radians.y), degrees(radians.z));
}

struct vec4
{
	float x,y,z,w;
	vec4() : x(0), y(0), z(0), w(0){}
    vec4(float v) : x(v), y(v), z(v), w(v){}
	vec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w){}
	vec4 &operator=(const vec4 &v)
	{
		memcpy(this, &v, sizeof(float)*4);
		return *this;
	}
	float &operator[](size_t i)
	{
		return *(((float*)this)+i);
	}
	const float &operator[](size_t i) const
	{
		return *(((float*)this)+i);
	}
	const vec4 operator-() const
	{
        return vec4(-x, -y, -z, -w);
	}
    vec4 &operator+=(const vec4 &v)
    {
		for (int i = 0; i < 4; i++)
			(*this)[i] += v[i];
        return *this;
    }
	vec4 &operator-=(const vec4 &v)
    {
		for (int i = 0; i < 4; i++)
			(*this)[i] -= v[i];
        return *this;
    }
	vec4 &operator*=(const vec4 &v)
    {
		for (int i = 0; i < 4; i++)
			(*this)[i] *= v[i];
        return *this;
    }
	vec4 &operator/=(const vec4 &v)
    {
		for (int i = 0; i < 4; i++)
			(*this)[i] /= v[i];
        return *this;
    }
 	friend ostream &operator<<(ostream &os, const vec4 v)
	{
		for (size_t i = 0; i < 4; i++)
			os << v[i] << ",";
		return os;
	}
	inline friend vec4 operator+(const vec4 &u, const vec4 &v)
	{
		return vec4(u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w);
	}
	inline friend vec4 operator-(const vec4 &u, const vec4 &v)
	{
		return vec4(u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w);
	}
	inline friend vec4 operator*(const vec4 &u, const vec4 &v)
	{
		return vec4(u.x * v.x, u.y * v.y, u.z * v.z, u.w * v.w);
	}
	inline friend vec4 operator/(const vec4 &u, const vec4 &v)
	{
		return vec4(u.x / v.x, u.y / v.y, u.z / v.z, u.w / v.w);
	}
	vec4 normalize() const
	{
		return (*this) / length();
	}
	float length() const
	{
		return sqrt(x*x+y*y+z*z+w*w);
	}
};
vec3::vec3(const vec4 &v)
{
	memcpy(this, &v, sizeof(float)*3);
}
float dot(const vec4 &u, const vec4 &v)
{
	return (u.x*v.x + u.y*v.y + u.z*v.z + u.w*v.w);
}
struct mat3;
struct quat
{
	float x,y,z,w;
	quat() : x(0),y(0),z(0),w(0){}
	quat(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w){}
	quat(const vec3 &axis, float angle)
	{
		x = axis.x * sin(angle / 2);
		y = axis.y * sin(angle / 2);
		z = axis.z * sin(angle / 2);
		w = cos(angle / 2);
	}
	quat(const mat3 &m);
	quat(float roll, float pitch, float yaw) // roll (x), pitch (Y), yaw (z)
	{
		// https://en.wikipedia.org/wiki/Conversion_between_quats_and_Euler_angles

		float cr = cos(roll * 0.5);
		float sr = sin(roll * 0.5);
		float cp = cos(pitch * 0.5);
		float sp = sin(pitch * 0.5);
		float cy = cos(yaw * 0.5);
		float sy = sin(yaw * 0.5);

		w = cr * cp * cy + sr * sp * sy;
		x = sr * cp * cy - cr * sp * sy;
		y = cr * sp * cy + sr * cp * sy;
		z = cr * cp * sy - sr * sp * cy;
	}
	quat(const vec3 &euler)
	{
		(*this) = quat(euler.x, euler.y, euler.z);
	}
	quat &operator=(const quat &q)
	{
		memcpy(this, &q, sizeof(float)*4);
		return *this;
	}
	float &operator[](size_t i)
	{
		return *((float*)this+i);
	}
	const float &operator[](size_t i) const
	{
		return *((float*)this+i);
	}
	friend quat operator*(const quat &q1, const quat &q2)
	{
		quat q;
		q.x =  q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x;
		q.y = -q1.x * q2.z + q1.y * q2.w + q1.z * q2.x + q1.w * q2.y;
		q.z =  q1.x * q2.y - q1.y * q2.x + q1.z * q2.w + q1.w * q2.z;
		q.w = -q1.x * q2.x - q1.y * q2.y - q1.z * q2.z + q1.w * q2.w;
		return q;
	}
	friend quat operator/(const quat &q, float v)
	{
		return quat(q.x/v, q.y/v, q.z/v, q.w/v);
	}
	friend ostream &operator<<(ostream &os, const quat q)
	{
		for (size_t i = 0; i < 4; i++)
			os << q[i] << ",";
		return os;
	}
	vec3 euler() const
	{
		// https://en.wikipedia.org/wiki/Conversion_between_quats_and_Euler_angles
		vec3 angles;
		// roll (x-axis rotation)
		float sinr_cosp = 2 * (w * x + y * z);
		float cosr_cosp = 1 - 2 * (x * x + y * y);
		angles.x = std::atan2(sinr_cosp, cosr_cosp);
		// pitch (y-axis rotation)
		float sinp = std::sqrt(1 + 2 * (w * y - x * z));
		float cosp = std::sqrt(1 - 2 * (w * y - x * z));
		angles.y = 2 * std::atan2(sinp, cosp) - M_PI / 2;
		// yaw (z-axis rotation)
		float siny_cosp = 2 * (w * z + x * y);
		float cosy_cosp = 1 - 2 * (y * y + z * z);
		angles.z = std::atan2(siny_cosp, cosy_cosp);
		return angles;
	}
};
float dot(const quat &p, const quat &q)
{
	return (p.x*q.x + p.y*q.y + p.z*q.z);
}
quat slerp(const quat &qa, const quat &qb, float t) 
{
	//https://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quats/slerp/index.htm
	quat qm;
	float cosHalfTheta = qa.w * qb.w + qa.x * qb.x + qa.y * qb.y + qa.z * qb.z;
	if (abs(cosHalfTheta) >= 1.0)
	{
		qm.w = qa.w;qm.x = qa.x;qm.y = qa.y;qm.z = qa.z;
		return qm;
	}
	float halfTheta = acos(cosHalfTheta);
	float sinHalfTheta = sqrt(1.0 - cosHalfTheta*cosHalfTheta);
	if (fabs(sinHalfTheta) < 0.001)
	{ 
		qm.w = (qa.w * 0.5 + qb.w * 0.5);
		qm.x = (qa.x * 0.5 + qb.x * 0.5);
		qm.y = (qa.y * 0.5 + qb.y * 0.5);
		qm.z = (qa.z * 0.5 + qb.z * 0.5);
		return qm;
	}
	float ratioA = sin((1 - t) * halfTheta) / sinHalfTheta;
	float ratioB = sin(t * halfTheta) / sinHalfTheta; 
	
	qm.w = (qa.w * ratioA + qb.w * ratioB);
	qm.x = (qa.x * ratioA + qb.x * ratioB);
	qm.y = (qa.y * ratioA + qb.y * ratioB);
	qm.z = (qa.z * ratioA + qb.z * ratioB);
	return qm;
}

struct mat4;
struct mat3
{
	vec3 row[3];
	mat3() 
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				row[i][j] = (i == j) ? 1. : 0.;
	};
	mat3(std::initializer_list<vec3> l)
	{
		for (int i = 0; i < 3; i++)
			row[i] = *(l.begin()+i);
    }
	mat3(std::initializer_list<float> l)
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				row[i][j] = *(l.begin()+i*3 + j);
    }
	mat3(const quat &q)
	{
		// 两种方法任选一种即可

		// 方法1
		// https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation#Comparison_with_other_representations_of_rotations
		// float a = q.w, b = q.x, c = q.y, d = q.z;
		// (*this) = {
		// 	{a*a+b*b-c*c-d*d, 2*b*c-2*a*d, 2*b*d+2*a*c},
		// 	{2*b*c+2*a*d, a*a-b*b+c*c-d*d, 2*c*d-2*a*b},
		// 	{2*b*d-2*a*c, 2*c*d+2*a*b, a*a-b*b-c*c+d*d}
		// };

		// 方法2
		//https://en.wikipedia.org/wiki/Rotation_matrix#Quaternion
		(*this) = {
			{1-2*q.y*q.y-2*q.z*q.z, 2*q.x*q.y-2*q.z*q.w, 2*q.x*q.z+2*q.y*q.w},
			{2*q.x*q.y+2*q.z*q.w, 1-q.x*q.x-2*q.z*q.z, 2*q.y*q.z-2*q.x*q.w},
			{2*q.x*q.z-2*q.y*q.w, q.y*q.z+2*q.x*q.w, 1-2*q.x*q.x-2*q.y*q.y}
		};		
	}
	mat3(const mat4 &m);
	vec3 &operator[](size_t i)
	{
		return row[i];
	}
	const vec3 &operator[](size_t i) const
	{
		return row[i];
	}
	friend ostream &operator<<(ostream &os, const mat3 &m)
	{
		for (int i = 0; i < 3; i++)
			os << m.row[i] << endl;
		return os;
	}
	friend mat3 operator*(const mat3 &u, const mat3 &v)
	{
		mat3 m;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				m[i][j] = dot(u[i], v.column(j));
		return m;
	}
	vec3 column(int i) const
	{
		return vec3((*this)[0][i], (*this)[1][i], (*this)[2][i]);
	}
	vec3 get_scale() const
	{
		return vec3(
			(*this)[0].length(),
			(*this)[1].length(),
			(*this)[2].length()
		);
	}
	float trace() const
	{
		return ((*this)[0][0] + (*this)[1][1] + (*this)[2][2]);
	}
};
quat::quat(const mat3 &m) 
{
	// https://en.wikipedia.org/wiki/Rotation_matrix#Conversions
	// 1) not work
	// float t =  m[0][0] + m[1][1] + m[2][2];
	// float r = sqrt(1 + t);
	// w = 0.5 * r;
	// x = (sgn(m[2][1] - m[1][2])) * abs(0.5 * sqrt(1 + m[0][0] - m[1][1] - m[2][2]));
	// y = (sgn(m[0][2] - m[2][0])) * abs(0.5 * sqrt(1 - m[0][0] + m[1][1] - m[2][2]));
	// y = (sgn(m[1][0] - m[0][1])) * abs(0.5 * sqrt(1 - m[0][0] - m[1][1] + m[2][2]));
	// 2) not work
	// float t = m[0][0] + m[1][1] + m[2][2];
	// float r = sqrt(1+t);
	// float s = 1/(2*r);
	// w = 0.5*r;
	// x = (m[2][1] - m[1][2]) * s;
	// y = (m[0][2] - m[2][0]) * s;
	// x = (m[1][0] - m[0][1]) * s;
	// 3) work, but low quality
	// float root, trace = m[0][0] + m[1][1] + m[2][2];
	// root = sqrt(trace + 1);
	// w = 0.5 * root;
	// root = 0.5 / root;
	// x = root * (m[1][2] - m[2][1]);
	// y = root * (m[2][0] - m[0][2]);
	// z = root * (m[0][1] - m[1][0]);
	// 4) work, but low quality
	// https://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
	// w = sqrt(1.0 + m[0][0] + m[1][1] + m[2][2]) / 2.0;
	// float w4 = (4.0 * w);
	// x = (m[2][1] - m[1][2]) / w4 ;
	// y = (m[0][2] - m[2][0]) / w4 ;
	// z = (m[1][0] - m[0][1]) / w4 ;
	// 5) find in glm
	int i, j, k = 0;
	float root, trace = m[0][0] + m[1][1] + m[2][2];
	if (trace > 0)
	{
		root = sqrt(trace + 1);
		w = 0.5 * root;
		root = 0.5 / root;
		x = root * (m[1][2] - m[2][1]);
		y = root * (m[2][0] - m[0][2]);
		z = root * (m[0][1] - m[1][0]);
	}
	else
	{
		int next[3] = {1,2,0};
		i = 0;
		if (m[1][1] > m[0][0]) 
			i = 1;
		if (m[2][0] > m[i][i])
			i = 2;
		j = next[i];
		k = next[j];
		
		root = sqrt(m[i][i] - m[j][j] - m[k][k] + 1);
		
		(*this)[i] = 0.5 * root;
		root = 0.5 / root;
		(*this)[j] = root * (m[i][j] + m[j][i]);
		(*this)[k] = root * (m[i][k] + m[k][i]);
		w = root * (m[j][k] - m[k][j]);
	}
}

struct mat4
{
	vec4 row[4];
 	mat4(std::initializer_list<vec4> l)
	{
		for (int i = 0; i < 4; i++)
			row[i] = *(l.begin()+i);
    }
	mat4(std::initializer_list<float> l)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				row[i][j] = *(l.begin()+i*4 + j);
    }
	mat4() 
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				(*this)[i][j] = (i == j) ? 1. : 0.;
	};
	mat4(const mat3 &m)
	{
		for (int i = 0; i < 3; i++)
		{
			memcpy(&(*this)[i], &m[i], sizeof(float)*3);
			(*this)[i][3] = 0;
			(*this)[3][i] = 0;
		}
		(*this)[3][3] = 1;
	}
	vec4 &operator[](size_t i)
	{
		return *((vec4*)this+i);
	}
	const vec4 &operator[](size_t i) const
	{
		return *((vec4*)this+i);
	}
	friend ostream &operator<<(ostream &os, const mat4 &m)
	{
		for (int i = 0; i < 4; i++)
			os << m[i] << endl;
		return os;
	}
	friend mat4 operator*(const mat4 &u, const mat4 v)
	{
		mat4 m;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				m[i][j] = dot(u[i], v.column(j));
		return m;
	}
	friend vec4 operator*(const mat4 &u, const vec4 &v)
	{
		vec4 r;
		for (int i = 0; i < 4; i++)
			r[i] = dot(u[i], v);
		return r;
	}
	mat4 T() const
	{
		mat4 t;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				t[i][j] = (*this)[j][i];
		return t;		
	}
	vec4 column(int i) const
	{
		return vec4((*this)[0][i], (*this)[1][i], (*this)[2][i], (*this)[3][i]);
	}
	vec3 get_translate()
	{
		return vec3((*this)[0][3], (*this)[1][3], (*this)[2][3]);
	}
	mat4 normalize() const
	{
		mat4 r = (*this);
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				r[i][j] /= r[3][3];
		return r;
	}
};
mat3::mat3(const mat4 &m)
{
	for (int i = 0; i < 3; i++)
		(*this)[i] = m[i];
}
mat4 T(const vec3 p)
{
	mat4 m = {
		{1	,0	,0	,p.x},
		{0	,1	,0	,p.y},
		{0	,0	,1	,p.z},
		{0	,0	,0	,1	}
	};
	return m;
}
mat4 RX(float rad)
{
	float c = cos(rad);
	float s = sin(rad);
	mat4 m = {
		{1	,0	,0	,0},
		{0	,c	,-s	,0},
		{0	,s	,c	,0},
		{0	,0	,0	,1}
	};
	return m;
}
mat4 RY(float rad)
{
	float c = cos(rad);
	float s = sin(rad);
	mat4 m = {
		{c	,0	,s	,0},
		{0	,1	,0	,0},
		{-s	,0	,c	,0},
		{0	,0	,0	,1}
	};
	return m;
}
mat4 RZ(float rad)
{
	float c = cos(rad);
	float s = sin(rad);
	mat4 m = {
		{c	,-s	,0	,0},
		{s	,c	,0	,0},
		{0	,0	,1	,0},
		{0	,0	,0	,1}
	};
	return m;
}
mat4 S(const vec3 &s)
{
	mat4 m = {
		{s.x,0	,0	,0},
		{0	,s.y,0	,0},
		{0	,0	,s.z,0},
		{0	,0	,0	,1}
	};
	return m;
}

struct transform
{
	vec3 p;
	quat q;
	vec3 s;
	transform() = default;
	transform(const mat4 mtx)
	{
		mat4 m = mtx.normalize();
		p = m.get_translate();
		mat3 r(m);
		s = r.get_scale();
		for (int i = 0; i < 3; i++)
			r[i] /= s[i];
		vec3 z = cross(r[0], r[1]);
		if (dot(r[2], z) < 0)
		{
			s *= -1;
			for (int i = 0; i < 3; i++)
				r[i] *= -1;
		}
		q = quat(quat(r).euler()*-1);
	}
	friend ostream &operator<<(ostream &os, const transform &t)
	{
		os << "p\t" << t.p << endl;
		os << "q\t" << degrees(t.q.euler()) << endl;
		os << "s\t" << t.s << endl;
		return os;
	}
};
#endif