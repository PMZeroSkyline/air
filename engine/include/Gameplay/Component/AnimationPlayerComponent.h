#ifndef ANIMATION_PLAYER_COMPONENT_H
#define ANIMATION_PLAYER_COMPONENT_H

#include "Component.h"
#include "Animation/AnimationInstance.h"

class AnimationPlayerComponent : public Component
{
public:
    AnimationInstance* playInst = nullptr;
    vector<AnimationInstance>* animInsts = nullptr;
    
    void Play(const string& animationName, bool isLoop = false)
    {
        if (!animInsts)
        {
            return;
        }
        if (playInst && playInst->animation->name == animationName)
        {
            return;
        }
        vector<AnimationInstance>::iterator found = find_if(animInsts->begin(), animInsts->end(), [&animationName](AnimationInstance& animInst){
            return animInst.animation->name == animationName;
        });
        if (found != animInsts->end())
        {
            playInst = &(*found);

            for_each(animInsts->begin(), animInsts->end(), [](AnimationInstance& animInst){
                animInst.weight = 0.f;
            });
            playInst->weight = 1.f;
            playInst->time = 0.f;
            playInst->isLoop = isLoop;
        }
        else
        {
            LOG("not find animation " << animationName << " to play !")
        }
    }

    void Tick() override
    {
        if (playInst)
        {
            playInst->time += window->deltaTime * playInst->speed;
            if (playInst->time > playInst->animation->max)
            {
                playInst->time = playInst->animation->min;
            }
        }
    }
};

#endif