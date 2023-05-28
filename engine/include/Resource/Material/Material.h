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
    float alphaCutoff = 0.5f;
    MaterialAlphaMode alphaMode = MaterialAlphaMode::OPAQUE;
    bool doubleSided = false;
    GLFaceMode faceMode = GLFaceMode::FILL;

    vector<pair<string, shared_ptr<Texture2D>>> texturePairs;
    map<string, mat4> mat4Map;
    map<string, mat4*> mat4PtrMap;
    vector<mat4> jointMatrix;

    
    void Use()
    {
        // Setup Render Context
        bool isCullFace = !doubleSided;
        if (renderContext.isCullFace != isCullFace)
        {
            GLSetCullFace(isCullFace);
            renderContext.isCullFace = isCullFace;
        }
        bool isBlend = alphaMode == MaterialAlphaMode::BLEND;
        if (renderContext.isBlend != isBlend)
        {
            GLSetBlend(isBlend);
            renderContext.isBlend = isBlend;
        }
        if (renderContext.frontAndBackFaceMode != faceMode)
        {
            GLSetFaceMode(faceMode);
            renderContext.frontAndBackFaceMode = faceMode;
        }
        
        
        // Set Shader
        shader->Use();
        for (auto it = mat4PtrMap.begin(); it != mat4PtrMap.end(); it++)
        {
           shader->SetMat4(it->first, *it->second);
        }
        shader->SetBool("isSkin", jointMatrix.size() != 0);
        for (int i = 0; i < jointMatrix.size(); i++)
        {
            shader->SetMat4("J[" + to_string(i) + "]", jointMatrix[i]);
        }
        for (int i = 0; i < texturePairs.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, texturePairs[i].second->glTexture2D.id);
            shader->SetInt(texturePairs[i].first, i);
        }
        shader->SetBool("isUseTexture", texturePairs.size() != 0);
    }
};

#endif