#ifndef CAMERA_CONPONENT_H
#define CAMERA_CONPONENT_H

#include "Component.h"
#include "Render/Camera/Camera.h"
#include "Render/Camera/OrthographicCamera.h"
#include "Render/Camera/PerspectiveCamera.h"

class CameraComponent : public Component
{
public:

	shared_ptr<Camera> camera;

};

#endif