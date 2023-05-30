#ifndef RENDER_TEXTURE_H
#define RENDER_TEXTURE_H

#include "Core/Container/SharedMap.h"
#include "SDK/OpenGL/CppOpenGL.h"

SharedMap<GLTexture2D> glTexture2DSharedMap;

shared_ptr<GLTexture2D> MakeShadowGLTexture2D(int x, int y)
{
    shared_ptr<GLTexture2D> t = make_shared<GLTexture2D>();
	t->Bind();
	t->SetupPixels(GLTexParam::DEPTH_COMPONENT, x, y, GLTexParam::DEPTH_COMPONENT, GLTexParam::FLOAT, NULL);
	t->SetupFilters(GLTexParam::LINEAR, GLTexParam::NEAREST);
	t->SetupWrapST(GLTexParam::REPEAT, GLTexParam::REPEAT);
    return t;
}

shared_ptr<GLTexture2D> ShadowGLTexture2DFromSharedMap(int x, int y)
{
    string key = "shadow_" + to_string(x) + "_" + to_string(y);
    shared_ptr<GLTexture2D> t = glTexture2DSharedMap.Get(key);
    if (t)
    {
        return t;
    }
    t = MakeShadowGLTexture2D(x, y);
    glTexture2DSharedMap.Set(key, t);
    return t;
}

class ShadowRenderTexture : public GLTexture2D
{
public:
    int x, y;
    ShadowRenderTexture() : x(0), y(0) {}
    ShadowRenderTexture(int _x, int _y) : x(_x), y(_y)
    {
        Setup();
    }
    void Setup()
    {
        Bind();
	    SetupPixels(GLTexParam::DEPTH_COMPONENT, x, y, GLTexParam::DEPTH_COMPONENT, GLTexParam::FLOAT, NULL);
	    SetupFilters(GLTexParam::LINEAR, GLTexParam::NEAREST);
	    SetupWrapST(GLTexParam::REPEAT, GLTexParam::REPEAT);
    }
    void Use()
    {
        glViewport(0, 0, x, y);
    }
};



#endif