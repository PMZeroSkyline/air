#ifndef ARMATURE_INSTANCE_H
#define ARMATURE_INSTANCE_H

#include "Resource/Skin/Armature.h"
#include "Gameplay/Object/Actor.h"

class ArmatureInstance
{
public:
    Armature* armature;
    vector<Actor*> joints;
};

#endif