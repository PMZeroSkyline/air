#include "Gameplay/Object/Role.h"
#include "Gameplay/World/WorldGenerate.h"
#include "Platform/File/FileDirectory.h"
#include "OS/Window/Window.h"
#include "Render/Render/Render.h"
#include "Resource/Texture/Image.h"



int main(int argc, char** argv)
{
	// EXEC_PO2

	WorkspaceCurrentPath("air", "game/res");

	Image img;
	img.Load("Texture/HDRSpheres/TexturesCom_AgbarPlatform_1K_hdri_sphere_tone.jpg");
	
	// vector<string> paths;
	// GetFolderFiles("Texture", paths, true);
	// for (auto path : paths)
	// {
	// 	if (path.find(".DS_Store") != string::npos)
	// 	{
	// 		continue;
	// 	}
	// 	Image img;
	// 	img.Load(path);
	// 	if (img.UpSizeToPowerOfTwo())
	// 	{
	// 		img.Save(path);
	// 	}
	// }

	return 0;

	Window window;

	shared_ptr<Actor> world = make_shared<Actor>();
	GenSandbox(world.get());
	Role* role = world->AddChild<Role>();
	GenDirLight(role);
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

