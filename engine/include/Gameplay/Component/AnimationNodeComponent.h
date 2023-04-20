#ifndef ANIMATION_NODE_COMPONENT_H
#define ANIMATION_NODE_COMPONENT_H

#include "SDK/STL/STL.h"
#include "Animation/AnimationState/AnimationView.h"
#include "Component.h"



class AnimationNodeComponent : public Component
{
public:
    vector<AnimationView> animationViews;
};

#endif