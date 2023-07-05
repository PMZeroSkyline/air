#ifndef RENDER_H
#define RENDER_H

#include "Gameplay/Object/Actor.h"
#include "Gameplay/Component/MeshComponent.h"
#include "Gameplay/Component/CameraComponent.h"
#include "Gameplay/Component/SkinComponent.h"
#include "OS/Window/Window.h"

#include "Resource/Texture/TextureCube.h"


class RenderPrimitive
{
public:
    mat4* parentWorldMatrix = nullptr;
    mat4* worldMatrix = nullptr;
    MeshPrimitive* meshPrimitive = nullptr;
    SkinInstance* skinInstance = nullptr;
    Material* material = nullptr;
};
void RenderQuery(Actor* root, map<MaterialAlphaMode, vector<RenderPrimitive>>& renderPrimitiveMap, CameraComponent*& cCamera)
{
    root->RForEachNode<Actor>([&renderPrimitiveMap, &cCamera](Actor* actor){
        MeshComponent* mc = actor->GetComponent<MeshComponent>();
        if (mc)
        {
            SkinComponent* sc = actor->GetComponent<SkinComponent>();
            Actor* owner = (Actor*)mc->owner;
            Actor* parent = owner ? ((Actor*)owner->parent) : nullptr;
            for (shared_ptr<MeshPrimitive> meshPrimitive : mc->mesh->primitives)
            {
                RenderPrimitive rp;
                rp.meshPrimitive = meshPrimitive.get();
                rp.material = meshPrimitive->material.get();
                rp.skinInstance = sc ? sc->skinInstance : nullptr;
                if (owner)
                {
                    rp.worldMatrix = &owner->worldMatrix;
                }
                if (mc->owner->parent)
                {
                    rp.parentWorldMatrix = &parent->worldMatrix;
                }
                renderPrimitiveMap[rp.material->alphaMode].push_back(rp);
            }
        }
        if (!cCamera)
        {
            CameraComponent* cameraComponent = actor->GetComponent<CameraComponent>();
            if (cameraComponent)
            {
                cCamera = cameraComponent;
            }
        }
    });
}
void SetSkinUniform(shared_ptr<Shader> shader, SkinInstance* skinInstance, mat4* parentWorldMatrix)
    {
        shader->SetBool("isSkin", skinInstance ? true : false);
        if (skinInstance)
        {
        	Skin* skin = skinInstance->skin;
        	vector<Actor*> bones = skinInstance->joints;
        	mat4 IW = parentWorldMatrix ? parentWorldMatrix->inverse() : mat4();
        	vector<mat4> jms;
        	for (int i = 0; i < bones.size(); i++)
        	{
        		Actor* bone = bones[i];
        		mat4 B = IW * bone->worldMatrix * skin->inverseBindMatrices[i];
        		shader->SetMat4("J[" + to_string(i) + "]", B);
        	}
        }
    }
class Render
{
public:
    map<MaterialAlphaMode, vector<RenderPrimitive>> renderPrimitiveMap;

    Window* window = GetCurrentWindowContext();
    ivec2 size = window->GetFrameBufferSize();

