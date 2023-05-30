#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include "TextureSampler.h"
#include "SDK/STL/STL.h"
#include "SDK/OpenGL/CppOpenGL.h"
#include "Image.h"
#include "Core/Container/WeakMap.h"

class Texture2D
{
public:
    string name;
    TextureSampler sampler;
    shared_ptr<Image> image;
    GLTexture2D glTexture2D;
    void SetupGLTexture2D(bool isGenMipmap = true)
    {
        LOG("SetupGLTexture2D " << name)
        GLTexParam format = GLTexParam::RGBA;
        if (image->n == 1)
        {
            format = GLTexParam::RED;
        }
        if (image->n == 3)
        {
            format = GLTexParam::RGB;
        }
        glTexture2D.Bind();
        glTexture2D.SetupWrapST(sampler.wrapS, sampler.wrapT);
        glTexture2D.SetupFilters(sampler.minFilter, sampler.magFilter);
        glTexture2D.SetupPixels(format, image->w, image->h, format, GLTexParam::UBYTE, image->d);
        glTexture2D.GenMipmap();
    }
};

WeakMap<Texture2D> texture2DWeakMap;

#endif