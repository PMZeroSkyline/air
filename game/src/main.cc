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
	cScenes->Load("game/res/Model/Architecture/Warzone/kb3d_warzone-native.gltf");
	cScenes->FieldExpand();
	aScene->isTick = false;

	Role* role = world->AddChild<Role>();
	world->ResetWorldMatrix();

	Render render;
	render.Load(world.get());
	
	while (window.IsOpen())
	{
		window.Tick();
		render.Draw();
		world->ResetWorldMatrix();
		world->Tick();
		
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

