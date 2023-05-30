#ifndef ORTHOGRAPHIC_CAMERA
#define ORTHOGRAPHIC_CAMERA

#include "Camera.h"

class OrthographicCamera : public Camera
{
public:
	bool isUseFramebufferAspect = true;
	float xmag = 5.f;
	float ymag = 5.f;
	float zfar = 0.1f;
	float znear = 10.f;
	
	virtual mat4 GetProjectioMatrix() override
	{
		if (isUseFramebufferAspect)
		{
			int width, height;
			glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
			float aspect = ((float)width) / ((float)height);
			float axmag = aspect * xmag;
			//return OrthographicProjection(-axmag, axmag, ymag, -ymag, znear, zfar);
			return OrthographicProjection(axmag, ymag, znear, zfar);
		}
		//return OrthographicProjection(-xmag, xmag, ymag, -ymag, znear, zfar);
		return OrthographicProjection(xmag, ymag, znear, zfar);
	}
};

#endif