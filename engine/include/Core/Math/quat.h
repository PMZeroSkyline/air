#ifndef QUAT_H
#define QUAT_H

#include "math_fwd.h"
#include "vec3.h"

template<typename T>
struct quatt
{
    T x,y,z,w;
	quatt<T>() : x(0), y(0), z(0), w(1) {}
	quatt<T>(T i) : x(i), y(i), z(i), w(i) {}
	quatt<T>(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}
	//quatt<T>(const vec3t<T>& axis, T angle);
	quatt<T>(const vec3t<T>& euler);
	quatt<T> operator+=(const quatt<T>& v);
	quatt<T> operator-=(const quatt<T>& v);
	quatt<T> operator*=(const quatt<T>& v);
	quatt<T> operator/=(const quatt<T>& v);
	quatt<T> operator+(const quatt<T>& v) const;
	quatt<T> operator-(const quatt<T>& v) const;
	quatt<T> operator*(const quatt<T>& v) const;
	quatt<T> operator*(T v) const;
	quatt<T> operator/(const quatt<T>& v) const;
	quatt<T> operator/(T v) const;
	quatt<T> operator-() const;
	bool operator==(const quatt<T>& v) const;
	bool operator!=(const quatt<T>& v) const;
	T& operator[](int i);
	T const& operator[](int i) const;
    T length() const;
    quatt<T> normalize() const;
	vec3t<T> toEuler() const;
};
// template<typename T>
// quatt<T>::quatt(const vec3t<T>& axis, T angle)
// {
// 	float halfAngle = angle * static_cast<T>(0.5);
// 	x = axis.x * sin(halfAngle);
// 	y = axis.y * sin(halfAngle);
// 	z = axis.z * sin(halfAngle);
// 	w = cos(halfAngle);
// }
template<typename T>
quatt<T>::quatt(const vec3t<T>& euler)
{
	// https://en.wikipedia.org/wiki/Conversion_between_quats_and_Euler_angles
	T pitch = euler.y;
	T yaw = euler.z;
	T roll = euler.x;

    T cr = cos(roll * 0.5);
    T sr = sin(roll * 0.5);
    T cp = cos(pitch * 0.5);
    T sp = sin(pitch * 0.5);
    T cy = cos(yaw * 0.5);
    T sy = sin(yaw * 0.5);

    w = cr * cp * cy + sr * sp * sy;
    x = sr * cp * cy - cr * sp * sy;
    y = cr * sp * cy + sr * cp * sy;
    z = cr * cp * sy - sr * sp * cy;
}
template<typename T>
vec3t<T> quatt<T>::toEuler() const
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
	angles.y = 2 * std::atan2(sinp, cosp) - PI / 2;
	// yaw (z-axis rotation)
	float siny_cosp = 2 * (w * z + x * y);
	float cosy_cosp = 1 - 2 * (y * y + z * z);
	angles.z = std::atan2(siny_cosp, cosy_cosp);
	return angles;
}
template<typename T>
inline quatt<T> quatt<T>::operator+(const quatt<T>& v) const
{
	return quatt<T>(x+v.x, y+v.y, z+v.z, w+v.w);
}
template<typename T>
inline quatt<T> quatt<T>::operator-(const quatt<T>& v) const
{
	return quatt<T>(x-v.x, y-v.y, z-v.z, w-v.w);
}
template<typename T>
inline quatt<T> quatt<T>::operator*(const quatt<T>& v) const
{
	const quatt<T> p(*this);
	const quatt<T> q(v);
	quatt<T> r;
	r.w = p.w * q.w - p.x * q.x - p.y * q.y - p.z * q.z;
	r.x = p.w * q.x + p.x * q.w + p.y * q.z - p.z * q.y;
	r.y = p.w * q.y + p.y * q.w + p.z * q.x - p.x * q.z;
	r.z = p.w * q.z + p.z * q.w + p.x * q.y - p.y * q.x;
	return r;
}
template<typename T>
inline quatt<T> quatt<T>::operator*(T v) const
{
	return quatt<T>(x*v, y*v, z*v, w*v);
}
template<typename T>
inline quatt<T> quatt<T>::operator/(const quatt<T>& v) const
{
	return quatt<T>(x/v.x, y/v.y, z/v.z, w/v.w);
}
template<typename T>
inline quatt<T> quatt<T>::operator/(T v) const
{
	return quatt<T>(x/v, y/v, z/v, w/v);
}	
template<typename T>
inline quatt<T> quatt<T>::operator+=(const quatt<T>& v)
{
	x+=v.x; y+=v.y; z+=v.z; w+=v.w;
	return *this;
}
template<typename T>
inline quatt<T> quatt<T>::operator-=(const quatt<T>& v)
{
	x-=v.x; y-=v.y; z-=v.z; w-=v.w;
	return *this;
}
template<typename T>
inline quatt<T> quatt<T>::operator*=(const quatt<T>& v)
{
	const quatt<T> p(*this);
	const quatt<T> q(v);
	w = p.w * q.w - p.x * q.x - p.y * q.y - p.z * q.z;
	x = p.w * q.x + p.x * q.w + p.y * q.z - p.z * q.y;
	y = p.w * q.y + p.y * q.w + p.z * q.x - p.x * q.z;
	z = p.w * q.z + p.z * q.w + p.x * q.y - p.y * q.x;
	return *this;
}
template<typename T>
inline quatt<T> quatt<T>::operator/=(const quatt<T>& v)
{
	x/=v.x; y/=v.y; z/=v.z; w/=v.w;
	return *this;
}
template<typename T>
inline quatt<T> quatt<T>::operator-() const
{
	return quatt<T>(-x,-y,-z,-w);
}
template<typename T>
inline bool quatt<T>::operator==(const quatt<T>& v) const
{
	return ((x==v.x) && (y==v.y) && (z==v.z) && (w==v.w));
}
template<typename T>
inline bool quatt<T>::operator!=(const quatt<T>& v) const
{
	return ((x!=v.x) && (y!=v.y) && (z!=v.z) && (w!=v.w));
}
template<typename T>
inline T& quatt<T>::operator[](int i)
{
    return *((T*)this+i);
}
template<typename T>
inline T const& quatt<T>::operator[](int i) const
{
	return *((T*)this+i);
}
template<typename T>
inline T quatt<T>::length() const
{
    return sqrt(x*x+y*y+z*z+w*w);
}
template<typename T>
inline quatt<T> quatt<T>::normalize() const
{
    return (*this)/length();
}
template<typename T>
inline ostream& operator<<(ostream& os, const quatt<T> &v)
{
	os << v.x << " " << v.y << " " << v.z << " " << v.w << endl;
	return os;
}
template<typename T>
inline T dot(const quatt<T>& a, const quatt<T>& b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}
template<typename T>
quatt<T> slerp(const quatt<T>& x, const quatt<T>& y, T a)
{
	quatt<T> z = y;
	T cosTheta = dot(x, y);
	// If cosTheta < 0, the interpolation will take the long way around the sphere.
	// To fix this, one quat must be negated.
	if(cosTheta < static_cast<T>(0))
	{
		z = -y;
		cosTheta = -cosTheta;
	}
	// Perform a linear interpolation when cosTheta is close to 1 to avoid side effect of sin(angle) becoming a zero denominator
	if(cosTheta > static_cast<T>(1) - epsilon<T>())
	{
		// Linear interpolation
		return quatt<T>(
			lerp(x.x, z.x, a),
			lerp(x.y, z.y, a),
			lerp(x.z, z.z, a),
			lerp(x.w, z.w, a));
	}
	else
	{
		// Essential Mathematics, page 467
		T angle = acos(cosTheta);
		return (x * sin((static_cast<T>(1) - a) * angle) + z * sin(a * angle)) / sin(angle);
	}
}

using quat = quatt<float>;

#endif