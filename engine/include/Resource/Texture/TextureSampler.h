#ifndef TEXTURE_SAMPLER
#define TEXTURE_SAMPLER

#include "SDK/OpenGL/CppOpenGL.h"

class TextureSampler
{
public:
    GLTexParam magFilter = GLTexParam::LINEAR;
    GLTexParam minFilter = GLTexParam::LINEAR;
    GLTexParam wrapS = GLTexParam::REPEAT;
    GLTexParam wrapT = GLTexParam::REPEAT;
};



#endif