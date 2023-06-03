#ifndef RENDER_H
#define RENDER_H

#include "Gameplay/Object/Actor.h"
#include "Gameplay/Component/MeshComponent.h"
#include "Gameplay/Component/CameraComponent.h"
#include "OS/Window/Window.h"

class RenderPrimitive
{
public:
    mat4* parentWorldMatrix = nullptr;
    mat4* worldMatrix = nullptr;
    MeshPrimitive* meshPrimitive = nullptr;
    ArmatureInstance* armatureInstance = nullptr;
    Material* material = nullptr;

    void DrawMVP(const mat4& V, const mat4& P)
    {
        shared_ptr<Shader> s = material->shader;
    	s->Use();
    	s->SetMat4("M", *worldMatrix);
    	s->SetMat4("V", V);
    	s->SetMat4("P", P);
    	s->SetBool("isSkin", armatureInstance);
    	if (armatureInstance)
    	{
    		Armature* armature = armatureInstance->armature;
    		vector<Actor*> bones = armatureInstance->joints;
    		mat4 IW = parentWorldMatrix ? parentWorldMatrix->inverse() : mat4();
    		vector<mat4> jms;
    		for (int i = 0; i < bones.size(); i++)
    		{
    			Actor* bone = bones[i];
    			mat4 B = IW * bone->worldMatrix * armature->inverseBindMatrices[i];
    			s->SetMat4("J[" + to_string(i) + "]", B);
    		}
    	}
        material->SetUniform();
    	material->ResetRenderContext();
    	meshPrimitive->Draw();
    }
};

void RenderQuery(Actor* root, vector<RenderPrimitive>& opaqueAndMasks, vector<RenderPrimitive>& blends, vector<CameraComponent*>& cameraComponents)
{
    root->RForEachNode<Actor>([&opaqueAndMasks, &blends, &cameraComponents](Actor* actor){
        vector<MeshComponent*> mcs;
        actor->GetComponents<MeshComponent>(mcs);
        for (MeshComponent* mc : mcs)
        {
            if (mc->mesh)
            {
                for (shared_ptr<MeshPrimitive> meshPrimitive : mc->mesh->primitives)
                {
                    RenderPrimitive rp;
                    rp.meshPrimitive = meshPrimitive.get();
                    rp.material = meshPrimitive->material.get();
                    rp.armatureInstance = mc->armatureInstance;
                    if (mc->owner)
                    {
                        rp.worldMatrix = &((Actor*)mc->owner)->worldMatrix;
                    }
                    if (mc->owner->parent)
                    {
                        rp.parentWorldMatrix = &((Actor*)mc->owner->parent)->worldMatrix;
                    }
                    if (rp.material->alphaMode == MaterialAlphaMode::BLEND)
                    {
                        blends.push_back(rp);
                    }
                    else
                    {
                        opaqueAndMasks.push_back(rp);
                    }
                }
            }
        }
        actor->GetComponents<CameraComponent>(cameraComponents);
    });
}
class Render
{
public:
    Window* window = GetCurrentWindowContext();
    vector<RenderPrimitive> opaqueAndMasks, blends;
    vector<CameraComponent*> cameraComponents;

    GLFrameBuffer fbo;
    GLTexture2D col;
    GLRenderBuffer rbo;
    Render()
    {
        ivec2 s = window->GetSize();

        fbo.Bind();
        col.Bind();
        col.Image2D(GL_RGBA, s.x, s.y, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        col.Filters(GL_LINEAR, GL_LINEAR);
        fbo.Texture2D(GL_COLOR_ATTACHMENT0, col.id);

        rbo.Bind();
        fbo.Bind();
        rbo.SetStorage(GL_DEPTH24_STENCIL8, s.x, s.y);
        fbo.Renderbuffer(GL_DEPTH_STENCIL_ATTACHMENT, rbo.id);

        LOG("fbo: " << fbo.IsComplete())
    }
    void Load(Actor* world)
    {
        opaqueAndMasks.clear();
        blends.clear();
        cameraComponents.clear();
        RenderQuery(world, opaqueAndMasks, blends, cameraComponents);
    }
    void RenderShadow()
    {        
    }

    void RenderGBuffer()
    {
        
    }
};

#endif