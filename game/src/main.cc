#include "Gameplay/Object/Role.h"
#include "Gameplay/World/WorldGenerate.h"
#include "Platform/File/FileDirectory.h"
#include "OS/Window/Window.h"
#include "Render/Render/Render.h"
#include "Resource/Texture/Image.h"
#include "Resource/Texture/TextureCube.h"


int main(int argc, char** argv)
{
	// EXEC_PO2
	CDWorkspacePath("air");

	
	Window window;

	shared_ptr<Actor> world = make_shared<Actor>();
	GenSandbox(world.get());
	// Actor* aScene = world->AddChild<Actor>();
	// ScenesComponent* cScenes = aScene->AddComponent<ScenesComponent>();
	// cScenes->Load("game/res/Model/Architecture/LightRoom/LightRoom.gltf");
	// cScenes->FieldExpand();

	world->AddChild<Role>();
	world->ResetWorldMatrix();

	Render render;
	while (window.IsOpen())
	{
		window.Tick();
		world->ResetWorldMatrix();
		world->Tick();
		
		
		render.Load(world.get());
		render.Draw();
		
		
		if (window.keys[KEY::ESCAPE].pressDown)
		{
			window.Close();
		}
		
		if (window.keys[KEY::F8].pressDown)
		{
			window.SetCursor(!window.GetCursor());
		}
		
	}
	return 0;
}

