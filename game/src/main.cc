#include "Gameplay/Object/Role.h"
#include "Gameplay/World/WorldGenerate.h"
#include "Platform/File/FileDirectory.h"
#include "OS/Window/Window.h"
#include "Render/Render/Render.h"
#include "Resource/Texture/Image.h"

int main()
{
	CDResourcesDir();

	Image img;
	img.Load("Texture/HDRSphere/TexturesCom_JapanKabuchiko_1K_hdri_sphere_tone.jpg");
	LOG(img.channel);

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
		render.DrawLightSpaceDepth();
		render.DrawGBuffer();
		render.DrawDeferredShading();
		
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

