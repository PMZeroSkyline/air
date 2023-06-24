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
    void SetupSampler()
    {
        glTexture2D.WrapST(sampler.wrapS, sampler.wrapT);
        glTexture2D.Filters(sampler.minFilter, sampler.magFilter);
    }
    void SetupImage(bool isGenMipmap = true)
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
        if (image->data)
        {
            glTexture2D.Image2D(format, image->width, image->height, format, GL_UNSIGNED_BYTE, image->data);
        }
        else if (image->hdrData)
        {
            glTexture2D.Image2D(format, image->width, image->height, format, GL_FLOAT, image->hdrData);
        }
        else
        {
            LOG("image datas is null !")
        }
        glTexture2D.GenMipmap();
    }
    void SetupHDR()
    {
        sampler = TextureSampler(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);
        SetupSampler();
        SetupImage(false);
    }
};

WeakMap<Texture2D> texture2DWeakMap;

#endif