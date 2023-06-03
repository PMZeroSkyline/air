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
        GLenum format = GLFormat::RGBA;
        if (image->n == 1)
        {
            format = GLFormat::RED;
        }
        if (image->n == 3)
        {
            format = GLFormat::RGB;
        }
        glTexture2D.Bind();
        glTexture2D.WrapST(sampler.wrapS, sampler.wrapT);
        glTexture2D.Filters(sampler.minFilter, sampler.magFilter);
        glTexture2D.Image2D(format, image->w, image->h, format, GLType::UBYTE, image->d);
        glTexture2D.GenMipmap();
    }
};

WeakMap<Texture2D> texture2DWeakMap;

#endif