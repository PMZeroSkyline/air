#include <iostream>
#include "OS/Window/Window.h"
#include "Resource/Model/Scenes.h"
#include "Platform/File/FileDirectory.h"
#include "SDK/OpenGL/CppOpenGL.h"
#include "Resource/Model/Mesh.h"
#include "Gameplay/Component/ScenesComponent.h"
#include "Render/Camera/Camera.h"
#include "Gameplay/Component/CameraComponent.h"
#include "Map/Map.h"
#include "Gameplay/Object/Actor.h"
#include "Render/Renderer/Renderables.h"


int main()
{
	CDResourcesDir();
	Window window;

	Map map;
	Actor* sa = map.AddChild<Actor>();
	ScenesComponent* sc = sa->AddComponent<ScenesComponent>();
	sc->Load("vroid/anim/anim.gltf");
	sc->animationInstances[0].weight = 1.f;
	
	Actor* ca = map.AddChild<Actor>();
	ca->localTransform.translation.y = 1.5;
	ca->localTransform.translation.z = 1;
	ca->localTransform.rotation = EulerToQuat(vec3(-15,0,0));
	ca->AddComponent<CameraComponent>();
	
	Renderables renderables;

	while (window.IsOpen())
	{
		window.Tick();
		
		GLClear();

		sc->animationInstances[0].time = (sin(window.time)*.5+.5) * sc->animationInstances[0].animation->samplersInputMax;
		map.ResetWorldMatrix();
		renderables.Load(&map);
		renderables.Render();

		if (window.keys[GLFW_KEY_ESCAPE].pressDown)
		{
			window.Close();
		}
	}
	return 0;
}