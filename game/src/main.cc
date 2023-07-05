#include "Gameplay/Object/Role.h"
#include "Gameplay/World/WorldGenerate.h"
#include "Platform/File/FileDirectory.h"
#include "OS/Window/Window.h"
#include "Render/Render/Render.h"
#include "Resource/Texture/Image.h"
#include "Resource/Texture/TextureCube.h"


#ifdef __APPLE__
#else
// #include <windows.h>
#ifndef DWORD
#define DWORD unsigned int
#endif
extern "C"
{
   __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
   __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
#endif


int main(int argc, char** argv)
{
	// EXEC_PO2
	CDWorkspacePath("air");
	LOG(fs::current_path().string())
	
	Window window;

	shared_ptr<Actor> world = make_shared<Actor>();
	// GenSandbox(world.get());
	Actor* aScene = world->AddChild<Actor>();
	ScenesComponent* cScenes = aScene->AddComponent<ScenesComponent>();
	cScenes->Load("game/res/Model/Architecture/Sponza/scene.gltf");
	cScenes->FieldExpand();

	Role* role = world->AddChild<Role>();
	aScene->localTransform.rotation = EulerToQuat(90.f, 0.f, 0.f);
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

