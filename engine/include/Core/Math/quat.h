#ifndef QUAT_H
#define QUAT_H

#include "math_fwd.h"
#include "vec3.h"

template<typename T>
struct quatt
{
    T x,y,z,w;
	quatt<T>() : x(0), y(0), z(0), w(0) {}
	quatt<T>(T i) : x(i), y(i), z(i), w(i) {}
	quatt<T>(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}
	quatt<T>(const vec3t<T> euler);
	quatt<T> operator+=(const quatt<T>& v);
	quatt<T> operator-=(const quatt<T>& v);
	quatt<T> operator*=(const quatt<T>& v);
	quatt<T> operator/=(const quatt<T>& v);
	quatt<T> operator+(const quatt<T>& v) const;
	quatt<T> operator-(const quatt<T>& v) const;
	quatt<T> operator*(const quatt<T>& v) const;
	quatt<T> operator/(const quatt<T>& v) const;
	quatt<T> operator-() const;
	bool operator==(const quatt<T>& v) const;
	bool operator!=(const quatt<T>& v) const;
	T& operator[](int i);
	T const& operator[](int i) const;
    T length() const;
    quatt<T> normalize() const;
	vec3t<T> toEuler() const;
};
template<typename T>
inline quatt<T>::quatt(const vec3t<T> euler)
{
	// https://en.wikipedia.org/wiki/Conversion_between_quats_and_Euler_angles
	T pitch = euler.x;
	T yaw = euler.y;
	T roll = euler.z;

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
	angles.z = std::atan2(sinr_cosp, cosr_cosp);
	// pitch (y-axis rotation)
	float sinp = std::sqrt(1 + 2 * (w * y - x * z));
	float cosp = std::sqrt(1 - 2 * (w * y - x * z));
	angles.x = 2 * std::atan2(sinp, cosp) - PI / 2;
	// yaw (z-axis rotation)
	float siny_cosp = 2 * (w * z + x * y);
	float cosy_cosp = 1 - 2 * (y * y + z * z);
	angles.y = std::atan2(siny_cosp, cosy_cosp);
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
	return quatt<T>(x*v.x, y*v.y, z*v.z, w*v.w);
}
template<typename T>
inline quatt<T> quatt<T>::operator/(const quatt<T>& v) const
{
	return quatt<T>(x/v.x, y/v.y, z/v.z, w/v.w);
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
	x*=v.x; y*=v.y; z*=v.z; w*=v.w;
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

using quat = quatt<float>;

#endif