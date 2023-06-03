#ifndef TEXTURE_SAMPLER
#define TEXTURE_SAMPLER

#include "SDK/OpenGL/CppOpenGL.h"

class TextureSampler
{
public:
    GLFilter magFilter = GLFilter::LINEAR;
    GLFilter minFilter = GLFilter::LINEAR;
    GLWrap wrapS = GLWrap::REPEAT;
    GLWrap wrapT = GLWrap::REPEAT;
};



#endif