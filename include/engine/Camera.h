#ifndef CAMERA_H
#define CAMERA_H

#include "Core.h"
#include "AC.h"

class CameraOrthographic
{
public:
	float xmag;
	float ymag;
	float zfar;
	float znear;
};
class CameraPerspective
{
public:
	float aspectRatio = -1.f;
	float yfov = 60.0f;
	float zfar = 0.1f;
	float znear = 1000.f;
	mat4 GetPerspectiveMatrix()
	{
		float aspect;
		if (aspectRatio <= 0)
		{
			int width, height;
			glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
			aspect = ((float)width) / ((float)height);
		}
		else
		{
			aspect = aspectRatio;
		}
		return perspective(radians(yfov), aspect, znear, zfar);
	}
};
class Camera
{
public:
	CameraOrthographic orthographic;
	CameraPerspective perspective;
	string type = "perspective";
};
class CameraComponent : public Component
{
public:
	Camera camera;
	mat4 GetProjectionMatrix()
	{
		if (camera.type == "perspective")
		{
			return camera.perspective.GetPerspectiveMatrix();
		}
		return mat4(1.);
	}
};

#endif