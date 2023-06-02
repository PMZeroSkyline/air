#include "Gameplay/Object/Role.h"
#include "Gameplay/World/WorldGenerate.h"
#include "Platform/File/FileDirectory.h"
#include "OS/Window/Window.h"
#include "Render/Render/Render.h"


int main()
{
	CDResourcesDir();
	Window window;
	renderContext.Reset();

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
		sort(rps.begin(), rps.end(), [&role](const RenderPrimitive& lhs, const RenderPrimitive& rhs){
			float lo = lhs.material->alphaMode == MaterialAlphaMode::BLEND ? distance(role->aCam->worldMatrix.column(3), lhs.worldMatrix->column(3)) : 0.f;
			float ro = rhs.material->alphaMode == MaterialAlphaMode::BLEND ? distance(role->aCam->worldMatrix.column(3), rhs.worldMatrix->column(3)) : 0.f;
			return lo < ro;
		});
		for (RenderPrimitive& rp : rps)
		{
			shared_ptr<Shader> s = rp.material->shader;
			s->Use();
			s->SetMat4("M", *rp.worldMatrix);
			s->SetMat4("V", RightHandZUpToYUpProjection() * role->aCam->worldMatrix.inverse());
			s->SetMat4("P", role->cCam->camera->GetProjectioMatrix());
			s->SetBool("isSkin", rp.armatureInstance);
			if (rp.armatureInstance)
			{
				ArmatureInstance* si = rp.armatureInstance;
				Armature* sk = si->armature;
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
			WindowCursorMode mode = window.GetCursorMode();
			if (mode == WindowCursorMode::CURSOR_DISABLED)
			{
				window.SetCursorMode(WindowCursorMode::CURSOR_NORMAL);
			}
			else
			{
				window.SetCursorMode(WindowCursorMode::CURSOR_DISABLED);
			}
		}
		if (window.keys[KEY::F10].pressDown)
		{
			if (window.GetSize() == ivec2(800, 600))
			{
				window.FullScreen();
			}
			else
			{
				window.SetSize(800, 600);
			}
		}
	}
	return 0;
}