#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include "Texture.h"

class Texture2D : public Texture
{
public:
    shared_ptr<Image> image;
    GLTexture2D glTexture2D;
    
    void SetupSampler()
    {
        glTexture2D.WrapST(sampler.wrapS, sampler.wrapT);
        glTexture2D.Filters(sampler.minFilter, sampler.magFilter);
    }
    void SetupImage2D(bool isGenMipmap = true)
    {
        glTexture2D.Image2D(glTexture2D.GetTarget(), 0, image->width, image->height, 0, image->channel, image->data, image->hdrData, isGenMipmap);
    }
};

WeakMap<Texture2D> texture2DWeakMap;

#endif