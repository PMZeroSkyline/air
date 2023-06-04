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
    void DrawMVP(const mat4& V, const mat4& P, const vec3& viewPos, const vec3& lightDir)
    {
        shared_ptr<Shader> s = material->shader;
    	s->Use();
    	s->SetMat4("M", *worldMatrix);
    	s->SetMat4("V", V);
    	s->SetMat4("P", P);
        s->SetVec3("viewPos", viewPos);
        s->SetVec3("lightDir", lightDir);
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
        s->SetFloat("isBlend", material->alphaMode == MaterialAlphaMode::BLEND);
        
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

    CameraComponent* playerCamComponent;
    CameraComponent* dirLightCamComponent;

    shared_ptr<MeshPrimitive> quad = MakeQuadMeshPrimitive();
    shared_ptr<Shader> gBufferShader = MakeShaderFromRes("deferred");
    shared_ptr<Shader> lightDepthShader = MakeShaderFromRes("depth");


    GLFrameBuffer gBuffer;
    GLTexture2D gBaseColorRoughness;
    GLTexture2D gPosition;
    GLTexture2D gNormal;
    GLTexture2D gDepth;

    GLFrameBuffer lBuffer;
    GLTexture2D lDepth;

    Actor* cameraActor = nullptr;
    Actor* lightActor = nullptr;
    mat4 cameraV;
    mat4 cameraP;
    mat4 lightV;
    mat4 lightP;
    vec3 viewPos;
    vec3 lightDir;
    int lightDepthSize = 4096;


    Render()
    {
        ivec2 s = window->GetFrameBufferSize();

        gPosition.Image2D(GL_RGBA16F, s.x, s.y, GL_RGBA, GL_FLOAT, NULL);
	    gPosition.Filters(GL_NEAREST, GL_NEAREST);
        gBuffer.Texture2D(GL_COLOR_ATTACHMENT0, gPosition.id);

	    gNormal.Image2D(GL_RGBA16F, s.x, s.y, GL_RGBA, GL_FLOAT, NULL);
	    gNormal.Filters(GL_NEAREST, GL_NEAREST);
	    gBuffer.Texture2D(GL_COLOR_ATTACHMENT1, gNormal.id);

	    gBaseColorRoughness.Image2D(GL_RGBA, s.x, s.y, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        gBaseColorRoughness.Filters(GL_NEAREST, GL_NEAREST);
        gBuffer.Texture2D(GL_COLOR_ATTACHMENT2, gBaseColorRoughness.id);

	    vector<unsigned int> attachments = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
	    gBuffer.DrawBuffers(attachments);

	    gDepth.Image2D(GL_DEPTH_COMPONENT, s.x, s.y, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	    gDepth.Filters(GL_NEAREST, GL_NEAREST);
	    gBuffer.Texture2D(GL_DEPTH_ATTACHMENT, gDepth.id);
        if (!gBuffer.IsComplete())
        {
            LOG("GBuffer not complete !")
        }
        
        lDepth.Image2D(GL_DEPTH_COMPONENT, lightDepthSize, lightDepthSize, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        lDepth.Filters(GL_NEAREST, GL_NEAREST);
        lDepth.WrapST(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
        lDepth.BorderColor(vec4(1.f));
        lBuffer.Texture2D(GL_DEPTH_ATTACHMENT, lDepth.id);
        lBuffer.DrawBuffer(GL_NONE);
        lBuffer.ReadBuffer(GL_NONE);
        if (!lBuffer.IsComplete())
        {
            LOG("shadowBuffer not complete !")
        }

        glContext.BindFrameBuffer();
    }
    void Load(Actor* world)
    {
        opaqueAndMasks.clear();
        blends.clear();
        cameraComponents.clear();
        RenderQuery(world, opaqueAndMasks, blends, cameraComponents);

        for (CameraComponent* cc : cameraComponents)
        {
            if (cc->tag == "role")
            {
                playerCamComponent = cc;
            }
            else if (cc->tag == "dirLight")
            {
                dirLightCamComponent = cc;
            }
        }
        cameraActor = (Actor*)playerCamComponent->owner;
        cameraV = RightHandZUpToYUpProjection() * cameraActor->worldMatrix.inverse();
        cameraP = playerCamComponent->camera->GetProjectioMatrix();

        lightActor = (Actor*)dirLightCamComponent->owner;
        lightV = RightHandZUpToYUpProjection() * lightActor->worldMatrix.inverse();
        lightP = dirLightCamComponent->camera->GetProjectioMatrix();

        viewPos = ToVec3(cameraActor->worldMatrix.column(3));
        lightDir = -lightActor->GetRightVector();
    }
    void DrawLightSpaceDepth()
    {
        lBuffer.Bind();
        glContext.Clear(GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, lightDepthSize, lightDepthSize);

        for (RenderPrimitive& rp : opaqueAndMasks)
    	{
    		rp.DrawMVP(lightV, lightP, viewPos, lightDir);
    	}
        ivec2 s = window->GetFrameBufferSize();
        glViewport(0, 0, s.x, s.y);
    }
    
    void DrawGBuffer()
    {
        gBuffer.Bind();
        glContext.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    	for (RenderPrimitive& rp : opaqueAndMasks)
    	{
    		rp.DrawMVP(cameraV, cameraP, viewPos, lightDir);
    	}
    }
    void DrawDeferredLighting()
    {        
        glContext.BindFrameBuffer();
        glContext.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glContext.SetDepthTest(false);

        gBufferShader->Use();
        gPosition.Active(0);
        gBufferShader->SetInt("gPosition", 0);
        gNormal.Active(1);
        gBufferShader->SetInt("gNormal", 1);
        gBaseColorRoughness.Active(2);
        gBufferShader->SetInt("gBaseColorRoughness", 2);
        gDepth.Active(3);
        gBufferShader->SetInt("gDepth", 3);
        lDepth.Active(4);

        gBufferShader->SetInt("lDepth", 4);
        gBufferShader->SetVec3("viewPos", viewPos);
        gBufferShader->SetVec3("lightDir", lightDir);
        
        mat4 cameraIVP = (cameraP * cameraV).inverse();
        gBufferShader->SetMat4("cameraIVP", cameraIVP);

        mat4 lightVP = lightP * lightV;
        gBufferShader->SetMat4("lightVP", lightVP);
        
        quad->Draw();
    }
    
};

#endif