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
	Role* role = world->AddChild<Role>();
	
	world->ResetWorldMatrix();
	while (window.IsOpen())
	{
		window.Tick();
		world->ResetWorldMatrix();
		world->Tick();

		GLClear();	
		vector<RenderPrimitive> rps = CollectionRenderPrimitives(world.get());
		for (RenderPrimitive& rp : rps)
		{
			shared_ptr<Shader> s = rp.material->shader;
			s->Use();
			s->SetMat4("M", *rp.worldMatrix);
			s->SetMat4("V", RightHandZUpToYUpProjection() * role->aCam->worldMatrix.inverse());
			s->SetMat4("P", role->cCam->camera->GetProjectioMatrix());
			s->SetBool("isSkin", rp.skinInstance);
			if (rp.skinInstance)
			{
				SkinInstance* si = rp.skinInstance;
				Skin* sk = si->skin;
				vector<Actor*> js = si->joints;
				mat4 IW = rp.parentWorldMatrix ? rp.parentWorldMatrix->inverse() : mat4();
				vector<mat4> jms;
				for (int i = 0; i < js.size(); i++)
				{
					Actor* j = js[i];
					mat4 JM = IW * j->worldMatrix * sk->inverseBindMatrices[i];
					s->SetMat4("J[" + to_string(i) + "]", JM);
				}
			}
			rp.material->ResetRenderContext();
			rp.meshPrimitive->Draw();
		}
		

		

		if (window.keys[KEY::ESCAPE].pressDown)
		{
			window.Close();
		}
		if (window.keys[KEY::F8].pressDown)
		{
			int mode = glfwGetInputMode(window.glfw_window, GLFW_CURSOR);
			if (mode == GLFW_CURSOR_DISABLED)
			{
				glfwSetInputMode(window.glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			else
			{
				glfwSetInputMode(window.glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
		}
	}
	return 0;
}