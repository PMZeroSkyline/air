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

	void ResetMinMax()
	{
		for (int i = 0; i < samplers.size(); i++)
		{
			if (i == 0)
			{
				min = samplers[i].min;
				max = samplers[i].max;
			}
			else
			{
				min = std::min(min, samplers[i].min);
				max = std::max(max, samplers[i].max);
			}
		}
	}
};


#endif