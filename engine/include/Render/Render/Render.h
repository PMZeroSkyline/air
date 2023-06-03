#ifndef RENDER_H
#define RENDER_H

#include "Gameplay/Object/Actor.h"
#include "Gameplay/Component/MeshComponent.h"

class RenderPrimitive
{
public:
    mat4* parentWorldMatrix = nullptr;
    mat4* worldMatrix = nullptr;
    MeshPrimitive* meshPrimitive = nullptr;
    ArmatureInstance* armatureInstance = nullptr;
    Material* material = nullptr;
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
    vector<RenderPrimitive> opaqueAndMasks, blends;
    vector<CameraComponent*> cameraComponents;
    GLFrameBuffer gBuffer;
    GLTexture2D gColor;
    GLTexture2D gPosition;
    GLTexture2D gNormal;
    Render()
    {
        // gPosition.Bind();
        // gPosition.Image2D()
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
        // depthTex.Bind();
        // glDrawBuffer(GL_NONE);
        // glReadBuffer(GL_NONE);
        // GLBindFrameBuffer();
        // glViewport(0, 0, 1024, 1024);
        // depthAttachment.Bind();
    }
    void RenderGBuffer()
    {
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
        GLClear(GLMask::COLOR_BUFFER_BIT | GLMask::DEPTH_BUFFER_BIT);

    	for (RenderPrimitive& rp : opaqueAndMasks)
    	{
    		shared_ptr<Shader> s = rp.material->shader;
    		s->Use();
    		s->SetMat4("M", *rp.worldMatrix);
    		s->SetMat4("V", V);
    		s->SetMat4("P", P);
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
    }
};

#endif