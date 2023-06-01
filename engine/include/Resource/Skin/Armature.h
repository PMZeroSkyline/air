#ifndef ARMATURE_H
#define ARMATURE_H

#include "Core/Math/Math.h"

class Armature
{
public:
    string name;
    vector<mat4> inverseBindMatrices;
    vector<int> jointIDs;
};

#endif