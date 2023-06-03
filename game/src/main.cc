#include "Gameplay/Object/Role.h"
#include "Gameplay/World/WorldGenerate.h"
#include "Platform/File/FileDirectory.h"
#include "OS/Window/Window.h"
#include "Render/Render/Render.h"


int main()
{
	CDResourcesDir();
	Window window;

	shared_ptr<Actor> world = make_shared<Actor>();
	GenSandbox(world.get());
	world->AddChild<Role>();
	world->ResetWorldMatrix();

	Render render;
	GLResetRenderContext(renderContext);
	while (window.IsOpen())
	{
		window.Tick();
		world->ResetWorldMatrix();
		world->Tick();

		render.Load(world.get());
		render.RenderGBuffer();
		
		if (window.keys[KEY::ESCAPE].pressDown)
		{
			window.Close();
		}
		
		if (window.keys[KEY::F8].pressDown)
		{
			window.SetCursor(!window.GetCursor());
		}
		
		if (window.keys[KEY::F10].pressDown)
		{
			if (window.GetMonitorSize() == window.GetSize())
			{
				window.SetSize(800, 600, true);
			}
			else
			{
				window.SetFullScreen();
			}
		}
	}
	return 0;
}