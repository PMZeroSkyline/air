#ifndef CAMERA_CONPONENT_H
#define CAMERA_CONPONENT_H

#include "Component.h"
#include "Renderer/Camera/Camera.h"

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
		return mat4();
	}
};

#endif