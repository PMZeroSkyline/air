#include <iostream>
#include "Window.h"
#include "Image.h"
#include "GL.h"
#include "Scenes.h"
#include "Renderer.h"

int main()
{
	CDResourcesDir();

	Window window;
	Renderer renderer;
	
	shared_ptr<Actor> root = make_shared<Actor>();
	ScenesComponent *sc = root->AddComponent<ScenesComponent>();
	sc->Load("sushi_bar/scene.gltf");
	sc->FieldsExpand();
	
	renderer.SetupMaterial();
	renderer.SetupActors(nullptr, root.get());

	while (window.IsOpen())
	{
		window.Tick();
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		renderer.Render(root.get());
		

		if (window.keys[GLFW_KEY_ESCAPE].pressDown)
		{
			window.Close();
		}
	}

	return 0;
}