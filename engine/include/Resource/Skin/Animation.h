#ifndef ANIMATION_H
#define ANIMATION_H

#include "AnimationChannel.h"
#include "AnimationSampler.h"

class Animation
{
public:
	string name;
	float samplersInputMin = -1;
	float samplersInputMax = -1;
	vector<AnimationChannel> channels;
	vector<AnimationSampler> samplers;
};


#endif