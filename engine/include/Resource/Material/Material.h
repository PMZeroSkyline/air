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
    map<string, vector<vec3>*> vec3sMap;

    // Render Context
    MaterialAlphaMode alphaMode = MaterialAlphaMode::OPAQUE;
    GLenum sfactor = GL_SRC_ALPHA, dfactor = GL_ONE_MINUS_SRC_ALPHA;
    bool isDepthTest = true;
    bool isDepthMask = true;
    bool doubleSided = false;
    GLenum cullFaceMode = GL_BACK;
    GLenum faceMode = GL_FILL;

    void ResetRenderContext()
    {
        glContext.SetBlend(alphaMode == MaterialAlphaMode::BLEND);
        glContext.BlendFunc(sfactor, dfactor);
        glContext.SetCullFace(!doubleSided);
        glContext.DepthMask(isDepthMask);
        glContext.SetDepthTest(isDepthTest);
        glContext.Polygon(faceMode);
        glContext.CullFace(cullFaceMode);
    }
    void SetTexturesUniform()
    {        
        int textureUnit = 0;
        for (auto const& pair : textureMap)
        {
            pair.second->ActiveTextureUnit(textureUnit);
            shader->SetInt(pair.first, textureUnit);
            textureUnit++;
        }
    }
    void SetTransformationUniform(const mat4& M, const mat4& V, const mat4& P)
    {
        shader->SetMat4("M", M);
        shader->SetMat4("V", V);
        shader->SetMat4("P", P);
    }
    void SetMaterialAlphaModeUniform()
    {
        shader->SetInt("isMask", alphaMode == MaterialAlphaMode::MASK);
    }
    void SetVec3sUniform()
    {
        for (const auto& pair : vec3sMap)
        {
            for (int i = 0; i < pair.second->size(); i++)
            {
                shader->SetVec3(pair.first+"["+to_string(i)+"]", (*pair.second)[i]);
            }
        }
    }
};

shared_ptr<Material> MakeMaterial(shared_ptr<Shader> shader)
{
    shared_ptr<Material> material = make_shared<Material>();
    material->shader = shader;
    return material;
}
#endif