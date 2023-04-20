#ifndef SKIN_INSTANCE_H
#define SKIN_INSTANCE_H

#include "Resource/Skin/Skin.h"
#include "Gameplay/Object/Actor.h"

class SkinInstance
{
public:
    Skin* skin;
    vector<Actor*> joints;
};

#endif