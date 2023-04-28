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
#include "Core/Parse/TreeFileParse.h"

int main()
{
	CDResourcesDir();
	Window window;

	Node root;
	TreeFileParse("anim/anim.md", &root);

	Map map;
	Role* roleActor = map.AddChild<Role>();
	Actor* a1 = map.AddChild<Actor>();
	a1->localTransform.rotation = EulerToQuat(0.f, 0.f, 0.f);
	ScenesComponent* s1 = a1->AddComponent<ScenesComponent>();
	s1->Load("bar_zup/bar.gltf");
	// s1->animationInstances[0].weight = 1.f;
	// s1->animationInstances[0].time = 1.f;							
	GLLineMode();
	Renderables renderables;
	map.Start();
	map.ResetWorldMatrix(true);
	while (window.IsOpen())
	{
		window.Tick();
		GLClear();
		
		map.Tick();
		renderables.Load(&map);
		renderables.Render();
		
		if (window.keys[KEY::ESCAPE].pressDown)
		{
			window.Close();
		}
	}
	return 0;
}