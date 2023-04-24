#ifndef VEC2_H
#define VEC2_H

#include "math_fwd.h"

template<typename T>
struct vec2t
{
    T x,y;
	vec2t<T>() : x(0), y(0){}
	vec2t<T>(T i) : x(i), y(i){}
	vec2t<T>(T _x, T _y) : x(_x), y(_y){}
	vec2t<T>(const vec2t<unsigned char> &v) : x(v.x), y(v.y) {}
	vec2t<T>(const vec2t<unsigned short> &v) : x(v.x), y(v.y) {}
	vec2t<T>(const vec2t<unsigned int> &v) : x(v.x), y(v.y) {}
	vec2t<T>(const vec2t<int> &v) : x(v.x), y(v.y) {}
	vec2t<T>(const vec2t<float> &v) : x(v.x), y(v.y) {}
	vec2t<T> operator+=(const vec2t<T>& v);
	vec2t<T> operator-=(const vec2t<T>& v);
	vec2t<T> operator*=(const vec2t<T>& v);
	vec2t<T> operator/=(const vec2t<T>& v);
	vec2t<T> operator+(const vec2t<T>& v) const;
	vec2t<T> operator-(const vec2t<T>& v) const;
	vec2t<T> operator*(const vec2t<T>& v) const;
	vec2t<T> operator/(const vec2t<T>& v) const;
	vec2t<T> operator-() const;
	bool operator==(const vec2t<T>& v) const;
	bool operator!=(const vec2t<T>& v) const;
	T& operator[](int i);
	T const& operator[](int i) const;
    T length() const;
    vec2t<T> normalize() const;
};
template<typename T>
inline vec2t<T> vec2t<T>::operator+(const vec2t<T>& v) const
{
	return vec2t<T>(x+v.x, y+v.y);
}
template<typename T>
inline vec2t<T> vec2t<T>::operator-(const vec2t<T>& v) const
{
	return vec2t<T>(x-v.x, y-v.y);
}
template<typename T>
inline vec2t<T> vec2t<T>::operator*(const vec2t<T>& v) const
{
	return vec2t<T>(x*v.x, y*v.y);
}
template<typename T>
inline vec2t<T> vec2t<T>::operator/(const vec2t<T>& v) const
{
	return vec2t<T>(x/v.x, y/v.y);
}
template<typename T>
inline vec2t<T> vec2t<T>::operator+=(const vec2t<T>& v)
{
	x+=v.x; y+=v.y;
	return *this;
}
template<typename T>
inline vec2t<T> vec2t<T>::operator-=(const vec2t<T>& v)
{
	x-=v.x; y-=v.y;
	return *this;
}
template<typename T>
inline vec2t<T> vec2t<T>::operator*=(const vec2t<T>& v)
{
	x*=v.x; y*=v.y;
	return *this;
}
template<typename T>
inline vec2t<T> vec2t<T>::operator/=(const vec2t<T>& v)
{
	x/=v.x; y/=v.y;
	return *this;
}
template<typename T>
inline vec2t<T> vec2t<T>::operator-() const
{
	return vec2t<T>(-x,-y);
}
template<typename T>
inline bool vec2t<T>::operator==(const vec2t<T>& v) const
{
	return ((x==v.x) && (y==v.y));
}
template<typename T>
inline bool vec2t<T>::operator!=(const vec2t<T>& v) const
{
	return ((x!=v.x) && (y!=v.y));
}
template<typename T>
inline T& vec2t<T>::operator[](int i)
{
    return *((T*)this+i);
}
template<typename T>
inline T const& vec2t<T>::operator[](int i) const
{
	return *((T*)this+i);
}
template<typename T>
inline T vec2t<T>::length() const
{
    return sqrt(x*x+y*y);
}
template<typename T>
inline vec2t<T> vec2t<T>::normalize() const
{
    return (*this)/length();
}
template<typename T>
inline ostream& operator<<(ostream& os, const vec2t<T> &v)
{
	os << v.x << " " << v.y << endl;
	return os;
}
template<typename T>
T dot(const vec2t<T>& a, const vec2t<T>& b)
{
    return a.x*b.x + a.y*b.y;
}

using u8vec2 = vec2t<unsigned char>;
using u16vec2 = vec2t<unsigned short>;
using uvec2 = vec2t<unsigned int>;
using ivec2 = vec2t<int>;
using vec2 = vec2t<float>;
using dvec2 = vec2t<double>;

#endif