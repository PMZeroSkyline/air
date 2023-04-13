#ifndef VEC4_H
#define VEC4_H

#include "math_fwd.h"

template<typename T>
struct vec4t
{
    T x,y,z,w;
	vec4t<T>() : x(0), y(0), z(0), w(0) {}
	vec4t<T>(T i) : x(i), y(i), z(i), w(i) {}
	vec4t<T>(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}
	vec4t<T>(const vec4t<unsigned char> &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
	vec4t<T>(const vec4t<unsigned short> &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
	vec4t<T>(const vec4t<unsigned int> &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
	vec4t<T>(const vec4t<int> &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
	vec4t<T>(const vec4t<float> &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
	vec4t<T> operator+=(const vec4t<T>& v);
	vec4t<T> operator-=(const vec4t<T>& v);
	vec4t<T> operator*=(const vec4t<T>& v);
	vec4t<T> operator/=(const vec4t<T>& v);
	vec4t<T> operator+(const vec4t<T>& v) const;
	vec4t<T> operator-(const vec4t<T>& v) const;
	vec4t<T> operator*(const vec4t<T>& v) const;
	vec4t<T> operator/(const vec4t<T>& v) const;
	vec4t<T> operator-() const;
	bool operator==(const vec4t<T>& v) const;
	bool operator!=(const vec4t<T>& v) const;
	T& operator[](int i);
	T const& operator[](int i) const;
    T length() const;
    vec4t<T> normalize() const;
};

template<typename T>
inline vec4t<T> vec4t<T>::operator+(const vec4t<T>& v) const
{
	return vec4t<T>(x+v.x, y+v.y, z+v.z, w+v.w);
}
template<typename T>
inline vec4t<T> vec4t<T>::operator-(const vec4t<T>& v) const
{
	return vec4t<T>(x-v.x, y-v.y, z-v.z, w-v.w);
}
template<typename T>
inline vec4t<T> vec4t<T>::operator*(const vec4t<T>& v) const
{
	return vec4t<T>(x*v.x, y*v.y, z*v.z, w*v.w);
}
template<typename T>
inline vec4t<T> vec4t<T>::operator/(const vec4t<T>& v) const
{
	return vec4t<T>(x/v.x, y/v.y, z/v.z, w/v.w);
}
template<typename T>
inline vec4t<T> vec4t<T>::operator+=(const vec4t<T>& v)
{
	x+=v.x; y+=v.y; z+=v.z; w+=v.w;
	return *this;
}
template<typename T>
inline vec4t<T> vec4t<T>::operator-=(const vec4t<T>& v)
{
	x-=v.x; y-=v.y; z-=v.z; w-=v.w;
	return *this;
}
template<typename T>
inline vec4t<T> vec4t<T>::operator*=(const vec4t<T>& v)
{
	x*=v.x; y*=v.y; z*=v.z; w*=v.w;
	return *this;
}
template<typename T>
inline vec4t<T> vec4t<T>::operator/=(const vec4t<T>& v)
{
	x/=v.x; y/=v.y; z/=v.z; w/=v.w;
	return *this;
}
template<typename T>
inline vec4t<T> vec4t<T>::operator-() const
{
	return vec4t<T>(-x,-y,-z,-w);
}
template<typename T>
inline bool vec4t<T>::operator==(const vec4t<T>& v) const
{
	return ((x==v.x) && (y==v.y) && (z==v.z) && (w==v.w));
}
template<typename T>
inline bool vec4t<T>::operator!=(const vec4t<T>& v) const
{
	return ((x!=v.x) && (y!=v.y) && (z!=v.z) && (w!=v.w));
}
template<typename T>
inline T& vec4t<T>::operator[](int i)
{
    return *((T*)this+i);
}
template<typename T>
inline T const& vec4t<T>::operator[](int i) const
{
	return *((T*)this+i);
}
template<typename T>
inline T vec4t<T>::length() const
{
    return sqrt(x*x+y*y+z*z+w*w);
}
template<typename T>
inline vec4t<T> vec4t<T>::normalize() const
{
    return (*this)/length();
}
template<typename T>
inline ostream& operator<<(ostream& os, const vec4t<T> &v)
{
	os << v.x << " " << v.y << " " << v.z << " " << v.w << endl;
	return os;
}
template<typename T>
T dot(const vec4t<T>& a, const vec4t<T>& b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

using u8vec4 = vec4t<unsigned char>;
using u16vec4 = vec4t<unsigned short>;
using uvec4 = vec4t<unsigned int>;
using ivec4 = vec4t<int>;
using vec4 = vec4t<float>;

#endif