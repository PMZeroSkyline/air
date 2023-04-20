#ifndef ANIMATION_INSTANCE_H
#define ANIMATION_INSTANCE_H

#include "Resource/Skin/Animation.h"

class AnimationInstance
{
public:
    float weight = 0.f;
    Animation* animation = nullptr;
};

#endif