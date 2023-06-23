#include "Gameplay/Object/Role.h"
#include "Gameplay/World/WorldGenerate.h"
#include "Platform/File/FileDirectory.h"
#include "OS/Window/Window.h"
#include "Render/Render/Render.h"
#include "Resource/Texture/Image.h"



int main(int argc, char** argv)
{
	// EXEC_PO2
	CDWorkspacePath("air");

	// Image img;
	// img.Load("Texture/Anim.png");
	// int w, h;
	// img.GetCeilingPowerOfTwoSize(w, h);
	// img.Resize(w, h);
	// img.Save("Texture/Anim1.png");
	// return 0;

	Window window;

	shared_ptr<Actor> world = make_shared<Actor>();
	// GenSandbox(world.get());
	Actor* aScene = world->AddChild<Actor>();
	ScenesComponent* cScenes = aScene->AddComponent<ScenesComponent>();
	cScenes->Load("game/res/Model/Architecture/LightRoom/LightRoom.gltf");
	cScenes->FieldExpand();

	world->AddChild<Role>();
	world->ResetWorldMatrix();

	while (window.IsOpen())
	{
		window.Tick();
		world->ResetWorldMatrix();
		world->Tick();

		Render render;
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

