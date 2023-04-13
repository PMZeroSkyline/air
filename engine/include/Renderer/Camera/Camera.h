#ifndef CAMERA
#define CAMERA

#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"

class Camera
{
public:
	CameraOrthographic orthographic;
	CameraPerspective perspective;
	string type = "perspective";
};

#endif