#ifndef ANIMATION_SAMPLER_H
#define ANIMATION_SAMPLER_H

#include "SDK/STL/STL.h"
#include "Core/Math/Math.h"
#include "Core/Log/Log.h"

class AnimationSampler
{
public:
	string interpolation = "LINEAR";

	vector<float> input;
	float inputMin;
	float inputMax;

	vector<vec3> outputVec3;
	vector<quat> outputQuat;

	void GetInputInterp(float time, int& x, int& y, float& interp) const
	{
		if (time <= inputMin)
		{
			interp = 0.f;
			x = 0.f;
			y = 0.f;
			return;
		}
		if (time >= inputMax)
		{
			interp = 1.f;
			x = input.size() - 1;
			y = input.size() - 1;
			return;
		}
		for (int i = 0; i < input.size() - 1; i++)
		{
			if (input[i] <= time && input[i+1] >= time)
			{
				float localTime = time - input[i];
				float localTimeRange = input[i+1] - input[i];
				interp = localTime / localTimeRange;
				x = i;
				y = i + 1;
				return;
			}
		}
		interp = 0.f;
		x = 0;
		y = 0;
		LOG("AnimationSampler GetInputPair out of range !")
	}
	vec3 SampleVec3(float time) const
	{
		if (outputVec3.size() == 0 || input.size() == 0)
		{
			return vec3();
		}
		float interp;
		int x, y;
		GetInputInterp(time, x, y, interp);
		if (interp == 0.f)
		{
			return outputVec3[x];
		}
		if (interp == 1.f)
		{
			return outputVec3[y];
		}
		return lerp(outputVec3[x], outputVec3[y], interp);
	}
	quat SampleQuat(float time) const
	{
		if (outputQuat.size() == 0 || outputQuat.size() == 0)
		{
			return quat();
		}
		float interp;
		int x, y;
		GetInputInterp(time, x, y, interp);
		if (interp == 0.f)
		{
			return outputQuat[x];
		}
		if (interp == 1.f)
		{
			return outputQuat[y];
		}
		return slerp(outputQuat[x], outputQuat[y], interp);
	}
};

#endif