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
	vec3t<T> ToEuler() const;
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
#define WIKI_CONVERSION
#ifdef WIKI_CONVERSION
// Euler angles (in 3-2-1 sequence) to quaternion conversion
template<typename T>
quatt<T>::quatt(const vec3t<T>& in_euler)
{
	// https://en.wikipedia.org/wiki/Conversion_between_quats_and_Euler_angles
	vec3t<T> euler = radians(in_euler);
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
// this implementation assumes normalized quaternion
// converts to Euler angles in 3-2-1 sequence
template<typename T>
vec3t<T> quatt<T>::ToEuler() const
{
	// https://en.wikipedia.org/wiki/Conversion_between_quats_and_Euler_angles
	vec3t<T> angles;
	// roll (x-axis rotation)
	T sinr_cosp = 2 * (w * x + y * z);
	T cosr_cosp = 1 - 2 * (x * x + y * y);
	angles.x = atan2(sinr_cosp, cosr_cosp);
	// pitch (y-axis rotation)
	T sinp = sqrt(1 + 2 * (w * y - x * z));
	T cosp = sqrt(1 - 2 * (w * y - x * z));
	angles.y = 2 * atan2(sinp, cosp) - PI / 2;
	// yaw (z-axis rotation)
	T siny_cosp = 2 * (w * z + x * y);
	T cosy_cosp = 1 - 2 * (y * y + z * z);
	angles.z = atan2(siny_cosp, cosy_cosp);
	return degrees(angles);
}
#else
template<typename T>
quatt<T>::quatt(const vec3t<T>& euler)
{
	// Unreal Math
	const double Pitch = euler.y;
	const double Yaw = euler.z;
	const double Roll = euler.x;

	const double DEG_TO_RAD = PI/(180.0);
	const double RADS_DIVIDED_BY_2 = DEG_TO_RAD/2.0;
	double SP, SY, SR;
	double CP, CY, CR;

	const double PitchNoWinding = mod(Pitch, 360.0);
	const double YawNoWinding = mod(Yaw, 360.0);
	const double RollNoWinding = mod(Roll, 360.0);

	sincos(PitchNoWinding * RADS_DIVIDED_BY_2, SP, CP);
	sincos(YawNoWinding * RADS_DIVIDED_BY_2, SY, CY);
	sincos(RollNoWinding * RADS_DIVIDED_BY_2, SR, CR);

	x =  CR*SP*SY - SR*CP*CY;
	y = -CR*SP*CY - SR*CP*SY;
	z =  CR*CP*SY - SR*SP*CY;
	w =  CR*CP*CY + SR*SP*SY;
}
// this implementation assumes normalized quaternion
// converts to Euler angles in 3-2-1 sequence
template<typename T>
vec3t<T> quatt<T>::ToEuler() const
{
	// Unreal Math
	const double SingularityTest = z * x - w * y;
	const double YawY = 2.0f * (w * z + x * y);
	const double YawX = (1.0f - 2.0f * (y*y + z*z));

	const double SINGULARITY_THRESHOLD = 0.4999995;
	const double RAD_TO_DEG = (180.0 / PI);
	double Pitch, Yaw, Roll;

	if (SingularityTest < -SINGULARITY_THRESHOLD)
	{
		Pitch = -90.0;
		Yaw = (atan2(YawY, YawX) * RAD_TO_DEG);
		Roll = NormalizeAxis(-Yaw - (2.0 * atan2(x, w) * RAD_TO_DEG));
	}
	else if (SingularityTest > SINGULARITY_THRESHOLD)
	{
		Pitch = 90.0;
		Yaw = (atan2(YawY, YawX) * RAD_TO_DEG);
		Roll = NormalizeAxis(Yaw - (2.0 * atan2(x, w) * RAD_TO_DEG));
	}
	else
	{
		Pitch = (asin(2.0 * SingularityTest) * RAD_TO_DEG); // Note: not FastAsin like float implementation
		Yaw = (atan2(YawY, YawX) * RAD_TO_DEG);
		Roll = (atan2(-2.0 * (w * x + y * z), (1.0 - 2.0 * (x*x + y*y))) * RAD_TO_DEG);
	}

	return vec3t<T>(Roll, Pitch, Yaw);
}
#endif
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