#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"
#include "Resource/Texture/Texture2D.h"
#include "Render/Render/RenderContext.h"

enum MaterialAlphaMode
{
    OPAQUE,
    MASK,
    BLEND
};

class Material
{
public:
    string name;
    shared_ptr<Shader> shader;

    // Params
    float alphaCutoff = 0.5f;
    map<string, GLTexture2D*> textureMap;
    map<string, mat4> matrixMap;
    map<string, vector<mat4>> matricesMap;

    // Render Context
    MaterialAlphaMode alphaMode = MaterialAlphaMode::OPAQUE;
    GLenum sfactor = GLBlendFactor::SRC_ALPHA, dfactor = GLBlendFactor::ONE_MINUS_SRC_ALPHA;
    bool depthTest = true;
    bool depthMask = true;
    bool doubleSided = false;
    GLenum faceMode = GLPolygonMode::FILL;

    void ResetRenderContext()
    {
        glContext.SetBlend(alphaMode == MaterialAlphaMode::BLEND);
        glContext.SetBlendFunc(sfactor, dfactor);
        glContext.SetCullFace(!doubleSided);
        glContext.SetDepthMask(depthMask);
        glContext.SetDepthTest(depthTest);
        glContext.SetPolygon(faceMode);
    }
    void SetUniform()
    {        
        // for (auto const& pair : matricesMap)
        // {
        //     for (int i = 0; i < pair.second.size(); i++)
        //     {
        //         shader->SetMat4(pair.first + "[" + to_string(i) + "]" , pair.second[i]);
        //     }            
        // }
        // for (auto const& pair : matrixMap)
        // {
        //     shader->SetMat4(pair.first, pair.second);
        // }

        int textureIndex = 0;
        for (auto const& pair : textureMap)
        {
            pair.second->Active(textureIndex);
            pair.second->Bind();
            shader->SetInt(pair.first, textureIndex);
            textureIndex++;
        }
        shader->SetBool("isUseTex", textureMap.size() != 0);
        
    }
};

shared_ptr<Material> MakeMaterialFromShaderRes(const string& name)
{
    shared_ptr<Material> material = make_shared<Material>();
    material->shader = MakeShaderFromRes(name);
    return material;
}
#endif