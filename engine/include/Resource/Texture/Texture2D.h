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
        GLenum format = GL_RGBA;
        if (image->channel == 1)
        {
            format = GL_RED;
        }
        if (image->channel == 3)
        {
            format = GL_RGB;
        }
        //glTexture2D.Bind();
        glTexture2D.WrapST(sampler.wrapS, sampler.wrapT);
        glTexture2D.Filters(sampler.minFilter, sampler.magFilter);
        glTexture2D.Image2D(format, image->width, image->hight, format, GL_UNSIGNED_BYTE, image->data);
        glTexture2D.GenMipmap();
    }
};

WeakMap<Texture2D> texture2DWeakMap;

#endif