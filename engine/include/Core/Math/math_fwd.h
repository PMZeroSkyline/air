#ifndef MATH_FWD_H
#define MATH_FWD_H

#include "SDK/STL/STL.h"
#include "Core/Log/Log.h"

// temp
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>
// temp end

#define CP_XYZW(a, b) a.x=b.x; a.y=b.y; a.z=b.z; a.w=b.w;
#define CP_XYZ(a, b) a.x=b.x; a.y=b.y; a.z=b.z;
#define P_XYZ(a) a.x, a.y, a.z

#ifndef PI
#define PI 3.14159265358979323846
#endif

template<typename T>
inline T lerp(T a, T b, T w)
{
	return a + (b - a) * w;
}
template<typename T>
T radians(T degrees)
{
	return ((degrees * PI ) / 180.0);
}
template<typename T>
T degrees(T radians)
{
	return (( radians * 180.0 ) / PI);
}
template<typename T>
T mod(T x, T y)
{
	T o = fmod(x, y);
	o = o < 0 ? o + y : o;
	return o;
}
template<typename T>
void sincos(T v, T& s, T& c)
{
	s = sin(v);
	c = cos(v);
}
template<typename T>
int sgn(T val) 
{
    return (0 < val) - (val < 0);
}
template<typename T>
T epsilon()
{
	return numeric_limits<T>::epsilon();
}
float FInterp(float Current, float Target, float DeltaTime, float InterpSpeed)
{
    if (Current == Target || DeltaTime <= 0.f || InterpSpeed <= 0.f)
    {
        return Current;
    }
    
    const float Distance = Target - Current;

	if(sqrt(Distance) < 0.00001f)
    {
        return Target;
    }

    const float DeltaMove = Distance * min(1.f, DeltaTime * InterpSpeed);

    return Current + DeltaMove;
}


#endif