#ifndef ANIMATION_INSTANCE_H
#define ANIMATION_INSTANCE_H

#include "Resource/Skin/Animation.h"

class AnimationInstance
{
public:
    Animation* animation = nullptr;
    
    float weight = 0.f;
    float time = 0.f;
    bool isLoop = false;
    float speed = 1.f;
};

#endif