#ifndef ANIMATION_VIEW_H
#define ANIMATION_VIEW_H

#include "AnimationInstance.h"
#include "Resource/Skin/Animation.h"

class AnimationView
{
public:
    AnimationInstance* animationInstance;
    vector<AnimationChannel*> channels;
};

#endif