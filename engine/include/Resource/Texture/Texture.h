#ifndef TEXTURE_H
#define TEXTURE_H

#include "TextureSampler.h"
#include "Image.h"
#include "Core/Container/WeakMap.h"
#include "SDK/OpenGL/CppOpenGL.h"

class Texture
{
public:
    string name;
    TextureSampler sampler;

    
};

#endif