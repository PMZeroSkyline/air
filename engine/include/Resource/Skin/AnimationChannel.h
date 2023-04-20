#ifndef ANIMATION_CHANNEL_H
#define ANIMATION_CHANNEL_H

#include "SDK/STL/STL.h"
#include "AnimationSampler.h"
#include "AnimationChannelTarget.h"
#include "AnimationSampler.h"

class AnimationChannel
{
public:
	AnimationSampler* sampler;
	AnimationChannelTarget target;
};


#endif