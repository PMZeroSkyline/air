#ifndef ORTHOGRAPHIC_CAMERA
#define ORTHOGRAPHIC_CAMERA

#include "Core/Math/Math.h"

class CameraOrthographic
{
public:
	bool isUseFramebufferAspect = true;
	float xmag = 5.f;
	float ymag = 5.f;
	float zfar = 0.1f;
	float znear = 1000.f;
	mat4 GetOrthographicMatrix()
	{
		if (isUseFramebufferAspect)
		{
			int width, height;
			glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
			float aspect = ((float)width) / ((float)height);
			return OrthographicProjection(aspect * xmag, ymag, znear, zfar);
		}
		return OrthographicProjection(xmag, ymag, znear, zfar);
	}
};

#endif