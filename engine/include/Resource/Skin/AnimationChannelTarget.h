#ifndef ANIMATION_CHANNEL_TARGET_H
#define ANIMATION_CHANNEL_TARGET_H

#include "SDK/STL/STL.h"
#include "Core/Log/Log.h"

enum AnimationChannelTargetPath
{
	TRANSLATION,
	ROTATION,
	SCALE
};

class AnimationChannelTarget
{
public:
	int nodeID = -1;
	// string name;
	AnimationChannelTargetPath path;

	void SetupPath(const string& pathStr)
	{
		if (pathStr == "translation")
		{
			path = AnimationChannelTargetPath::TRANSLATION;
		}
		else if (pathStr == "rotation")
		{
			path = AnimationChannelTargetPath::ROTATION;
		}
		else if (pathStr == "scale")
		{
			path = AnimationChannelTargetPath::SCALE;
		}
		else
		{
			LOG("failed setup AnimationChannelTargetPath " << pathStr << " !")
		}
	}
};

#endif
