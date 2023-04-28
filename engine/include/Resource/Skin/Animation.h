#ifndef ANIMATION_H
#define ANIMATION_H

#include "AnimationChannel.h"
#include "AnimationSampler.h"

class Animation
{
public:
	string name;
	float min = -1;
	float max = -1;
	vector<AnimationChannel> channels;
	vector<AnimationSampler> samplers;
};


#endif