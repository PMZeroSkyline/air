#ifndef ANIMATION_CHANNEL_TARGET_H
#define ANIMATION_CHANNEL_TARGET_H

#include "SDK/STL/STL.h"
#include "Core/Log/Log.h"

enum AnimationChannelTargetPath
{
	translation,
	rotation,
	scale
};

class AnimationChannelTarget
{
public:
	int nodeID = -1;
	AnimationChannelTargetPath path;

	void SetupPath(const string& pathStr)
	{
		if (pathStr == "translation")
		{
			path = AnimationChannelTargetPath::translation;
			return;
		}
		else if (pathStr == "rotation")
		{
			path = AnimationChannelTargetPath::rotation;
			return;
		}
		else if (pathStr == "scale")
		{
			path = AnimationChannelTargetPath::scale;
			return;
		}
		LOG("failed setup AnimationChannelTargetPath " << pathStr << " !")
	}
};

#endif
