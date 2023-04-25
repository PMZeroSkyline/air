#ifndef VEC3_H
#define VEC3_H

#include "math_fwd.h"

template<typename T>
struct vec3t
{
    T x,y,z;
	vec3t<T>() : x(0), y(0), z(0){}
	vec3t<T>(T i) : x(i), y(i), z(i){}
	vec3t<T>(T _x, T _y, T _z) : x(_x), y(_y), z(_z){}
	vec3t<T>(const vec3t<unsigned char> &v) : x(v.x), y(v.y), z(v.z) {}
	vec3t<T>(const vec3t<unsigned short> &v) : x(v.x), y(v.y), z(v.z) {}
	vec3t<T>(const vec3t<unsigned int> &v) : x(v.x), y(v.y), z(v.z) {}
	vec3t<T>(const vec3t<int> &v) : x(v.x), y(v.y), z(v.z) {}
	vec3t<T>(const vec3t<float> &v) : x(v.x), y(v.y), z(v.z) {}
	vec3t<T>(const vec3t<double> &v) : x(v.x), y(v.y), z(v.z) {}
	vec3t<T> operator+=(const vec3t<T>& v);
	vec3t<T> operator-=(const vec3t<T>& v);
	vec3t<T> operator*=(const vec3t<T>& v);
	vec3t<T> operator/=(const vec3t<T>& v);
	vec3t<T> operator%=(T v);
	vec3t<T> operator+(const vec3t<T>& v) const;
	vec3t<T> operator-(const vec3t<T>& v) const;
	vec3t<T> operator*(const vec3t<T>& v) const;
	vec3t<T> operator*(T v) const;
	vec3t<T> operator/(const vec3t<T>& v) const;
	vec3t<T> operator/(T) const;
	vec3t<T> operator-() const;
	vec3t<T> operator%(T v) const;
	bool operator==(const vec3t<T>& v) const;
	bool operator!=(const vec3t<T>& v) const;
	T& operator[](int i);
	T const& operator[](int i) const;
    T length() const;
    vec3t<T> normalize() const;
};
template<typename T>
inline vec3t<T> vec3t<T>::operator+(const vec3t<T>& v) const
{
	return vec3t<T>(x+v.x, y+v.y, z+v.z);
}
template<typename T>
inline vec3t<T> vec3t<T>::operator-(const vec3t<T>& v) const
{
	return vec3t<T>(x-v.x, y-v.y, z-v.z);
}
template<typename T>
inline vec3t<T> vec3t<T>::operator*(const vec3t<T>& v) const
{
	return vec3t<T>(x*v.x, y*v.y, z*v.z);
}
template<typename T>
inline vec3t<T> vec3t<T>::operator*(T v) const
{
	return vec3t<T>(x*v, y*v, z*v);
}
template<typename T>
inline vec3t<T> vec3t<T>::operator/(const vec3t<T>& v) const
{
	return vec3t<T>(x/v.x, y/v.y, z/v.z);
}
template<typename T>
inline vec3t<T> vec3t<T>::operator/(T v) const
{
	return vec3t<T>(x/v, y/v, z/v);
}
template<typename T>
inline vec3t<T> vec3t<T>::operator+=(const vec3t<T>& v)
{
	x+=v.x; y+=v.y; z+=v.z;
	return *this;
}
template<typename T>
inline vec3t<T> vec3t<T>::operator-=(const vec3t<T>& v)
{
	x-=v.x; y-=v.y; z-=v.z;
	return *this;
}
template<typename T>
inline vec3t<T> vec3t<T>::operator*=(const vec3t<T>& v)
{
	x*=v.x; y*=v.y; z*=v.z;
	return *this;
}
template<typename T>
inline vec3t<T> vec3t<T>::operator/=(const vec3t<T>& v)
{
	x/=v.x; y/=v.y; z/=v.z;
	return *this;
}
template<typename T>
inline vec3t<T> vec3t<T>::operator%=(T v)
{
	x = mod(x, v);
	y = mod(y, v);
	z = mod(z, v);
	return *this;
}
template<typename T>
inline vec3t<T> vec3t<T>::operator-() const
{
	return vec3t<T>(-x,-y,-z);
}
template<typename T>
inline vec3t<T> vec3t<T>::operator%(T v) const
{
	return vec3t<T>(mod(x, v), mod(y, v), mod(z, v));
}
template<typename T>
inline bool vec3t<T>::operator==(const vec3t<T>& v) const
{
	return ((x==v.x) && (y==v.y) && (z==v.z));
}
template<typename T>
inline bool vec3t<T>::operator!=(const vec3t<T>& v) const
{
	return ((x!=v.x) && (y!=v.y) && (z!=v.z));
}
template<typename T>
inline T& vec3t<T>::operator[](int i)
{
    return *((T*)this+i);
}
template<typename T>
inline T const& vec3t<T>::operator[](int i) const
{
	return *((T*)this+i);
}
template<typename T>
inline T vec3t<T>::length() const
{
    return sqrt(x*x+y*y+z*z);
}
template<typename T>
inline vec3t<T> vec3t<T>::normalize() const
{
    return (*this)/length();
}
template<typename T>
inline ostream& operator<<(ostream& os, const vec3t<T> &v)
{
	os << v.x << " " << v.y << " " << v.z << endl;
	return os;
}
template<typename T>
inline vec3t<T> operator*(T a, const vec3t<T>& b)
{
	return vec3t<T>(a*b.x, a*b.y, a*b.z);
}
template<typename T>
inline T dot(const vec3t<T>& a, const vec3t<T>& b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}
template<typename T>
inline vec3t<T> cross(const vec3t<T>& x, const vec3t<T>& y)
{
	return vec3t<T>(
				x.y * y.z - y.y * x.z,
				x.z * y.x - y.z * x.x,
				x.x * y.y - y.x * x.y);
}
template<typename T>
inline vec3t<T> lerp(const vec3t<T>& x, const vec3t<T>& y, T a)
{
	return x * (static_cast<T>(1) - a) + y * a;
}
template<typename T>
T NormalizeAxis(T angle)
{
	double a = mod((double)angle, 360.0);
	return a > 180.0 ? a - 360.0 : a;
}
template<typename T>
vec3t<T> NormalizeEulerAngle(const vec3t<T>& e)
{
	return vec3t<T>(
		NormalizeAxis(e.x),
		NormalizeAxis(e.y),
		NormalizeAxis(e.z)
	);
}

using u8vec3 = vec3t<unsigned char>;
using u16vec3 = vec3t<unsigned short>;
using uvec3 = vec3t<unsigned int>;
using ivec3 = vec3t<int>;
using vec3 = vec3t<float>;
using dvec3 = vec3t<double>;

#endif