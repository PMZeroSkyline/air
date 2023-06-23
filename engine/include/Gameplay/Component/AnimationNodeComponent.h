#ifndef ANIMATION_NODE_COMPONENT_H
#define ANIMATION_NODE_COMPONENT_H

#include "SDK/STL/STL.h"
#include "Animation/AnimationView.h"
#include "Component.h"
#include "Core/Transform/Transform.h"
#include "Core/Log/Log.h"


class AnimationNodeComponent : public Component
{
public:
    vector<AnimationView> animationViews;

    AnimationView* GetAnimationView(AnimationInstance* animationInstance)
    {
        for (int i = 0; i < animationViews.size(); i++)
        {
            if (animationViews[i].animationInstance == animationInstance)
            {
                return &animationViews[i];
            }
        }
        return nullptr;
    }
    Transform GetAnimationTransform(float offset = 0) const
    {
        Transform result;
        for (int i = 0; i < animationViews.size(); i++)
        {
            const AnimationView* view = &animationViews[i];
            const AnimationInstance* animationInstance = view->animationInstance;
            if (animationInstance->weight <= 0)
            {
                continue;
            }
            for (int j = 0; j < view->channels.size(); j++)
            {
                AnimationChannel* channel = view->channels[j];
                AnimationSampler* sampler = &animationInstance->animation->samplers[channel->samplerId];
                // AnimationSampler* sampler = channel->sampler;
                if (channel->target.path == AnimationChannelTargetPath::TRANSLATION)
                {
                    result.translation += sampler->SampleVec3(animationInstance->time + offset);
                }
                else if(channel->target.path == AnimationChannelTargetPath::ROTATION)
                {
                    result.rotation *= sampler->SampleQuat(animationInstance->time + offset);
                }
                else if(channel->target.path == AnimationChannelTargetPath::SCALE)
                {
                    result.scaling *= sampler->SampleVec3(animationInstance->time + offset);
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