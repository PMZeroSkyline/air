#include "Gameplay/Object/Role.h"
#include "Gameplay/World/WorldGenerate.h"
#include "Platform/File/FileDirectory.h"
#include "OS/Window/Window.h"
#include "Render/Render/Render.h"


int main()
{
	CDResourcesDir();
	Window window;

	shared_ptr<MeshPrimitive> quad = MakeCubeMeshPrimitive("default");
	shared_ptr<Actor> world = make_shared<Actor>();
	GenSandbox(world.get());
	Role* role = world->AddChild<Role>();
	//shared_ptr<MeshPrimitive> cube = MakeQuadMeshPrimitive("screen");

	// Render
	// GLFrameBuffer depthFBO;
	// depthFBO.Bind();
	// GLTexture2D depthTex;
	// depthTex.Bind();
	// depthTex.SetupPixels(GLTexParam::DEPTH_COMPONENT, 1024, 1024, GLTexParam::DEPTH_COMPONENT, GLTexParam::FLOAT, NULL);
	// //depthTex.SetupPixels(GLTexParam::DEPTH_COMPONENT, window.GetSize().x, window.GetSize().y, GLTexParam::DEPTH_COMPONENT, GLTexParam::FLOAT, NULL);
	// depthTex.SetupFilters(GLTexParam::LINEAR, GLTexParam::NEAREST);
	// depthTex.SetupWrapST(GLTexParam::REPEAT, GLTexParam::REPEAT);
	// depthFBO.SetAttachmentTexture2D(GLFrameBufferParam::DEPTH_ATTACHMENT, depthTex.id);
	// glDrawBuffer(GL_NONE);
	// glReadBuffer(GL_NONE);
	// glViewport(0, 0, 1024, 1024);
	
	// Init
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
			s = MakeShaderFromRes("default");
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