#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include "TextureSampler.h"
#include "SDK/STL/STL.h"
#include "SDK/OpenGL/CppOpenGL.h"
#include "Image.h"
#include "Resource/Container/Blob.h"

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
        GLenum format = GL_RGBA;
        if (image->n == 1)
        {
            format = GL_RED;
        }
        if (image->n == 3)
        {
            format = GL_RGB;
        }
        glTexture2D.Bind();
        glTexture2D.SetupWrapST(sampler.wrapS, sampler.wrapT);
        glTexture2D.SetupFilter(sampler.minFilter, sampler.magFilter);
        glTexture2D.SetupPixels(format, image->w, image->h, format, GL_UNSIGNED_BYTE, image->d);
        glTexture2D.GenMipmap();
    }
};

Blob<Texture2D> texture2DBlob;

#endif