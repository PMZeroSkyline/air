#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include "Core/Transform/Transform.h"

class SceneNode
{
public:
    string name;
    Transform localTransform;
    int meshID;
    int skinID;
    vector<int> childrenID;
};

#endif