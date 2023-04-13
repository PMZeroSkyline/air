#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include "TextureSampler.h"
#include "SDK/STL/STL.h"
#include "SDK/OpenGL/CppOpenGL.h"
#include "Image.h"

class Texture2D
{
public:
    string name;
    shared_ptr<TextureSampler> sampler;
    shared_ptr<Image> image;
    GLTexture2D glTexture2D;
    void SetupGLTexture2D()
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
        glTexture2D.Setup(sampler->wrapS, sampler->wrapT, sampler->minFilter, sampler->magFilter, format, image->w, image->h, format, GL_UNSIGNED_BYTE, image->d);
    }
};
#endif