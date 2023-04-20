#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include "Core/Transform/Transform.h"

class SceneNode
{
public:
    string name;
    Transform localTransform;
    int meshID = -1;
    int skinID = -1;
    vector<int> childrenID;
};

#endif