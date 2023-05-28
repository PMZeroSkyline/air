#ifndef PERSPECITVE_CAMERA
#define PERSPECITVE_CAMERA

#include "OS/Window/window.h"
#include "Core/Math/Math.h"

class CameraPerspective
{
public:
	bool isUseFramebufferAspect = true;
	float aspectRatio = 1.f;
	float yfov = 60.0f;
	float zfar = 0.1f;
	float znear = 1000.f;
	mat4 GetPerspectiveMatrix()
	{
		float aspect;
		if (isUseFramebufferAspect)
		{
			int width, height;
			glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
			aspect = ((float)width) / ((float)height);
		}
		else
		{
			aspect = aspectRatio;
		}
		return PerspectiveProjection(radians(yfov), aspect, znear, zfar);
	}
};

#endif