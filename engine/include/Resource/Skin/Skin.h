#ifndef SKIN_H
#define SKIN_H

#include "Core/Math/Math.h"

class Skin
{
public:
    string name;
    int skeletonID;
    vector<mat4> inverseBindMatrices;
    vector<int> jointIDs;
};

#endif