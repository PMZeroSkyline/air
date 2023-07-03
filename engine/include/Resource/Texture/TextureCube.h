#ifndef TEXTURE_CUBE_H
#define TEXTURE_CUBE_H

#include "Texture.h"


class TextureCube : public Texture
{
public:
    vector<shared_ptr<Image>> images; // right, left, top, bottom, front, back
    GLTextureCube glTextureCube;
    TextureCube()
    {
        sampler = TextureSampler(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    }
    
    void SetupSampler()
    {
        glTextureCube.WrapSTR(sampler.wrapS, sampler.wrapT, sampler.wrapR);
        glTextureCube.Filters(sampler.minFilter, sampler.magFilter);
    }
    
    void SetupImageCube(bool isGenMipmap = false)
    {
        if (images.size() != 6)
        {
            LOG("images size need is 6, current size : " << images.size() << " !")
        }
        for (int i = 0; i < images.size(); i++)
        {
            shared_ptr<Image>& image = images[i];
            glTextureCube.Image2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, image->width, image->height, 0, image->channel, image->data, image->hdrData, isGenMipmap);
        }
    }
    void Load(const vector<string> paths)
    {
        if (paths.size() != 6)
        {
            LOG("load paths size need is 6 !")
            return;
        }
        images.resize(6);
        for (int i = 0; i < 6; i++)
        {
            images[i] = make_shared<Image>();
	        images[i]->Load(paths[i]);
        }
        SetupImageCube(false);
        SetupSampler();
    }
};

WeakMap<TextureCube> textureCubeWeakMap;

#endif