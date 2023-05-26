#ifndef AABB_H
#define AABB_H

#include "Core/Math/Math.h"

class AABB
{
public:
    vec3 min = vec3(0.f);
    vec3 max = vec3(0.f);
    AABB() = default;
    AABB(vec3 inMin, vec3 inMax) : min(inMin), max(inMax){}
    bool Check(const vec3& v) const
    {
        bool x = v.x < max.x && v.x > min.x;
        bool y = v.y < max.y && v.y > min.y;
        bool z = v.z < max.z && v.z > min.z;
        return x && y && z;
    }
    void Merge(const AABB& b)
    {
        min.x = std::min(min.x, b.min.x);
        min.y = std::min(min.y, b.min.y);
        min.z = std::min(min.z, b.min.z);
        max.x = std::max(max.x, b.max.x);
        max.y = std::max(max.y, b.max.y);
        max.z = std::max(max.z, b.max.z);
    }
};
ostream &operator<<(ostream &os, const AABB& b)
{
	os << "min\t" << b.min << "max\t" << b.max << endl;
	return os;
}
#endif