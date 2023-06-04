#ifndef PERSPECITVE_CAMERA
#define PERSPECITVE_CAMERA

#include "Camera.h"

class PerspectiveCamera : public Camera
{
public:
	bool isUseFramebufferAspect = true;
	float aspectRatio = 1.f;
	float yfov = 60.0f;
	float zfar = 1000.f;
	float znear = 0.1f;

	virtual mat4 GetProjectioMatrix() override
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