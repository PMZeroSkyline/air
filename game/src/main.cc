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

	//Render render;
	GLFrameBuffer fbo;
    GLTexture2D col;
    GLRenderBuffer rbo;
	fbo.Bind();
    col.Bind();
    col.Image2D(GL_RGBA, window.GetSize().x, window.GetSize().y, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    col.Filters(GL_LINEAR, GL_LINEAR);
    fbo.Texture2D(GL_COLOR_ATTACHMENT0, col.id);

    rbo.Bind();
    fbo.Bind();
    rbo.SetStorage(GL_DEPTH24_STENCIL8, window.GetSize().x, window.GetSize().y);
    fbo.Renderbuffer(GL_DEPTH_STENCIL_ATTACHMENT, rbo.id);
	
	while (window.IsOpen())
	{
		window.Tick();
		world->ResetWorldMatrix();
		world->Tick();

		//render.Load(world.get());
		//render.RenderGBuffer();

		vector<RenderPrimitive> opaqueAndMasks, blends;
    	vector<CameraComponent*> cameraComponents;
		RenderQuery(world.get(), opaqueAndMasks, blends, cameraComponents);

		mat4 V, P;
        if (cameraComponents.size() && cameraComponents[0]->owner)
        {
            Actor* cameraActor = (Actor*)cameraComponents[0]->owner;
            V = RightHandZUpToYUpProjection() * cameraActor->worldMatrix.inverse();
            P =  cameraComponents[0]->camera->GetProjectioMatrix();
        }
        else
        {
            V = RightHandZUpToYUpProjection();
            P = PerspectiveCamera().GetProjectioMatrix();
        }
        fbo.Bind();
        glContext.Clear(GLMask::COLOR_BUFFER_BIT | GLMask::DEPTH_BUFFER_BIT | GLMask::STENCIL_BUFFER_BIT);
    	for (RenderPrimitive& rp : opaqueAndMasks)
    	{
    		rp.DrawMVP(V, P);
    	}
        for (RenderPrimitive& rp : blends)
    	{
    		rp.DrawMVP(V, P);
    	}
        glContext.BindFrameBuffer();
        glContext.Clear(GLMask::COLOR_BUFFER_BIT | GLMask::DEPTH_BUFFER_BIT | GLMask::STENCIL_BUFFER_BIT);
        glContext.SetDepthTest(false);
        shared_ptr<MeshPrimitive> quad = MakeQuadMeshPrimitive(MakeMaterialFromShaderRes("screen"));
        quad->material->shader->Use();
        col.Bind();
        col.Active(0);
        quad->material->shader->SetInt("screenTex", 0);
        quad->Draw();
		
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