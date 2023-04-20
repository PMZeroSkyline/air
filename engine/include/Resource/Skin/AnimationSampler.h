#ifndef ANIMATION_SAMPLER_H
#define ANIMATION_SAMPLER_H

#include "SDK/STL/STL.h"
#include "Core/Math/Math.h"

class AnimationSampler
{
public:
	string interpolation = "LINEAR";

	vector<float> input;
	float inputMin;
	float inputMax;

	vector<vec3> outputVec3;
	vector<quat> outputQuat;
};

#endif