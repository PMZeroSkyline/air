#ifndef ANIMATION_NODE_COMPONENT_H
#define ANIMATION_NODE_COMPONENT_H

#include "SDK/STL/STL.h"
#include "Animation/AnimationState/AnimationInstanceView.h"
#include "Component.h"



class AnimationNodeComponent : public Component
{
public:
    vector<AnimationInstanceView> animationInstanceViews;

    AnimationInstanceView* GetAnimationInstanceView(AnimationInstance* animationInstance)
    {
        for (int i = 0; i < animationInstanceViews.size(); i++)
        {
            if (animationInstanceViews[i].animationInstance == animationInstance)
            {
                return &animationInstanceViews[i];
            }
        }
        return nullptr;
    }
};

#endif