    GLFrameBuffer gBuffer;
    shared_ptr<GLTexture2D> tColor = MakeTexture2D(size.x, size.y, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
    shared_ptr<GLTexture2D> tPosition = MakeTexture2D(size.x, size.y, GL_RGBA16F, GL_RGBA, GL_FLOAT);
    shared_ptr<GLTexture2D> tNormal = MakeTexture2D(size.x, size.y, GL_RGBA16F, GL_RGBA, GL_FLOAT);
    shared_ptr<GLTexture> tDepth = MakeTexture2D(size.x, size.y, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);

    int shadowSize = 2048;
    GLFrameBuffer gShadow;
    shared_ptr<GLTexture2D> tShadow = MakeTexture2D(shadowSize, shadowSize, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
    shared_ptr<Material> mShadow = MakeMaterial(MakeShader());

    bool isSoftSSSM = false;
    GLFrameBuffer gSSSM;
    shared_ptr<GLTexture2D> tSSSM =  MakeTexture2D(size.x, size.y, GL_RED, GL_RED, GL_UNSIGNED_BYTE);
    vector<vec3> randPoints = RandomUnitSphere(64);

    mat4 V;
    mat4 P;
    vec3 viewPos;
    CameraComponent* cCamera = nullptr;
    
    mat4 lV;
    mat4 lP;
    vec3 lightDir;
    shared_ptr<Actor> aLight = make_shared<Actor>();
    CameraComponent* cLight = aLight->AddComponent<CameraComponent>();
    
    shared_ptr<MeshPrimitive> quad = MakeQuadMeshPrimitive();
    shared_ptr<Material> mSSSM = MakeMaterial(MakeShader("sssm"));
    shared_ptr<Material> mScreen = MakeMaterial(MakeShader("screen"));

    Render()
    {
        gBuffer.Texture2D(GL_DEPTH_ATTACHMENT, tDepth->id);
        gBuffer.Texture2D(GL_COLOR_ATTACHMENT0, tPosition->id);
        gBuffer.Texture2D(GL_COLOR_ATTACHMENT1, tNormal->id);
        gBuffer.Texture2D(GL_COLOR_ATTACHMENT2, tColor->id);
        gBuffer.DrawColorBuffers(3);

        
        tShadow->WrapST(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        tShadow->BorderColor(borderColor);
        gShadow.Texture2D(GL_DEPTH_ATTACHMENT, tShadow->id);
        gShadow.DrawBuffer(GL_NONE);
        gShadow.ReadBuffer(GL_NONE);

        aLight->localTransform.translation = vec3(0.f, 0.f, 25.f);
        aLight->localTransform.rotation = EulerToQuat(vec3(-85.f, 0.f, 4.f));
        aLight->ResetWorldMatrix();
        shared_ptr<OrthographicCamera> shadowCamera = make_shared<OrthographicCamera>();
        shadowCamera->zfar = 50.f;
        shadowCamera->xmag = 25.f;
        shadowCamera->ymag = 25.f;
        cLight->camera = shadowCamera;
    }
    void Load(Actor* root)
    {
        cCamera = nullptr;
        renderPrimitiveMap.clear();
        RenderQuery(root, renderPrimitiveMap, cCamera);
        if (window->keys[KEY::J].pressDown)
        {
            isSoftSSSM = !isSoftSSSM;
        }
    }
    void Draw()
    {
        
        Actor* aCamera = ((Actor*)cCamera->owner);
        V = RightHandZUpToYUpProjection() * aCamera->worldMatrix.inverse();
        P = cCamera->camera->GetProjectioMatrix();
        viewPos = ToVec3(aCamera->worldMatrix.column(3));

        Actor* aLight = ((Actor*)cLight->owner);
        lV = RightHandZUpToYUpProjection() * aLight->worldMatrix.inverse();
        lP = cLight->camera->GetProjectioMatrix();
        lightDir = -aLight->GetRightVector();

        glContext.Viewport(0, 0, shadowSize, shadowSize);
        gShadow.Clear(GL_DEPTH_BUFFER_BIT);
        glContext.SetCullFace(true);
        glContext.CullFace(GL_FRONT);
        for (auto& rp : renderPrimitiveMap[MaterialAlphaMode::OPAQUE])
        {
            mShadow->SetTransformationUniform(*rp.worldMatrix, lV,lP);
            SetSkinUniform(mShadow->shader, rp.skinInstance, rp.parentWorldMatrix);
            mShadow->SetMaterialAlphaModeUniform();
            rp.meshPrimitive->Draw();
        }
        for (auto& rp : renderPrimitiveMap[MaterialAlphaMode::MASK])
        {
            mShadow->SetTransformationUniform(*rp.worldMatrix, lV, lP);
            SetSkinUniform(mShadow->shader, rp.skinInstance, rp.parentWorldMatrix);
            mShadow->SetMaterialAlphaModeUniform();
            rp.meshPrimitive->Draw();
        }
        glContext.CullFace(GL_BACK);
        glContext.Viewport(0, 0, size.x, size.y);
        
        gBuffer.ClearColor(0.f, 0.f, 0.f, 0.f);
        gBuffer.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    	for (auto& rp : renderPrimitiveMap[MaterialAlphaMode::OPAQUE])
        {
            rp.material->SetTransformationUniform(*rp.worldMatrix, V, P);
            rp.material->SetMaterialAlphaModeUniform();
            SetSkinUniform(rp.material->shader, rp.skinInstance, rp.parentWorldMatrix);
            rp.material->SetTexturesUniform();
            rp.material->ResetRenderContext();
            rp.meshPrimitive->Draw();
        }
        for (auto& rp : renderPrimitiveMap[MaterialAlphaMode::MASK])
        {
            rp.material->SetTransformationUniform(*rp.worldMatrix, V, P);
            rp.material->SetMaterialAlphaModeUniform();
            SetSkinUniform(rp.material->shader, rp.skinInstance, rp.parentWorldMatrix);
            rp.material->SetTexturesUniform();
            rp.material->ResetRenderContext();
            rp.meshPrimitive->Draw();
        }


        glContext.ClearColor(0.f, 0.f, 0.f, 0.f);
        glContext.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mScreen->textureMap["tC"] = tColor.get();
        mScreen->textureMap["tP"] = tPosition.get();
        mScreen->textureMap["tN"] = tNormal.get();
        mScreen->textureMap["tS"] = tShadow.get();
        mScreen->textureMap["tD"] = tDepth.get();
        mScreen->vec3sMap["randPoints"] = &randPoints;
        mScreen->shader->SetMat4("lVP", lP * lV);
        mScreen->shader->SetVec3("lightDir", lightDir);
        mScreen->shader->SetBool("isUseZeroSkylineVolumeShadow", isSoftSSSM);
        mScreen->SetTexturesUniform();
        mScreen->SetVec3sUniform();
        quad->Draw();
        
        // glContext.ClearColor(0.f, 0.f, 0.f, 0.f);
        // glContext.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // quad->material->textureMap["tB"] = tColor.get();
        // quad->material->textureMap["tP"] = tPosition.get();
        // quad->material->textureMap["tN"] = tNormal.get();
        // quad->material->shader->Use();
        // quad->material->SetTexturesUniform();
        // quad->Draw();
    }
    
};

#endif