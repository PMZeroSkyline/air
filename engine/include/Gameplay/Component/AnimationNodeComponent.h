#ifndef ANIMATION_NODE_COMPONENT_H
#define ANIMATION_NODE_COMPONENT_H

#include "SDK/STL/STL.h"
#include "Animation/AnimationState/AnimationInstanceView.h"
#include "Component.h"
#include "Core/Transform/Transform.h"
#include "Core/Log/Log.h"


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
    Transform GetAnimationTransform() const
    {
        Transform result;
        for (int i = 0; i < animationInstanceViews.size(); i++)
        {
            const AnimationInstanceView* view = &animationInstanceViews[i];
            const AnimationInstance* animationInstance = view->animationInstance;
            if (animationInstance->weight <= 0)
            {
                continue;
            }
            
            for (int j = 0; j < view->channels.size(); j++)
            {
                AnimationChannel* channel = view->channels[j];
                if (channel->target.path == AnimationChannelTargetPath::translation)
                {
                    result.translation += channel->sampler->SampleVec3(animationInstance->time);
                }
                else if(channel->target.path == AnimationChannelTargetPath::rotation)
                {
                    result.rotation *= channel->sampler->SampleQuat(animationInstance->time);
                }
                else if(channel->target.path == AnimationChannelTargetPath::scale)
                {
                    result.scaling *= channel->sampler->SampleVec3(animationInstance->time);
                }
                else
                {
                    LOG("AnimationNodeComponent GetTransform not supported AnimationChannelTargetPath !")
                }
            }
        }
        return result;
    }
};

#endif