#ifndef ANIMATION_H
#define ANIMATION_H

#include "AnimationChannel.h"
#include "AnimationSampler.h"

class Animation
{
public:
	string name;
	float samplersInputMin;
	float samplersInputMax;
	vector<AnimationChannel> channels;
	vector<AnimationSampler> samplers;
};


#endif