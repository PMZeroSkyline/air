#ifndef ANIMATION_INSTANCE_H
#define ANIMATION_INSTANCE_H

#include "Resource/Skin/Animation.h"

class AnimationInstance
{
public:
    bool isLoop = false;
    float weight = 0.f;
    float time = 0.f;
    Animation* animation = nullptr;
};

#endif