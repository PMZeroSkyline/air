#ifndef TEXTURE_SAMPLER
#define TEXTURE_SAMPLER

#include "SDK/OpenGL/CppOpenGL.h"

class TextureSampler
{
public:
    GLenum magFilter = GL_LINEAR;
    GLenum minFilter = GL_LINEAR;
    GLenum wrapS = GL_REPEAT;
    GLenum wrapT = GL_REPEAT;
};



#endif