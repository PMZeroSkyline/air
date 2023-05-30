#ifndef CAMERA
#define CAMERA

#include "Core/Math/Math.h"

class Camera
{
public:
	virtual ~Camera(){}

	virtual mat4 GetProjectioMatrix() = 0;
};

#endif