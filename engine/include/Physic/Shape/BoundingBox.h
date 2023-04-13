#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "Core/Math/Math.h"

class BoundingBox
{
public:
    vec3 min = vec3(0.f);
    vec3 max = vec3(0.f);
};

#endif