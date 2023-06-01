#ifndef ANIMATION_PLAYER_COMPONENT_H
#define ANIMATION_PLAYER_COMPONENT_H

#include "Component.h"
#include "Animation/AnimationInstance.h"

class AnimationPlayerComponent : public Component
{
public:
    AnimationInstance* current = nullptr;
    vector<AnimationInstance>* animInsts = nullptr;
    
    void Play(const string& animationName, bool isLoop = false)
    {
        if (!animInsts)
        {
            return;
        }
        if (current && current->animation->name == animationName)
        {
            return;
        }
        vector<AnimationInstance>::iterator found = find_if(animInsts->begin(), animInsts->end(), [&animationName](AnimationInstance& animInst){
            return animInst.animation->name == animationName;
        });
        if (found != animInsts->end())
        {
            current = &(*found);

            for_each(animInsts->begin(), animInsts->end(), [](AnimationInstance& animInst){
                animInst.weight = 0.f;
            });
            current->weight = 1.f;
            current->time = 0.f;
            current->isLoop = isLoop;
        }
        else
        {
            LOG("not find animation " << animationName << " to play !")
        }
    }

    void Tick() override
    {
        if (current)
        {
            current->time += window->deltaTime * current->speed;
            if (current->time > current->animation->max)
            {
                current->time = current->animation->min;
            }
        }
    }
};

#endif