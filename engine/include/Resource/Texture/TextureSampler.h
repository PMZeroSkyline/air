#ifndef TEXTURE_SAMPLER
#define TEXTURE_SAMPLER

#include "SDK/OpenGL/CppOpenGL.h"

class TextureSampler
{
public:
    int magFilter = GL_LINEAR;
    int minFilter = GL_LINEAR;
    int wrapS = GL_REPEAT;
    int wrapT = GL_REPEAT;
};



#endif