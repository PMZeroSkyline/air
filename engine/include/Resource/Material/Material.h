#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"
#include "Resource/Texture/Texture2D.h"
#include "SDK/OpenGL/CppOpenGL.h"

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
    map<string, GLTexture*> textureMap;
    map<string, mat4> matrixMap;
    map<string, vector<mat4>> matricesMap;

    // Render Context
    MaterialAlphaMode alphaMode = MaterialAlphaMode::OPAQUE;
    GLenum sfactor = GL_SRC_ALPHA, dfactor = GL_ONE_MINUS_SRC_ALPHA;
    bool depthTest = true;
    bool depthMask = true;
    bool doubleSided = false;
    GLenum faceMode = GL_FILL;

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
        int textureUnit = 0;
        for (auto const& pair : textureMap)
        {
            pair.second->ActiveTextureUnit(textureUnit);
            shader->SetInt(pair.first, textureUnit);
            textureUnit++;
        }
        shader->SetInt("bMask", alphaMode == MaterialAlphaMode::MASK);
        
        
    }
};
#endif