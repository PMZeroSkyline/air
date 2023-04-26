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
#include "Gameplay/Object/Role.h"
#include "Render/Renderer/Renderables.h"
#include "Core/Temp/Temp.h"


int main()
{
	CDResourcesDir();
	Window window;
	Map map;
	Role* roleActor = map.AddChild<Role>();
	Actor* a1 = map.AddChild<Actor>();
	ScenesComponent* s1 = a1->AddComponent<ScenesComponent>();
	s1->Load("bar_zup/bar.gltf");
	Renderables renderables;
	map.ResetWorldMatrix(true);
	map.Start();
	while (window.IsOpen())
	{
		window.Tick();
		GLClear();
		map.Tick();
		renderables.Load(&map);
		renderables.Render();
		if (window.keys[GLFW_KEY_ESCAPE].pressDown)
		{
			window.Close();
		}
	}
	return 0;
}