#ifndef ANIMATION_SAMPLER_H
#define ANIMATION_SAMPLER_H

#include "SDK/STL/STL.h"
#include "Core/Math/Math.h"
#include "Core/Log/Log.h"

class AnimationSampler
{
public:
	string interpolation = "LINEAR";

	vector<float> inputs;
	float min;
	float max;

	vector<vec3> outputVec3s;
	vector<quat> outputQuats;

	void GetInputInterp(float time, int& x, int& y, float& interp) const
	{
		if (time <= min)
		{
			interp = 0.f;
			x = 0.f;
			y = 0.f;
			return;
		}
		if (time >= max)
		{
			interp = 1.f;
			x = inputs.size() - 1;
			y = inputs.size() - 1;
			return;
		}
		for (int i = 0; i < inputs.size() - 1; i++)
		{
			if (inputs[i] <= time && inputs[i+1] >= time)
			{
				float localTime = time - inputs[i];
				float localTimeRange = inputs[i+1] - inputs[i];
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
		if (outputVec3s.size() == 0 || inputs.size() == 0)
		{
			return vec3();
		}
		float interp;
		int x, y;
		GetInputInterp(time, x, y, interp);
		if (interp == 0.f)
		{
			return outputVec3s[x];
		}
		if (interp == 1.f)
		{
			return outputVec3s[y];
		}
		return lerp(outputVec3s[x], outputVec3s[y], interp);
	}
	quat SampleQuat(float time) const
	{
		if (outputQuats.size() == 0 || outputQuats.size() == 0)
		{
			return quat();
		}
		float interp;
		int x, y;
		GetInputInterp(time, x, y, interp);
		if (interp == 0.f)
		{
			return outputQuats[x];
		}
		if (interp == 1.f)
		{
			return outputQuats[y];
		}
		return slerp(outputQuats[x], outputQuats[y], interp);
	}
};

#endif